# npm install -g html-minifier

param(
    [switch]$DeleteMinifiedFiles
)

# Input and output directory paths
$sourceDirectory = "webserver"
$outputDirectory = "data"

# Define the HTML file sets to be minified and compressed
$htmlSets = @{
    "index.html.gz" = "index.html"
    "preferences.html.gz" = "preferences.html"
    "status.html.gz" = "status.html"
    "ota.html.gz" = "ota.html"    
}

Write-Host "Source Directory: $sourceDirectory"
Write-Host "Output Directory: $outputDirectory"

# Create the output directory if it doesn't exist
if (-not (Test-Path -Path $outputDirectory)) {
    New-Item -ItemType Directory -Path $outputDirectory | Out-Null
}

Write-Host "Minifying and compressing HTML files..."

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

# Process each HTML set
foreach ($setKey in $htmlSets.Keys) {
    $originFile = $htmlSets[$setKey]
    $originFilePath = Join-Path -Path $sourceDirectory -ChildPath $originFile

    # Minify the HTML file
    $minifiedFile = Join-Path -Path $outputDirectory -ChildPath ($setKey -replace "\.html\.gz$", ".min.html")
    Write-Host "Minifying HTML file $originFilePath as $minifiedFile..."
    
    # Minify the HTML content
    try {
        html-minifier $originFilePath --collapse-whitespace --remove-comments --minify-css true --minify-js true -o $minifiedFile
        Write-Host "Successfully minified $originFilePath to $minifiedFile"
    } catch {
        Write-Host "Error minifying file ${originFilePath}: $($_)"
        continue
    }

    # Compress the minified HTML file
    $gzFilePath = Join-Path -Path $outputDirectory -ChildPath $setKey
    Write-Host "Compressing minified HTML file to $gzFilePath..."
    Compress-ToGzip $minifiedFile $gzFilePath

    # Remove the minified HTML file if specified
    if ($DeleteMinifiedFiles) {
        Remove-Item $minifiedFile -ErrorAction SilentlyContinue
    }
}

Write-Host "Minification and compression process completed."
