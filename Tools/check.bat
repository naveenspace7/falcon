@echo off
pscp -r -pw raspberry E:\Falcon\Common E:\Falcon\Libraries E:\Falcon\Engines pi@192.168.0.143:/home/pi/Falcon/
pause