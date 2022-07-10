## Software download
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
LD_LIBRARY_PATH=. ./app/src
export LD_LIBRARY_PATH
```
## rstp test
```
gst-launch-1.0 rtspsrc location=rtsp://admin:htzm123456@112.44.218.103:354/0/0/0 latency=0 ! rtph265depay ! h265parse ! mppvideodec ! videoconvert ! autovideosink
gst-launch-1.0 rtspsrc location=rtsp://admin:htzm123456@171.221.205.72:14154 latency=0 ! rtph265depay ! h265parse ! mppvideodec ! videoconvert ! autovideosink
```
