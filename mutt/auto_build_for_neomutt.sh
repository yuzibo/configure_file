#########################################################################
# File Name: auto_build_for_neomutt.sh
# Author: Bo Yu
# mail: tsu.yubo@gmail.com
# Created Time: 2018年10月05日 星期五 13时55分21秒
##########################################################################
#!/bin/bash

echo "创建 ~/.mutt目录"
mkdir ~/.mutt

echo "create account in .mutt: qq, 126, gmail\n"
echo "pleasee remember fill with password\n"

cp account/* ~/.mutt/*

echo "create alias for neomutt"
cp mutt.alias ~/.mutt.alias

echo "create (neo)muttrc\n"
cp muttrc/muttrc ~/.muttrc

echo "cp mailcap file to auto view html"
cp muttrc/mailcap ~/.mailcap


