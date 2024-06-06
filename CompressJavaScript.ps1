# Input and output directory paths
$sourceDirectory = "data"
$outputDirectory = "$sourceDirectory"
$tempCombinedFileNew = "$outputDirectory\temp_combined_new.js"  # Temporary file for combined JavaScript
$javascriptCombinedOutputFileName = "combined.js.gz"  # Compressed file name for the combined JavaScript

# Path to 7z.exe executable
$zipExecutable = "C:\Program Files\7-Zip\7z.exe"

Write-Host "Source Directory: $sourceDirectory"
Write-Host "Output Directory: $outputDirectory"

# Create the output directory if it doesn't exist
if (-not (Test-Path -Path $outputDirectory)) {
    New-Item -ItemType Directory -Path $outputDirectory | Out-Null
}

Write-Host "Minifying HTML, CSS, and JavaScript files..."

# Initialize or clear the combined JavaScript file content
$combinedJavaScriptContent = @()

# Iterate over the files in the source directory
Get-ChildItem -Path $sourceDirectory | Where-Object { $_.Extension -in @(".html", ".css", ".js") -and $_.Name -notlike "*.gz" } | ForEach-Object {
    $file = $_

    switch -Wildcard ($file.Extension) {
        "*.html" {
            Write-Host "Processing HTML file: $($file.FullName)"
            if ($file.Name -notlike "*.min.*") {
                $intermediateFileName = "$($file.BaseName).min.html"
                $outputFileName = "$($file.BaseName).html.gz"
                html-minifier "$($file.FullName)" -o "$($outputDirectory)\$intermediateFileName" --collapse-whitespace --minify-css true --minify-js true
                
                if (Test-Path "$($outputDirectory)\$outputFileName") {
                    Remove-Item "$($outputDirectory)\$outputFileName" -ErrorAction SilentlyContinue
                }

                & $zipExecutable a -tgzip "$($outputDirectory)\$outputFileName" "$($outputDirectory)\$intermediateFileName"
                Remove-Item "$($outputDirectory)\$intermediateFileName" -ErrorAction SilentlyContinue
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
                    Remove-Item "$($outputDirectory)\$outputFileName" -ErrorAction SilentlyContinue
                }

                & $zipExecutable a -tgzip "$($outputDirectory)\$outputFileName" "$($outputDirectory)\$intermediateFileName"
                Remove-Item "$($outputDirectory)\$intermediateFileName" -ErrorAction SilentlyContinue
            } else {
                Write-Host "File $($file.FullName) is already minified. Skipping..."
            }
        }
        "*.js" {
            # Process JavaScript files
            if ($file.Name -notlike "*.min.*" -and $file.Name -ne "combined.js" -and $file.Name -ne "combined.min.js") {
                Write-Host "Adding JavaScript file: $($file.FullName)"
                try {
                    # Add JavaScript content to the list
                    $combinedJavaScriptContent += Get-Content $file.FullName -Raw
                } catch {
                    Write-Host "Error: Could not add JavaScript file $($file.FullName) to combined content: $_"
                    continue
                }
            } else {
                Write-Host "File $($file.FullName) is already minified or is the combined file. Skipping..."
            }
        }
        default {
            Write-Host "File $($file.FullName) is not an HTML, CSS, or JavaScript file. Skipping..."
        }
    }
}

# Write the combined JavaScript content to the temporary file
try {
    $combinedJavaScriptContent | Out-File -FilePath $tempCombinedFileNew -Encoding utf8 -ErrorAction Stop
} catch {
    Write-Host "Error: Could not write combined JavaScript content to the temporary file: $_"
    exit 1
}

# Minify the combined JavaScript
Write-Host "Minifying combined JavaScript file..."
uglifyjs $tempCombinedFileNew -o "$outputDirectory\combined.min.js"

# Remove any existing .gz files for combined.js
if (Test-Path "$outputDirectory\$javascriptCombinedOutputFileName") {
    Remove-Item "$outputDirectory\$javascriptCombinedOutputFileName" -ErrorAction SilentlyContinue
}

# Compress the minified JavaScript file directly to combined.js.gz
Write-Host "Compressing minified JavaScript file to $javascriptCombinedOutputFileName..."
& $zipExecutable a -tgzip "$outputDirectory\$javascriptCombinedOutputFileName" "$outputDirectory\combined.min.js"

# Remove the temporary JavaScript file
Remove-Item $tempCombinedFileNew -ErrorAction SilentlyContinue

# Remove the combined.min.js file
Remove-Item "$outputDirectory\combined.min.js" -ErrorAction SilentlyContinue

Write-Host "Combination, minification, and compression process completed."
