#!/bin/bash
`
groupadd oinstall;
groupadd dba;
groupadd oper;
useradd -m -s /bin/bash -g oinstall -G dba,oper oracle; #初始群组为 oinstall，有效群组为 dba、oper
passwd oracle; 
`
