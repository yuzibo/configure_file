#########################################################################
# File Name: autodev.sh
# Author: Bo Yu
# mail: tsu.yubo@gmail.com
# Created Time: 2019年03月10日 星期日 21时07分11秒
##########################################################################
#!/bin/bash

if [ -f "~/.config/terminator/config" ]; then
	echo "terminator config file has been existed!"
else
	mkdir -p ~/.config/terminator/ && cp config ~/.config/terminator/
	cp protect-eys.jpg ~/.config/terminator
fi
