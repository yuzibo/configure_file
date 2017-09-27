sudo mknod ram0 b 1 0  # all one needs is ram0
sudo mknod ram1 b 1 1
ln -s ram0 ramdisk
sudo mknod initrd b 1 250
sudo mknod mem c 1 1
sudo mknod kmem c 1 2
sudo mknod null c 1 3
sudo mknod port c 1 4
sudo mknod zero c 1 5
sudo mknod core c 1 6
sudo mknod full c 1 7
sudo mknod random c 1 8
sudo mknod urandom c 1 9
sudo mknod aio c 1 10
sudo mknod kmsg c 1 11
sudo mknod sda b 8 0
sudo mknod tty0 c 4 0
sudo mknod ttyS0 c 4 64
sudo mknod ttyS1 c 4 65
sudo mknod tty c 5 0
sudo mknod console c 5 1
sudo mknod ptmx c 5 2
sudo mknod ttyprintk c 5 3ln -s ../proc/self/fd fd
ln -s ../proc/self/fd/0 stdin # process i/o
ln -s ../proc/self/fd/1 stdout
ln -s ../proc/self/fd/2 stderr
ln -s ../proc/kcore     kcore