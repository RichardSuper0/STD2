Remove-Item -Path "build.sh", "build.bat" -ErrorAction SilentlyContinue

$arch = [System.Runtime.InteropServices.RuntimeInformation]::ProcessArchitecture
if ($arch -ne "Arm64") {
    Write-Error "Errore: Architettura non supportata. std2 richiede ARM64."
    exit
}

if (-not (Test-Path "deps.zip")) {
    Write-Error "Errore: file deps.zip non trovato nella directory corrente."
    exit
}

Write-Host "Estrazione selettiva della cartella 'arm64' dallo zip..."
Add-Type -AssemblyName System.IO.Compression.FileSystem
$zip = [System.IO.Compression.ZipFile]::OpenRead("deps.zip")
$currentPath = Get-Location

foreach ($entry in $zip.Entries) {
    if ($entry.FullName.StartsWith("arm64/")) {
        $targetPath = Join-Path $currentPath $entry.FullName
        $directory = [System.IO.Path]::GetDirectoryName($targetPath)
        
        if (-not (Test-Path $directory)) {
            New-Item -ItemType Directory -Path $directory | Out-Null
        }
        
        if (-not $targetPath.EndsWith("/")) {
            [System.IO.Compression.ZipFileExtensions]::ExtractToFile($entry, $targetPath, $true)
        }
    }
}
$zip.Dispose()

Write-Host "Spostamento dei file estratti nella cartella principale..."
Move-Item -Path "arm64/*" -Destination "./" -Force

Write-Host "Pulizia dei file temporanei e dello zip..."
Remove-Item -Recurse -Force "arm64", "deps.zip"

Write-Host "✅ Estrazione nativa completata con successo!" -ForegroundColor Green
