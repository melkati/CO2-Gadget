# Define the paths to the scripts to be executed
$scripts = @(
    "webserver/MinifyCompressHTMLFiles.ps1",
    "webserver/MinifyCompressJavaScriptFiles.ps1",
    "webserver/MinifyCompressCSSFiles.ps1"
)

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

# Loop through each script and execute it
foreach ($script in $scripts) {
    Write-Host "Executing script: ${script}"
    try {
        & $script -DeleteMinifiedFiles
        Write-Host "Successfully executed script: $script"
    } catch {
        Write-Host "Error executing script ${script}: $($_.Exception.Message)"
    }
}

# Compress favicon.png if it exists
$sourceFavicon = "webserver/favicon.png"
$outputFavicon = "data/favicon.png.gz"

if (Test-Path $sourceFavicon) {
    Write-Host "Compressing favicon.png to $outputFavicon..."
    Compress-ToGzip $sourceFavicon $outputFavicon
} else {
    Write-Host "Favicon.png not found."
}

Write-Host "All scripts have been executed."
