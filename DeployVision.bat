@echo off

ssh pi@10.54.28.13 "rm /home/pi/vision/start_vision.sh || true;" 2>nul
sftp -oBatchMode=no -b sftpVision pi@10.54.28.13 > nul 2>&1
ssh pi@10.54.28.13 "sudo killall mjpg_streamer;sudo chmod +x /home/pi/vision/start_vision.sh; /home/pi/vision/start_vision.sh"