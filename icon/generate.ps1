$inputFilePath = Join-Path -Path $PSScriptRoot -ChildPath "drawing.svg"
$outputFilePath = Join-Path -Path $PSScriptRoot -ChildPath "icon.ico"
magick $inputFilePath -background transparent -define icon:auto-resize=128,64,48,32,16 $outputFilePath