# Rutas de directorios de entrada y salida
$sourceDirectory = "data"
$outputDirectory = "$sourceDirectory"
$tempCombinedFile = "$outputDirectory\combined.js"
$outputFileName = "combined.js.gz"

# Ruta del ejecutable 7z.exe
$zipExecutable = "C:\Program Files\7-Zip\7z.exe"

Write-Host "Source Directory: $sourceDirectory"
Write-Host "Output Directory: $outputDirectory"

# Crear el directorio de salida si no existe
if (-not (Test-Path -Path $outputDirectory)) {
    New-Item -ItemType Directory -Path $outputDirectory | Out-Null
    Write-Host "Output directory created."
} else {
    Write-Host "Output directory already exists."
}

Write-Host "Combining and minifying JavaScript files..."

# Inicializar o limpiar el archivo combinado
Set-Content -Path $tempCombinedFile -Value ""

# Iterar sobre los archivos en el directorio de origen y combinar JS, excluyendo el archivo combinado temporal
Get-ChildItem -Path $sourceDirectory -Filter *.js | Where-Object { $_.Name -ne "combined.js" -and $_.Name -ne "combined.min.js" } | ForEach-Object {
    $file = $_
    if ($file.Name -notlike "*.min.*") {
        Write-Host "Adding JavaScript file: $($file.FullName)"
        Get-Content $file.FullName | Add-Content $tempCombinedFile
    } else {
        Write-Host "File $($file.FullName) is already minified. Skipping..."
    }
}

# Minificar el archivo combinado
Write-Host "Minifying combined JavaScript file..."
uglifyjs $tempCombinedFile -o "$outputDirectory\combined.min.js"

# Eliminar cualquier archivo .gz existente
if (Test-Path "$outputDirectory\$outputFileName") {
    Remove-Item "$outputDirectory\$outputFileName"
}

# Comprimir el archivo minificado directamente a combined.js.gz
Write-Host "Compressing minified JavaScript file to combined.js.gz..."
& $zipExecutable a -tgzip "$outputDirectory\$outputFileName" "$outputDirectory\combined.min.js"

# Eliminar los archivos temporales
Remove-Item $tempCombinedFile
Remove-Item "$outputDirectory\combined.min.js"

Write-Host "Combination, minification, and compression process completed."
