@echo off

IF NOT EXIST bin mkdir bin
IF NOT EXIST temp mkdir temp
IF NOT EXIST temp\\CHRISTOPHEngine mkdir temp\\CHRISTOPHEngine

pushd temp\\CHRISTOPHEngine

set CommonCompilerFlags= -std=c++1y -fPIC -fext-numeric-literals "-IC:/Users/%USERNAME%/wpilib/cpp/current/include" "-I../../inc" -O0 -g3 -w

arm-frc-linux-gnueabi-g++ -shared %CommonCompilerFlags% -fmessage-length=0 -o ..\\..\\bin\\libCHRISTOPH.so -Wl,-Map,libCHRISTOPH.map "../../src/CHRISTOPHEngine.cpp" 

popd

ssh lvuser@172.22.11.2 "rm /home/lvuser/libCHRISTOPH.so || true;" 2>nul
sftp -oBatchMode=no -b sftpCHRISTOPHEngine lvuser@172.22.11.2 > nul 2>&1
echo Deployed CHRISTOPH Engine
