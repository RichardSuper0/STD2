set -e

rm -f build.bat build.ps1

ARCH=$(uname -m)
if [ "$ARCH" != "aarch64" ] && [ "$ARCH" != "arm64" ]; then
    echo "Errore: Architettura non supportata. std2 richiede ARM64."
    exit 1
fi

if [ ! -f "deps.zip" ]; then
    echo "Errore: file deps.zip non trovato nella directory corrente."
    exit 1
fi

echo "Estrazione selettiva della cartella 'arm64' dallo zip..."
unzip -q deps.zip "arm64/*"

echo "Spostamento dei file estratti nella cartella principale..."
mv arm64/* ./

echo "Pulizia dei file temporanei e dello zip..."
rm -rf arm64 deps.zip

echo "✅ Estrazione nativa completata con successo!"
