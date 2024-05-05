@echo OFF
set EXIT_ON_COMPLETE=0

:process_args
if "%1"=="" goto continue

if /i "%1"=="-exit_on_complete" (
	set EXIT_ON_COMPLETE=%2
	shift
)

shift
goto process_args

:continue

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

echo Starting project generation...
call CleanProjectFiles.bat
call "ThirdParty\Premake5\premake5.exe" vs2022

if %EXIT_ON_COMPLETE%==0 (
	pause
)