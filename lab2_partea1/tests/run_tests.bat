@echo off
setlocal
cd /d "%~dp0.."
if not exist tests\build mkdir tests\build
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cl /nologo /EHsc /W4 /std:c++14 /Itests\fakes /Iinclude tests\test_logic.cpp src\AppState.cpp src\AppTasks.cpp src\Button.cpp src\Led.cpp src\Scheduler.cpp /Fotests\build\ /Fetests\build\test_logic.exe
if errorlevel 1 exit /b 1
tests\build\test_logic.exe
