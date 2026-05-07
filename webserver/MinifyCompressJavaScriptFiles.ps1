# npm install -g uglify-js

param(
    [switch]$DeleteMinifiedFiles
)

# Input and output directory paths
$sourceDirectory = "webserver"
$outputDirectory = "data"

# Define the JavaScript file sets to be combined and compressed
$javascriptSets = @{
    "index.js.gz" = @("common.js", "themes.js", "captiveportal.js", "captivePortalStatusBar.js", "serverStatusDot.js", "index.js")
    "preferences.js.gz" = @("common.js", "themes.js", "captiveportal.js", "debugWindow.js", "captivePortalStatusBar.js", "serverStatusDot.js", "preferences.js")
    "status.js.gz" = @("common.js", "themes.js", "captiveportal.js", "captivePortalStatusBar.js", "serverStatusDot.js", "status.js")
    "ota.js.gz" = @("common.js", "themes.js", "captiveportal.js", "captivePortalStatusBar.js", "serverStatusDot.js", "ota.js")
    "low_power.js.gz" = @("common.js", "themes.js", "captiveportal.js", "captivePortalStatusBar.js", "serverStatusDot.js", "low_power.js")
}

Write-Host "Source Directory: $sourceDirectory"
Write-Host "Output Directory: $outputDirectory"

# Create the output directory if it doesn't exist
if (-not (Test-Path -Path $outputDirectory)) {
    New-Item -ItemType Directory -Path $outputDirectory | Out-Null
}

Write-Host "Minifying and compressing JavaScript files..."

# Function to compress a file to Gzip
function Compress-ToGzip ($sourceFilePath, $destinationFilePath) {
    try {
        $sourceStream = [System.IO.File]::OpenRead($sourceFilePath)
        $destinationStream = [System.IO.File]::Create($destinationFilePath)
        $gzipStream = New-Object System.IO.Compression.GzipStream($destinationStream, [System.IO.Compression.CompressionMode]::Compress)
        $sourceStream.CopyTo($gzipStream)
        $gzipStream.Close()
        $sourceStream.Close()
        $destinationStream.Close()
        Write-Host "Successfully compressed $sourceFilePath to $destinationFilePath"
    } catch {
        Write-Host "Error compressing file ${sourceFilePath}: $($_)"
    }
}

# Process each JavaScript set
foreach ($setKey in $javascriptSets.Keys) {
    $setFiles = $javascriptSets[$setKey]

    # Initialize or clear the combined arrangement of JavaScript files
    $combinedJavaScriptContent = @()

    # Iterate over the files in the JavaScript set
    foreach ($originFile in $setFiles) {
        $originFilePath = Join-Path -Path $sourceDirectory -ChildPath $originFile

        # Add JavaScript content to the combined file
        try {
            $combinedJavaScriptContent += Get-Content $originFilePath -Raw
            Write-Host "Adding JavaScript file: $originFilePath for combined file: $setKey"
        } catch {
            Write-Host "Error: Could not add JavaScript file ${originFilePath} to combined content: $($_)"
            continue
        }
    }

    # Write the combined JavaScript content to a temporary file
    $tempCombinedFile = Join-Path -Path $outputDirectory -ChildPath ($setKey -replace "\.js\.gz$", ".comb.js")
    try {
        $combinedJavaScriptContent | Out-File -FilePath $tempCombinedFile -Encoding utf8 -ErrorAction Stop
    } catch {
        Write-Host "Error: Could not write combined JavaScript content to the temporary file ${tempCombinedFile}: $($_)"
        exit 1
    }

    # Minify the combined JavaScript
    $minifiedFile = $tempCombinedFile -replace "\.comb\.js$", ".min.js"
    Write-Host "Minifying combined JavaScript file $tempCombinedFile as $minifiedFile..."
    uglifyjs $tempCombinedFile -o $minifiedFile

    # Compress the minified JavaScript file
    $gzFilePath = Join-Path -Path $outputDirectory -ChildPath $setKey
    Write-Host "Compressing minified JavaScript file to $gzFilePath..."
    Compress-ToGzip $minifiedFile $gzFilePath

    # Remove the temporary JavaScript file
    Remove-Item $tempCombinedFile -ErrorAction SilentlyContinue

    # Remove the minified JavaScript file if specified
    if ($DeleteMinifiedFiles) {
        Remove-Item $minifiedFile -ErrorAction SilentlyContinue
    }
}

# If file exists, compress favicon.ico to the output directory
if (Test-Path (Join-Path -Path $sourceDirectory -ChildPath "favicon.ico")) {
    $sourceFilePath = Join-Path -Path $sourceDirectory -ChildPath "favicon.ico"
    $outputFileName = Join-Path -Path $outputDirectory -ChildPath "favicon.ico.gz"
    Write-Host "Compressing favicon.ico to $outputFileName..."
    Compress-ToGzip $sourceFilePath $outputFileName
}

Write-Host "Combination, minification, and compression process completed."
