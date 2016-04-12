#!/bin/bash
`echo 'Red Hat Linux release 6' | tee -a  /etc/redhat-release
ln -s /etc /etc/rc.d
ln -s /lib/i386-linux-gnu/libgcc_s.so.1 /lib/
ln -s /usr/bin/awk /bin/awk
ln -s /usr/bin/basename /bin/basename
ln -s /usr/bin/rpm /bin/rpm
ln -s /usr/lib/i386-linux-gnu/libc_nonshared.a /usr/lib/
ln -s /usr/lib/i386-linux-gnu/libpthread_nonshared.a /usr/lib/
ln -s /usr/lib/i386-linux-gnu/libstdc++.so.6 /lib/
ln -s /usr/lib/i386-linux-gnu/libstdc++.so.6 /usr/lib/
`

