set NCadSDK=C:\NanoCadSdk
set NCadExe=C:\Program Files\Nanosoft\nanoCAD x64 22.0\nCad.exe
set VSVER=16
set VSTOOLSVER=142

@if     "%ProgramFiles(x86)%" == "" set UniProgramFiles=%ProgramFiles%\
@if not "%ProgramFiles(x86)%" == "" set UniProgramFiles=%ProgramFiles(x86)%\

call :GetVSCommonToolsDir

call "%VSCOMNTOOLS%VC\Auxiliary\Build\vcvars32.bat"

devenv HelloNRX.vcxproj

goto :EOF

:GetVSCommonToolsDir
@set VSCOMNTOOLS=
@set vswhere="%UniProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
@for /f "usebackq delims=*" %%i in (`%vswhere% -version %VSVER% -property installationPath`) do (
  @if exist "%%i\Common7\Tools\vsdevcmd.bat" (
	@SET VSCOMNTOOLS=%%i\
	@exit /b 0
  )
)
@exit /B 1 