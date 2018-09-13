#########################################################################
# File Name: autobuildvimrc.sh
# Author: Bo Yu
# mail: tsu.yubo@gmail.com
# Created Time: Thu 13 Sep 2018 08:59:23 AM EDT
##########################################################################
#!/bin/bash

# Until now, we just move it according to command line, and we should
# know the return value from the execution

mkdir -p ~/.vim/colors

cp colors/eldar.vim ~/.vim/colors && cp vimrcforlinux ~/.vimrc
