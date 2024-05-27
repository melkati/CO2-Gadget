
# Rutas de directorios de entrada y salida
$sourceDirectory = "data"
$outputDirectory = "$sourceDirectory"

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

Write-Host "Minifying HTML, CSS, and JavaScript files..."

# Iterar sobre los archivos en el directorio de origen
Get-ChildItem -Path $sourceDirectory | ForEach-Object {
    $file = $_

    # Verificar si el archivo tiene una de las extensiones que queremos procesar
    switch -Wildcard ($file.Extension) {
        "*.html" {
            Write-Host "Processing HTML file: $($file.FullName)"
            if ($file.Name -notlike "*.min.*") {
                $intermediateFileName = "$($file.BaseName).min.html"
                $outputFileName = "$($file.BaseName).html.gz"
                html-minifier "$($file.FullName)" -o "$($outputDirectory)\$intermediateFileName" --collapse-whitespace --minify-css true --minify-js true
                
                if (Test-Path "$($outputDirectory)\$outputFileName") {
                    Remove-Item "$($outputDirectory)\$outputFileName"
                }

                & $zipExecutable a -tgzip "$($outputDirectory)\$outputFileName" "$($outputDirectory)\$intermediateFileName"
                Remove-Item "$($outputDirectory)\$intermediateFileName"
            } else {
                Write-Host "File $($file.FullName) is already minified. Skipping..."
            }
        }
        "*.css" {
            Write-Host "Processing CSS file: $($file.FullName)"
            if ($file.Name -notlike "*.min.*") {
                $intermediateFileName = "$($file.BaseName).min.css"
                $outputFileName = "$($file.BaseName).css.gz"
                uglifycss "$($file.FullName)" --output "$($outputDirectory)\$intermediateFileName"
                
                if (Test-Path "$($outputDirectory)\$outputFileName") {
                    Remove-Item "$($outputDirectory)\$outputFileName"
                }

                & $zipExecutable a -tgzip "$($outputDirectory)\$outputFileName" "$($outputDirectory)\$intermediateFileName"
                Remove-Item "$($outputDirectory)\$intermediateFileName"
            } else {
                Write-Host "File $($file.FullName) is already minified. Skipping..."
            }
        }
        "*.js" {
            Write-Host "Processing JavaScript file: $($file.FullName)"
            if ($file.Name -notlike "*.min.*") {
                $intermediateFileName = "$($file.BaseName).min.js"
                $outputFileName = "$($file.BaseName).js.gz"
                uglifyjs "$($file.FullName)" -o "$($outputDirectory)\$intermediateFileName"
                
                if (Test-Path "$($outputDirectory)\$outputFileName") {
                    Remove-Item "$($outputDirectory)\$outputFileName"
                }

                & $zipExecutable a -tgzip "$($outputDirectory)\$outputFileName" "$($outputDirectory)\$intermediateFileName"
                Remove-Item "$($outputDirectory)\$intermediateFileName"
            } else {
                Write-Host "File $($file.FullName) is already minified. Skipping..."
            }
        }
        default {
            Write-Host "File $($file.FullName) is not an HTML, CSS, or JavaScript file. Skipping..."
        }
    }
}

Write-Host "Compression process completed."
