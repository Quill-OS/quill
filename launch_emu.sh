#!/bin/bash
# Install this using cargo, or use `busybox httpd -f -p 8000`
killall simple-http-server;
~/.cargo/bin/simple-http-server &
sleep 1;

( sleep 0.5; echo "ifsctl mnt rootfs rw"; sleep 0.4 )  | telnet 127.0.0.1 5555 2>/dev/null 1>/dev/null
( sleep 0.5; echo "rm /kobo/tmp/exec"; sleep 40 ) | telnet 127.0.0.1 5555 2>/dev/null 1>/dev/null
( sleep 0.5; echo "wget 192.168.88.22:8000/exec -O /kobo/tmp/exec;"; sleep 15 )  | telnet 127.0.0.1 5555 2>/dev/null 1>/dev/null # Increase sleep time if it doesn't manage to download the whole binary
( sleep 0.5; echo "chmod +x /kobo/tmp/exec"; sleep 0.5 )  | telnet 127.0.0.1 5555 2>/dev/null 1>/dev/null
killall simple-http-server;
( sleep 0.5; echo "/kobo/launch_app.sh"; sleep infinity ) | telnet 127.0.0.1 5555 2>/dev/null
# Look: https://github.com/Szybet/kobo-nia-audio/blob/main/apps-on-kobo/launch_app.sh
# But change QT_QPA_PLATFORM to QT_QPA_PLATFORM=vnc:size=768x1024
