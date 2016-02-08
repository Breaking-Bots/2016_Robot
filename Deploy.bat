@echo off

ssh lvuser@roboRIO-5428-FRC.local "rm /home/lvuser/FRCUserProgram || true; rm /home/lvuser/libEngine.so || true;"
sftp -oBatchMode=no sftpbatchfile lvuser@roboRIO-5428-FRC.local
ssh admin@roboRIO-5428-FRC.local "killall netconsole-host" 2> nul
ssh lvuser@roboRIO-5428-FRC.local ". /etc/profile.d/natinst-path.sh; chmod -R 777 /home/lvuser; chmod a+x /home/lvuser/FRCUserProgram; /usr/local/frc/bin/frcKillRobot.sh -t -r" 2> nul
