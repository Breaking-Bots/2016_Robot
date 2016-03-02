@echo off

ssh lvuser@10.54.28.90 "rm /home/lvuser/FRCUserProgram || true; rm /home/lvuser/libCHRISTOPH.so || true;" 2>nul
sftp -oBatchMode=no -b sftpbatchfile lvuser@10.54.28.90 > nul 2>&1
ssh admin@10.54.28.90 "killall netconsole-host"
ssh lvuser@10.54.28.90 ". /etc/profile.d/natinst-path.sh; chmod -R 777 /home/lvuser; chmod a+x /home/lvuser/FRCUserProgram; /usr/local/frc/bin/frcKillRobot.sh -t -r"
echo Deployed All