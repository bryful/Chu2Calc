param(
    [string]$BuildDirectory = 'build',
    [string]$OutputDirectory = 'release'
)
$ErrorActionPreference = 'Stop'
$workspace = Split-Path -Parent $PSScriptRoot
$buildRoot = Join-Path $workspace $BuildDirectory
$outputRoot = Join-Path $workspace $OutputDirectory
$executable = Join-Path $buildRoot 'Release/Chu2Calc.exe'
if (-not (Test-Path -LiteralPath $executable))
{
    throw "Release executable not found: $executable"
}
$version = (Get-Item -LiteralPath $executable).VersionInfo.ProductVersion
if ($version -notmatch '^\d+\.\d+\.\d+$') { throw 'Executable has no valid release version.' }
$name = "Chu2Calc-$version-windows-x64"
$package = Join-Path $outputRoot $name
$archive = Join-Path $outputRoot "$name.zip"
if ((Test-Path -LiteralPath $package) -or (Test-Path -LiteralPath $archive))
{
    throw "Release output already exists: $package"
}
New-Item -ItemType Directory -Path $package | Out-Null
Copy-Item -LiteralPath $executable -Destination $package
foreach ($document in @('README.md', 'DEVELOPMENT.md', 'RELEASE_NOTES.md'))
{
    Copy-Item -LiteralPath (Join-Path $workspace $document) -Destination $package
}
$exeHash = (Get-FileHash -LiteralPath (Join-Path $package 'Chu2Calc.exe') -Algorithm SHA256).Hash.ToLowerInvariant()
Set-Content -LiteralPath (Join-Path $package 'SHA256SUMS.txt') -Value "$exeHash  Chu2Calc.exe" -Encoding utf8
Compress-Archive -LiteralPath $package -DestinationPath $archive
$zipHash = (Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash.ToLowerInvariant()
Set-Content -LiteralPath "$archive.sha256" -Value "$zipHash  $name.zip" -Encoding utf8
Get-Item -LiteralPath $archive, "$archive.sha256" | Select-Object FullName, Length
