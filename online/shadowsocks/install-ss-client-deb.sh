#########################################################################
# File Name: install-ss-client-deb.sh
# Author: Bo Yu
# mail: tsu.yubo@gmail.com
# Created Time: 2016年07月21日 星期四 19时53分38秒
##########################################################################
#!/bin/bash
sudo apt-get update && apt-get install python-pip python-setuptools m2crypto
&& pip install shadowsochs


