param(
    [switch]$DeleteMinifiedFiles
)

# Input and output directory paths
$sourceDirectory = "webserver"
$outputDirectory = "data"
$tempCombinedFileNew = "$sourceDirectory\temp_combined_new.js"  # Temporary file for combined JavaScript
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
                html-minifier "$($file.FullName)" -o "$($sourceDirectory)\$intermediateFileName" --collapse-whitespace --minify-css true --minify-js true
                
                if (Test-Path "$($outputDirectory)\$outputFileName") {
                    Remove-Item "$($outputDirectory)\$outputFileName" -ErrorAction SilentlyContinue
                }

                & $zipExecutable a -tgzip "$($outputDirectory)\$outputFileName" "$($sourceDirectory)\$intermediateFileName"
                Remove-Item "$($sourceDirectory)\$intermediateFileName" -ErrorAction SilentlyContinue
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
            if ($file.Name -in @("common.js", "themes.js", "captiveportal.js", "captivePortalStatusBar.js", "serverStatusDot.js", "debugWindow.js") -and $file.Name -notlike "*.min.*") {
                Write-Host "Adding JavaScript file: $($file.FullName)"
                try {
                    # Add JavaScript content to the list
                    $combinedJavaScriptContent += Get-Content $file.FullName -Raw
                } catch {
                    Write-Host "Error: Could not add JavaScript file $($file.FullName) to combined content: $_"
                    continue
                }
            } elseif ($file.Name -notlike "*.min.*") {
                # Minify the JavaScript file
                $minifiedFileName = "$($outputDirectory)\$($file.BaseName).min.js"  # Full path to the minified file in the output directory
                Write-Host "Minifying JavaScript file $($file.FullName) to $minifiedFileName..."
                uglifyjs $file.FullName -o $minifiedFileName
                
                if (Test-Path $minifiedFileName) {
                    # If minified file is created successfully, proceed with compression
                    $compressedFileName = "$($outputDirectory)\$($file.BaseName).js.gz"
                
                    Write-Host "Compressing minified JavaScript file to $compressedFileName..."

                    # Remove existing .gz file if it exists
                    if (Test-Path $compressedFileName) {
                        Remove-Item $compressedFileName -ErrorAction SilentlyContinue
                    }
                    
                    # Compress the minified file using 7-Zip
                    & $zipExecutable a -tgzip $compressedFileName $minifiedFileName
                    
                    Write-Host "Successfully compressed $($file.FullName) to $compressedFileName"
                    
                    if ($DeleteMinifiedFiles) {
                        Remove-Item $minifiedFileName -ErrorAction SilentlyContinue
                    }
                } else {
                    Write-Host "Failed to minify $($file.FullName) to $minifiedFileName"
                }                
            }
            else {
                Write-Host "File $($file.FullName) is either already minified, not required, or is the combined file. Skipping..."
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
uglifyjs $tempCombinedFileNew -o "$sourceDirectory\combined.min.js"

# Remove any existing .gz files for combined.js
if (Test-Path "$outputDirectory\$javascriptCombinedOutputFileName") {
    Remove-Item "$outputDirectory\$javascriptCombinedOutputFileName" -ErrorAction SilentlyContinue
}

# Compress the minified JavaScript file directly to combined.js.gz
Write-Host "Compressing minified JavaScript file to $javascriptCombinedOutputFileName..."
& $zipExecutable a -tgzip "$outputDirectory\$javascriptCombinedOutputFileName" "$sourceDirectory\combined.min.js"

# Remove the temporary JavaScript file
Remove-Item $tempCombinedFileNew -ErrorAction SilentlyContinue

# Remove the combined.min.js file
Remove-Item "$sourceDirectory\combined.min.js" -ErrorAction SilentlyContinue

# If file exist, compress favicon.ico to the output directory
if (Test-Path "$sourceDirectory\favicon.ico") {
    $outputFileName = "$outputDirectory\favicon.ico.gz"
    Write-Host "Compressing favicon.ico to $outputFileName..."
    & $zipExecutable a -tgzip "$outputFileName" "$sourceDirectory\favicon.ico"
}

Write-Host "Combination, minification, and compression process completed."
