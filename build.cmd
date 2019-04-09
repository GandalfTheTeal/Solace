@echo on
REM Build batch script
REM Copyright (C) 2018 Ethan Linquist, All rights reserved

REM Check parameters

@set SRC_DIR=%USERPROFILE%\Perforce\Solace\App\Solace
@REM @set SRC_DIR=%USERPROFILE%\Perforce\Build\App\Solace
@REM @set DEST_DIR=H:\FinalProjectBuilds
@set DEST_DIR=%USERPROFILE%\Perforce\Builds

REM Clean build and destination folders
rmdir %SRC_DIR%\Build /S /Q
mkdir %SRC_DIR%\Build

@REM Delete Oldest build
rmdir %DEST_DIR%\Oldest /S /Q

@REM Rename Old to oldest
REN %DEST_DIR%\Old "Oldest"
@REM Rename Current to Oldest
REN %DEST_DIR%\Current "Old"

REM Build it in the src folder
@c:
@cd \
@cd C:\Program Files\Epic Games\UE_4.20\Engine\Build\BatchFiles

call RunUAT BuildCookRun -project="%SRC_DIR%\Solace.uproject" -noP4 -platform=Win64 -utf8output -nocompile -targetplatform=Win64 -clientconfig=Shipping -serverconfig=Shipping -cook -allmaps -build -stage -pak -archive -archivedirectory="%SRC_DIR%\Build"

REM Clean destinantion folder, generate release

rmdir %DEST_DIR%\Current /S /Q
mkdir %DEST_DIR%\Current

REM Copy build folder to "Current" folder
xcopy %SRC_DIR%\Build %DEST_DIR%\Current /s /y

rmdir %DEST_DIR%\Current\WindowsNoEditor\FileOpenOrder /S /Q
DEL %DEST_DIR%\Current\WindowsNoEditor\Manifest_DebugFiles_Win64.txt
DEL %DEST_DIR%\Current\WindowsNoEditor\Manifest_NonUFSFiles_Win64.txt

@REM Create log file/notification

cd %DEST_DIR%\Current

echo ERRORS BELOWWW!. This was built(or not) %DATE% > ErrorLog.txt
findstr "error" "C:\Program Files\Epic Games\UE_4.20\Engine\Programs\AutomationTool\Saved\Logs\Log.txt" >> ErrorLog.txt
findstr "Error" "C:\Program Files\Epic Games\UE_4.20\Engine\Programs\AutomationTool\Saved\Logs\Log.txt" >> ErrorLog.txt
findstr "ERROR" "C:\Program Files\Epic Games\UE_4.20\Engine\Programs\AutomationTool\Saved\Logs\Log.txt" >> ErrorLog.txt

REM Remove build directory after use
rmdir %SRC_DIR%\Build /S /Q

c:
cd \
cd %USERPROFILE%\Perforce\Solace\App\

@set SRC_DIR = 
@set DEST_DIR =