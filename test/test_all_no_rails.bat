@echo off
REM oj_windows test runner - core tests without Rails
REM Uses MSVC Ruby at C:\RubyMSVC34

setlocal

cd /d "%~dp0"

REM Set MSVC Ruby path
set PATH=C:\RubyMSVC34\bin;%PATH%

echo Using Ruby: 
C:\RubyMSVC34\bin\ruby --version
echo.

echo ----- General tests (test_various.rb) -----
C:\RubyMSVC34\bin\ruby test_various.rb
if errorlevel 1 exit /b 1

echo ----- Strict parser tests (test_strict.rb) -----
C:\RubyMSVC34\bin\ruby test_strict.rb
if errorlevel 1 exit /b 1

echo ----- Compat parser tests (test_compat.rb) -----
C:\RubyMSVC34\bin\ruby test_compat.rb
if errorlevel 1 exit /b 1

echo ----- Object parser tests (test_object.rb) -----
C:\RubyMSVC34\bin\ruby test_object.rb
if errorlevel 1 exit /b 1

echo ----- Fast tests (test_fast.rb) -----
C:\RubyMSVC34\bin\ruby test_fast.rb
if errorlevel 1 exit /b 1

echo ----- SAJ parser tests (test_saj.rb) -----
C:\RubyMSVC34\bin\ruby test_saj.rb
if errorlevel 1 exit /b 1

echo ----- SC Parser tests (test_scp.rb) -----
C:\RubyMSVC34\bin\ruby test_scp.rb
if errorlevel 1 exit /b 1

echo ----- GC tests (test_gc.rb) -----
C:\RubyMSVC34\bin\ruby test_gc.rb
if errorlevel 1 exit /b 1

echo ----- Writer tests (test_writer.rb) -----
C:\RubyMSVC34\bin\ruby test_writer.rb
if errorlevel 1 exit /b 1

echo ----- File loading tests (test_file.rb) -----
C:\RubyMSVC34\bin\ruby test_file.rb
if errorlevel 1 exit /b 1

echo ----- Hash loading tests (test_hash.rb) -----
C:\RubyMSVC34\bin\ruby test_hash.rb
if errorlevel 1 exit /b 1

echo.
echo ===== All core tests completed successfully =====

endlocal
