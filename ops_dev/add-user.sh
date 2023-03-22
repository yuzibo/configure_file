#########################################################################
# File Name: add-user.sh
# Author: Bo Yu
# mail: tsu.yubo@gmail.com
# Created Time: Sat 12 Nov 2022 06:36:16 PM
##########################################################################
#!/bin/bash

set -e
u="vimer"

# fill with your ssh pub id
ssh_key="$(ssh-pub.id)"
uh=/home/$u
sshdir=$uh/.ssh
authfile=$sshdir/authorized_keys
echo "u = [[$u]], ssh_pub = [[$ssh_key]]"
date | md5sum | base64
#read -p 'is it looks good?'
[ -d $uh ] || adduser $u --disabled-password
mkdir $sshdir || true
chmod 700 $sshdir
echo ${ssh_key} >> $authfile
chmod 600 $authfile
chown -R $u.$u $sshdir
ls -la $sshdir
echo "Done for $u , $ssh_key"


