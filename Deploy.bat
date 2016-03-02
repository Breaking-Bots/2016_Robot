@echo off

ssh lvuser@roboRIO-5428-FRC "rm /home/lvuser/FRCUserProgram || true; rm /home/lvuser/libCHRISTOPH.so || true;" 2>nul
sftp -oBatchMode=no -b sftpbatchfile lvuser@roboRIO-5428-FRC > nul 2>&1
ssh admin@roboRIO-5428-FRC "killall netconsole-host"
ssh lvuser@roboRIO-5428-FRC ". /etc/profile.d/natinst-path.sh; chmod -R 777 /home/lvuser; chmod a+x /home/lvuser/FRCUserProgram; /usr/local/frc/bin/frcKillRobot.sh -t -r"
echo Deployed All