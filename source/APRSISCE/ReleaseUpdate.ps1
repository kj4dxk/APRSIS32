param([string]$Target, [string]$Installed, [int]$ParentId)

$ErrorActionPreference = 'Stop'
$stage = $PSScriptRoot
$request = Join-Path $stage 'restart'
$status = Join-Path $stage 'status'
$download = Join-Path $stage 'download.exe'
$directory = [IO.Path]::GetDirectoryName($Target)
$backup = Join-Path $directory ('APRSIS32-backup-' + [guid]::NewGuid() + '.exe')
$replacing = $false
$child = $null
$event = $null

try {
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
    $app = Get-Process -Id $ParentId
    if ($app.Path -ine $Target) { throw 'The original APRSIS32 process has changed.' }
    $release = Invoke-RestMethod -UseBasicParsing -Uri 'https://api.github.com/repos/kj4dxk/APRSIS32/releases/latest' -TimeoutSec 15
    $version = [string]$release.tag_name
    if ($release.draft -or $release.prerelease -or $version -notmatch '^\d{4}\.\d{2}\.\d{2}\.\d{4}$' -or
        [string]::CompareOrdinal($version,$Installed) -le 0) { throw 'No newer stable release is available.' }
    $asset = @($release.assets | Where-Object name -ceq 'APRSIS32.exe')
    $url = "https://github.com/kj4dxk/APRSIS32/releases/download/$version/APRSIS32.exe"
    if ($asset.Count -ne 1 -or $asset[0].browser_download_url -cne $url -or
        $asset[0].digest -notmatch '^sha256:[0-9a-f]{64}$') { throw 'The release asset or its SHA-256 digest is missing.' }
    Invoke-WebRequest -UseBasicParsing -Uri $url -OutFile $download -TimeoutSec 180
    if ((Get-FileHash -LiteralPath $download -Algorithm SHA256).Hash -ine $asset[0].digest.Substring(7)) {
        throw 'The update failed SHA-256 verification.'
    }
    $info = (Get-Item -LiteralPath $download).VersionInfo
    $actual = '{0:0000}.{1:00}.{2:00}.{3:0000}' -f $info.FileMajorPart,$info.FileMinorPart,$info.FileBuildPart,$info.FilePrivatePart
    if ($actual -cne $version) { throw 'The executable version does not match the release.' }
    $closeRequested = $true
    [IO.File]::WriteAllText($request,'')
    if (!$app.WaitForExit(90000)) { throw 'APRSIS32 did not close. The original executable is intact.' }
    $updateName = 'Local\APRSIS32-Update-' + [guid]::NewGuid()
    $event = [Threading.EventWaitHandle]::new($false,[Threading.EventResetMode]::ManualReset,$updateName)
    $env:APRSIS32_UPDATE_EVENT = $updateName
    $localDownload = Join-Path $directory ('APRSIS32-download-' + [guid]::NewGuid() + '.exe')
    [IO.File]::Copy($download,$localDownload)
    [IO.File]::Replace($localDownload,$Target,$backup)
    $replacing = $true
    $child = Start-Process -FilePath $Target -WorkingDirectory $directory -PassThru
    $deadline = [DateTime]::UtcNow.AddSeconds(60)
    while (!$event.WaitOne(250)) {
        if ($child.HasExited -or [DateTime]::UtcNow -ge $deadline) { throw 'The updated program did not confirm startup.' }
    }
    [IO.File]::WriteAllText($status,"Started $version. Backup: $backup")
} catch {
    $failure = $_.Exception.Message
    if ($closeRequested -and $app.HasExited) {
        try {
            if ($replacing) {
                if ($child -and !$child.HasExited) {
                    [void]$child.CloseMainWindow()
                    if (!$child.WaitForExit(10000)) { throw "Close APRSIS32 before restoring $backup." }
                }
                [IO.File]::Replace($backup,$Target,($backup + '.failed'))
            }
            Remove-Item Env:APRSIS32_UPDATE_EVENT -ErrorAction SilentlyContinue
            Start-Process -FilePath $Target -WorkingDirectory $directory
        } catch { $failure += ' ' + $_.Exception.Message }
    }
    [IO.File]::WriteAllText($status,$failure,[Text.Encoding]::ASCII)
    if ($closeRequested) {
        Add-Type -AssemblyName System.Windows.Forms
        [void][Windows.Forms.MessageBox]::Show($failure,'APRSIS32 Update')
    }
} finally {
    if ($event) { $event.Dispose() }
    if ($localDownload -and (Test-Path -LiteralPath $localDownload)) { Remove-Item -LiteralPath $localDownload }
    if (Test-Path -LiteralPath $download) { Remove-Item -LiteralPath $download }
}
