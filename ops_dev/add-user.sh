#!/bin/bash

set -e

# pubkeys in txt like 'pubkey.debian-ci'
keys_prefix="pubkey"


for file in "$keys_prefix"*; do
    if [ -f "$file" ]; then
        echo "Found file: $file"
    fi
    [ -f "$file" ] || continue
    u="${file#pubkey.}"
    echo $u
    u=`echo "$u" | tr '[:upper:]' '[:lower:]'`
    uh=/home/$u
    sshdir=$uh/.ssh
    authfile=$sshdir/authorized_keys
    echo "u = [[$u]], keys = [[$file]]"
    date | md5sum | base64
    read -p 'is it looks good?'
    [ -d $uh ] || adduser $u --disabled-password
    mkdir $sshdir || true
    chmod 700 $sshdir
    cat $file >> $authfile
    chmod 600 $authfile
    chown -R $u:$u $sshdir
    ls -la $sshdir
    echo "Done for $u , $file"
    mv -vf "$file" "touched.$file"
    #echo "#$u" >> /etc/ssh/sshd_config
    read -p "ENTOR for next"
done

