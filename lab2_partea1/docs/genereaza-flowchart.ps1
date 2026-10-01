$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing

# Diagrama functionala a programului; regenerare fara dependinte externe.
$bitmap = [System.Drawing.Bitmap]::new(1300, 1300)
$bitmap.SetResolution(180, 180)
$g = [System.Drawing.Graphics]::FromImage($bitmap)
$g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
$g.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::AntiAliasGridFit
$g.Clear([System.Drawing.ColorTranslator]::FromHtml('#FFFFFF'))

function Brush([string]$color) {
    return [System.Drawing.SolidBrush]::new([System.Drawing.ColorTranslator]::FromHtml($color))
}
function Label([string]$text, [float]$x, [float]$y, [float]$w, [float]$h,
               [int]$size = 28, [string]$color = '#172B42', [bool]$bold = $false,
               [bool]$center = $false) {
    $style = [System.Drawing.FontStyle]::Regular
    if ($bold) { $style = [System.Drawing.FontStyle]::Bold }
    $font = [System.Drawing.Font]::new('Segoe UI', $size, $style, [System.Drawing.GraphicsUnit]::Pixel)
    $format = [System.Drawing.StringFormat]::new()
    $format.LineAlignment = [System.Drawing.StringAlignment]::Center
    if ($center) { $format.Alignment = [System.Drawing.StringAlignment]::Center }
    $brush = Brush $color
    $g.DrawString($text, $font, $brush, [System.Drawing.RectangleF]::new($x,$y,$w,$h), $format)
    $brush.Dispose(); $font.Dispose(); $format.Dispose()
}
function Box([float]$x, [float]$y, [float]$w, [float]$h,
             [string]$title, [string]$body, [string]$fill = '#FFFFFF',
             [string]$accent = '#2563A6') {
    $rect = [System.Drawing.RectangleF]::new($x,$y,$w,$h)
    $brush = Brush $fill
    $pen = [System.Drawing.Pen]::new([System.Drawing.ColorTranslator]::FromHtml($accent), 2)
    $g.FillRectangle($brush, $rect)
    $g.DrawRectangle($pen, $x,$y,$w,$h)
    $stripe = Brush $accent
    $g.FillRectangle($stripe,$x,$y,7,$h)
    Label $title ($x+24) ($y+14) ($w-48) 48 32 $accent $true
    Label $body ($x+24) ($y+65) ($w-48) ($h-75) 27
    $brush.Dispose(); $pen.Dispose(); $stripe.Dispose()
}
function Arrow([float[]]$coords, [string]$color = '#52677D') {
    $points = [System.Drawing.PointF[]]::new($coords.Length / 2)
    for ($i=0; $i -lt $points.Length; $i++) {
        $points[$i] = [System.Drawing.PointF]::new($coords[$i*2],$coords[$i*2+1])
    }
    $pen = [System.Drawing.Pen]::new([System.Drawing.ColorTranslator]::FromHtml($color), 4)
    $cap = [System.Drawing.Drawing2D.AdjustableArrowCap]::new(5,6)
    $pen.CustomEndCap = $cap
    $g.DrawLines($pen,$points)
    $pen.Dispose(); $cap.Dispose()
}

Label 'Fluxul circuitului' 100 45 1100 70 44 '#172B42' $true $true

Box 130 170 440 115 'Buton B1' 'Comandă LED1' '#F1F6FB'
Box 730 170 440 115 'Butoane B+ / B−' 'Reglează clipirea' '#F1F6FB'
Arrow @(350,285,350,370)
Arrow @(950,285,950,370)

Box 130 370 440 130 'Comutare LED1' 'Aprinde / stinge LED1'
Box 730 370 440 130 'Reglare durată' 'B+: mai lent • B−: mai rapid'
Arrow @(350,500,350,590)
Arrow @(950,500,950,590)

Box 130 590 1040 130 'Control LED2' 'Verifică starea LED1 și durata clipirii' '#F1F6FB'
Arrow @(350,720,350,850)
Arrow @(950,720,950,850)
Label 'LED1 aprins' 370 755 220 45 26 '#52677D'
Label 'LED1 stins' 970 755 220 45 26 '#52677D'

Box 130 850 440 130 'LED2 stins' 'Clipire oprită'
Box 730 850 440 130 'LED2 clipește' 'Folosește durata aleasă'
Arrow @(350,980,350,1100)
Arrow @(950,980,950,1100)

Box 130 1100 1040 130 'Monitor Serial' 'Afișează starea sistemului' '#F1F6FB'
$outputPath = Join-Path $PSScriptRoot 'flowchart-circuit.png'
$bitmap.Save($outputPath, [System.Drawing.Imaging.ImageFormat]::Png)
$g.Dispose(); $bitmap.Dispose()
Write-Output $outputPath
