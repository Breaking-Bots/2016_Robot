@echo off

ssh-keygen -t rsa
ssh pi@10.54.28.13 "mkdir ~/.ssh; chmod 777 ~/.ssh"
scp id_rsav.pub pi@10.54.28.13:~/.ssh/authorized-keys
PAUSE