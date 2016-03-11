@echo off

ssh lvuser@172.22.11.2 "rm /home/lvuser/FRCUserProgram || true; rm /home/lvuser/libCHRISTOPH.so || true;" 2>nul
sftp -oBatchMode=no -b sftpbatchfile lvuser@172.22.11.2 > nul 2>&1
ssh admin@172.22.11.2 "killall netconsole-host"
ssh lvuser@172.22.11.2 ". /etc/profile.d/natinst-path.sh; chmod -R 777 /home/lvuser; chmod a+x /home/lvuser/FRCUserProgram; /usr/local/frc/bin/frcKillRobot.sh -t -r"
echo Deployed All