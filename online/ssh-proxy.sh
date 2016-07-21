#########################################################################
# File Name: ssh-proxy.sh
# Author: Bo Yu
# mail: tsu.yubo@gmail.com
# Created Time: 2016年07月21日 星期四 18时48分15秒
#
#   When  use the shell script,you must modify _IP_
#
##########################################################################
#!/bin/bash

ssh -N -D 7070 username@your_ip
