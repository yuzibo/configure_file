#########################################################################
# File Name: ss.sh
# Author: Bo Yu
# mail: tsu.yubo@gmail.com
# Created Time: 2016年07月21日 星期四 17时00分45秒
##########################################################################
#!/bin/bash
sslocal -s ss-server-ip -p port_num -k passwd -t 600 -m aes-256-cfb
