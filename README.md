## SOftware download
1. Flash the os [Tinker Edge R Debian Stretch V1.0.4 2020/07/07] to tinker edge r. [Tinker Edge R Debian Stretch V1.0.4] older than 2020/07/07 may not be possible transition to ums mode. I recommend download [Tinker Edge R Debian Stretch V1.0.4] again.

2. Start LXterminal on tinker edge r and connect  USB Type-C

3. run the following command on LXterminal

```
sudo reboot ums
```
4. If you are asked to format the drive on your computer, it is a success.

5. Start Etcher on PC and flash new os.

## Install packages
```
sudo apt-get install libgstreamer-plugins-base1.0-dev
sudo apt install libopencv-dev
sudo apt-get -y install gdb
```
## Add the following to .bashrc
```
LD_LIBRARY_PATH=.
export LD_LIBRARY_PATH
```
## rstp test
```
gst-launch-1.0 --gst-debug=rtspsrc:5 rtspsrc location=rtsp://admin:htzm123456@112.44.218.103:354/0/0/0 latency=0 ! rtph264depay ! h264parse ! mppvideodec ! videoconvert ! fpsdisplaysink

gst-launch-1.0 rtspsrc location=rtsp://admin:htzm123456@112.44.218.103:354/0/0/0 latency=0 ! rtph264depay ! h264parse ! mppvideodec ! videoconvert ! fpsdisplaysink

gst-launch-1.0 rtspsrc location=rtsp://admin:htzm123456@112.44.218.103:354/0/0/0 ! video/x-raw,format=NV12,width=640,height=480,framerate=30/1 ! videoconvert ! appsink
```
