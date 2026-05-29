@echo off
REM oj_windows test runner - all tests with Rails
REM Uses MSVC Ruby at C:\RubyMSVC34

setlocal

cd /d "%~dp0"

REM Set MSVC Ruby path
set PATH=C:\RubyMSVC34\bin;%PATH%

echo Using Ruby: 
C:\RubyMSVC34\bin\ruby --version
echo.

echo ----- General tests (tests.rb) -----
C:\RubyMSVC34\bin\ruby tests.rb
if errorlevel 1 exit /b 1

echo ----- Various tests (test_various.rb) -----
C:\RubyMSVC34\bin\ruby test_various.rb
if errorlevel 1 exit /b 1

echo ----- Parser(:saj) tests (test_parser_saj.rb) -----
C:\RubyMSVC34\bin\ruby test_parser_saj.rb
if errorlevel 1 exit /b 1

echo ----- Parser(:usual) tests (test_parser_usual.rb) -----
C:\RubyMSVC34\bin\ruby test_parser_usual.rb
if errorlevel 1 exit /b 1

echo ----- Mimic tests (tests_mimic.rb) -----
C:\RubyMSVC34\bin\ruby tests_mimic.rb
if errorlevel 1 exit /b 1

echo ----- Mimic with additions tests (tests_mimic_addition.rb) -----
C:\RubyMSVC34\bin\ruby tests_mimic_addition.rb
if errorlevel 1 exit /b 1

echo ----- Oj.generate without calling mimic_JSON (test_generate.rb) -----
C:\RubyMSVC34\bin\ruby test_generate.rb
if errorlevel 1 exit /b 1

echo.
echo ===== All tests completed successfully =====

endlocal
