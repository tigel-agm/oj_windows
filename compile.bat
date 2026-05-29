@echo off
setlocal
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" set "VSWHERE=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -prerelease -find VC\Auxiliary\Build\vcvars64.bat`) do set "VCVARS=%%i"
if not defined VCVARS ( echo ERROR: could not locate vcvars64.bat via vswhere. & exit /b 1 )
for %%d in ("%VSWHERE%") do set "PATH=%%~dpd;%PATH%"
call "%VCVARS%"
if exist C:\RubyMSVC34\bin set "PATH=C:\RubyMSVC34\bin;%PATH%"
echo Compiling...
call bundle exec rake compile
echo Done.
endlocal
