#include "signal_filter.h"

namespace {
int saturate(int value, int minimum, int maximum) {
    if (value < minimum) {
        return minimum;
    }
    if (value > maximum) {
        return maximum;
    }
    return value;
}

void push(int *buffer, uint8_t size, int value) {
    for (int i = size - 1; i > 0; --i) {
        buffer[i] = buffer[i - 1];
    }
    buffer[0] = value;
}

void sort(int *window, uint8_t size) {
    for (uint8_t i = 0; i < size - 1; ++i) {
        for (uint8_t j = i + 1; j < size; ++j) {
            if (window[i] > window[j]) {
                const int temporary = window[i];
                window[i] = window[j];
                window[j] = temporary;
            }
        }
    }
}
}

void SignalFilter::reset() {
    initialized = false;
}

void SignalFilter::initialize(int value) {
    // Prima valoare reala umple istoricul, fara zerouri artificiale.
    for (uint8_t i = 0; i < Config::MEDIAN_SIZE; ++i) {
        medianBuffer[i] = value;
    }
    for (uint8_t i = 0; i < Config::AVERAGE_SIZE; ++i) {
        averageBuffer[i] = value;
    }
    initialized = true;
}

int SignalFilter::median(int value) {
    push(medianBuffer, Config::MEDIAN_SIZE, value);
    int window[Config::MEDIAN_SIZE];
    for (uint8_t i = 0; i < Config::MEDIAN_SIZE; ++i) {
        window[i] = medianBuffer[i];
    }
    // Sortam copia pentru a pastra ordinea istoricului.
    sort(window, Config::MEDIAN_SIZE);
    return window[Config::MEDIAN_SIZE / 2];
}

int SignalFilter::average(int value) {
    push(averageBuffer, Config::AVERAGE_SIZE, value);
    long weightedSum = 0;
    int weightSum = 0;
    for (uint8_t i = 0; i < Config::AVERAGE_SIZE; ++i) {
        // Conversia precede inmultirea: int are 16 biti pe Arduino Mega.
        weightedSum += static_cast<long>(averageBuffer[i]) * Config::WEIGHTS[i];
        weightSum += Config::WEIGHTS[i];
    }
    return static_cast<int>(weightedSum / weightSum);
}

void SignalFilter::process(int input, int minimum, int maximum,
                           int &saturatedValue, int &medianValue, int &averageValue) {
    saturatedValue = saturate(input, minimum, maximum);
    if (!initialized) {
        initialize(saturatedValue);
    }
    medianValue = median(saturatedValue);
    averageValue = average(medianValue);
}
