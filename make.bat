@echo off 
@setlocal

pushd "Z:/FRC/Repositories/2016_Robot"

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