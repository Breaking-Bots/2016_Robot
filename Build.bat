@echo off
@setlocal

IF NOT EXIST bin mkdir bin
IF NOT EXIST temp mkdir temp
IF NOT EXIST temp\\Engine mkdir temp\\Engine
IF NOT EXIST temp\\HardwareLayer mkdir temp\\HardwareLayer

pushd temp\\HardwareLayer

set CommonCompilerFlags= -std=c++1y -fext-numeric-literals "-IC:/Users/yuxua_000/wpilib/cpp/current/include" "-I../../inc" -O0 -g3 -w

arm-frc-linux-gnueabi-g++ %CommonCompilerFlags% -c -fmessage-length=0 "../../src/HardwareLayer.cpp"

arm-frc-linux-gnueabi-g++  "-LC:/Users/yuxua_000/wpilib/cpp/current/lib" -Wl,-rpath,/opt/GenICam_v2_3/bin/Linux_armv7-a -o "../../bin/FRCUserProgram" -Wl,-Map,HardwareLayer.map "HardwareLayer.o" -lwpi

popd