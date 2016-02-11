@echo off 
@setlocal

pushd "C:/Users/yuxua_000/Desktop/2016Robot"

IF "%~1"=="-u" (
	CALL DeployEngine.bat
	goto endofmake
)

CALL Build.bat

IF "%~1"=="-d" (
	CALL Deploy.bat
	goto endofmake
)

:endofmake

popd