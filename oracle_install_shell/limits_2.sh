#!/bin/bash
`
cp /etc/security/limits.conf /etc/security/limits.conf.original
echo "#Oracle 11gR2 shell limits:">>/etc/security/limits.conf
echo "oracle soft nproc 2048">>/etc/security/limits.conf
echo "oracle hard nproc 16384">>/etc/security/limits.conf
echo "oracle soft nofile 1024">>/etc/security/limits.conf
echo "oracle hard nofile 65536">>/etc/security/limits.conf
`
