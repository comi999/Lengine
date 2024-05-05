@echo off

:: We want to check if Visual Studio is open first and wait for user to close it.
set VS_PROCESS=devenv.exe
set PRINTED_CLOSE_VS_MESSAGE=0
:recheck_for_vs_open
tasklist | find /i "%VS_PROCESS%" > nul

if %errorlevel% equ 0 (
	if %PRINTED_CLOSE_VS_MESSAGE%==0 (
		echo Please close Visual Studio.
		set PRINTED_CLOSE_VS_MESSAGE=1
	)
	
	timeout /t 1 /nobreak > nul
	goto recheck_for_vs_open
)

del "Lengine.sln" > nul 2>&1

rmdir /s /q "Generated/Project" > nul 2>&1

rmdir /s /q ".vs" > nul 2>&1