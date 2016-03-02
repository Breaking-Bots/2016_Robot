@echo off

ssh-keygen -t rsa
ssh admin@10.54.28.90 "mkdir ~/.ssh; chmod 777 ~/.ssh"
scp id_rsa.pub admin@10.54.28.90:~/.ssh/authorized-keys
PAUSE