# npm install -g uglifycss

param(
    [switch]$DeleteMinifiedFiles
)

# Input and output directory paths
$sourceDirectory = "webserver"
$outputDirectory = "data"

# Define the CSS file sets to be minified and compressed
$cssSets = @{
    "style.css.gz" = "style.css"
}

Write-Host "Source Directory: $sourceDirectory"
Write-Host "Output Directory: $outputDirectory"

# Create the output directory if it doesn't exist
if (-not (Test-Path -Path $outputDirectory)) {
    New-Item -ItemType Directory -Path $outputDirectory | Out-Null
}

Write-Host "Minifying and compressing CSS files..."

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

# Process each CSS set
foreach ($setKey in $cssSets.Keys) {
    $originFile = $cssSets[$setKey]
    $originFilePath = Join-Path -Path $sourceDirectory -ChildPath $originFile

    # Minify the CSS file
    $minifiedFile = Join-Path -Path $outputDirectory -ChildPath ($setKey -replace "\.css\.gz$", ".min.css")
    Write-Host "Minifying CSS file $originFilePath as $minifiedFile..."
    
    # Minify the CSS content
    try {
        uglifycss $originFilePath > $minifiedFile
        Write-Host "Successfully minified $originFilePath to $minifiedFile"
    } catch {
        Write-Host "Error minifying file ${originFilePath}: $($_)"
        continue
    }

    # Compress the minified CSS file
    $gzFilePath = Join-Path -Path $outputDirectory -ChildPath $setKey
    Write-Host "Compressing minified CSS file to $gzFilePath..."
    Compress-ToGzip $minifiedFile $gzFilePath

    # Remove the minified CSS file if specified
    if ($DeleteMinifiedFiles) {
        Remove-Item $minifiedFile -ErrorAction SilentlyContinue
    }
}

Write-Host "Minification and compression process completed."
