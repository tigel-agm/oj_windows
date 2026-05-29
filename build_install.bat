@echo off
setlocal
rem Load MSVC environment (any VS 2019+, located via vswhere)
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" set "VSWHERE=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -prerelease -find VC\Auxiliary\Build\vcvars64.bat`) do set "VCVARS=%%i"
if not defined VCVARS ( echo ERROR: could not locate vcvars64.bat via vswhere. & exit /b 1 )
for %%d in ("%VSWHERE%") do set "PATH=%%~dpd;%PATH%"
call "%VCVARS%"

rem Use the MSVC-built Ruby (adjust this path for your environment)
if exist C:\RubyMSVC34\bin set "PATH=C:\RubyMSVC34\bin;%PATH%"

echo Running bundle install...
call bundle install
endlocal
