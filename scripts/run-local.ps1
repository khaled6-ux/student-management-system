$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $PSScriptRoot
Set-Location $Root

if (-not $env:VCPKG_ROOT) {
    throw "VCPKG_ROOT is not set. Install vcpkg and set VCPKG_ROOT to its folder. See README.md."
}
if (-not (Test-Path (Join-Path $env:VCPKG_ROOT "scripts/buildsystems/vcpkg.cmake"))) {
    throw "VCPKG_ROOT does not point to a valid vcpkg installation."
}
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) { throw "CMake is required." }
if (-not (Get-Command npm -ErrorAction SilentlyContinue)) { throw "Node.js and npm are required." }

New-Item -ItemType Directory -Force -Path "data", "logs", "build" | Out-Null
$env:STUDENT_DB_PATH = (Join-Path $Root "data/students.db")
$env:PORT = "8080"
$env:CORS_ORIGIN = "http://127.0.0.1:5173"

if (-not (Test-Path "build/CMakeCache.txt")) {
    cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" -DCMAKE_BUILD_TYPE=Release
    if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed." }
}

cmake --build build --config Release
if ($LASTEXITCODE -ne 0) { throw "C++ build failed." }

if (-not (Test-Path "frontend/node_modules")) {
    Push-Location frontend
    npm install
    if ($LASTEXITCODE -ne 0) { Pop-Location; throw "Frontend dependency installation failed." }
    Pop-Location
}

$ApiExe = Join-Path $Root "build/Release/student-api.exe"
if (-not (Test-Path $ApiExe)) { $ApiExe = Join-Path $Root "build/student-api.exe" }
if (-not (Test-Path $ApiExe)) { throw "Could not find student-api.exe after build." }

$ApiProcess = $null
$FrontendProcess = $null
try {
    $ApiProcess = Start-Process -FilePath $ApiExe -WorkingDirectory $Root -PassThru -RedirectStandardOutput (Join-Path $Root "logs/api.log") -RedirectStandardError (Join-Path $Root "logs/api-error.log")
    Start-Sleep -Seconds 2
    if ($ApiProcess.HasExited) { throw "The C++ API stopped during startup. Check logs/api-error.log." }

    $FrontendProcess = Start-Process -FilePath "cmd.exe" -ArgumentList "/c npm run dev -- --host 127.0.0.1" -WorkingDirectory (Join-Path $Root "frontend") -PassThru -RedirectStandardOutput (Join-Path $Root "logs/frontend.log") -RedirectStandardError (Join-Path $Root "logs/frontend-error.log")
    Start-Sleep -Seconds 3
    if ($FrontendProcess.HasExited) { throw "The frontend stopped during startup. Check logs/frontend-error.log." }

    Start-Process "http://127.0.0.1:5173"
    Write-Host ""
    Write-Host "Student Management System is running locally." -ForegroundColor Green
    Write-Host "Browser: http://127.0.0.1:5173"
    Write-Host "API:     http://127.0.0.1:8080/api/health"
    Write-Host "Data:    $env:STUDENT_DB_PATH"
    Write-Host "Logs:    $Root/logs"
    Write-Host ""
    Write-Host "Keep this window open. Press Ctrl+C to stop both processes."

    while (-not $ApiProcess.HasExited -and -not $FrontendProcess.HasExited) {
        Start-Sleep -Seconds 1
    }
    if ($ApiProcess.HasExited) { throw "The C++ API stopped. Check logs/api-error.log." }
    if ($FrontendProcess.HasExited) { throw "The frontend stopped. Check logs/frontend-error.log." }
}
finally {
    if ($FrontendProcess -and -not $FrontendProcess.HasExited) {
        Stop-Process -Id $FrontendProcess.Id -Force -ErrorAction SilentlyContinue
    }
    if ($ApiProcess -and -not $ApiProcess.HasExited) {
        Stop-Process -Id $ApiProcess.Id -Force -ErrorAction SilentlyContinue
    }
}
