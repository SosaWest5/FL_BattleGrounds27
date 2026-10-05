@echo off
setlocal
cd /d "%~dp0"
if not exist "FL_BattleGrounds27.vst3" (
 echo Missing compiled plugin. Download the Windows GitHub Actions artifact, not the source ZIP.
 pause
 exit /b 1
)
set "battleFolder=%CommonProgramFiles%\VST3"
if not exist "%battleFolder%" mkdir "%battleFolder%"
xcopy "FL_BattleGrounds27.vst3" "%battleFolder%\FL_BattleGrounds27.vst3\" /E /I /Y >nul
if errorlevel 1 (
 echo Copy failed. Right-click this installer and choose Run as administrator.
 pause
 exit /b 1
)
echo Installed FL_BattleGrounds27 to %battleFolder%
echo In FL Studio: Options - Manage plugins - Verify plugins - Find installed plugins.
echo Load FL_BattleGrounds27 from an empty Mixer effect slot.
pause
