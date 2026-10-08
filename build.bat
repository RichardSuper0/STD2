@echo off
setlocal enabledelayedexpansion

del build.sh build.ps1 >nul 2>&1

if not "%PROCESSOR_ARCHITECTURE%"=="ARM64" (
    echo Errore: Architettura non supportata. std2 richiede ARM64.
    exit /b 1
)

if not exist "deps.zip" (
    echo Errore: file deps.zip non trovato nella directory corrente.
    exit /b 1
)

echo Estrazione selettiva della cartella 'arm64' dallo zip...
powershell -Command "Add-Type -AssemblyName System.IO.Compression.FileSystem; $zip = [System.IO.Compression.ZipFile]::OpenRead('deps.zip'); foreach($e in $zip.Entries) { if($e.FullName.StartsWith('arm64/')) { $target = Join-Path (Get-Location) $e.FullName; $dir = [System.IO.Path]::GetDirectoryName($target); if(-not (Test-Path $dir)) { New-Item -ItemType Directory -Path $dir | Out-Null }; if(-not $target.EndsWith('/')) { [System.IO.Compression.ZipFileExtensions]::ExtractToFile($e, $target, $true) } } }; $zip.Dispose()"

echo Spostamento dei file estratti nella cartella principale...
xcopy "arm64\*" ".\" /E /Y >nul

echo Pulizia dei file temporanei e dello zip...
rd /s /q arm64
del deps.zip

echo ✅ Estrazione nativa completata con successo!
