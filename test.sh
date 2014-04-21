#!/bin/bash

set -x
insmod lab5fs_mod.ko
mount -o loop -t lab5fs image /mnt/
cd /mnt
touch x
touch y
touch z
ls
rm x
rm y
touch a
ls
rm *
ls
cd
umount /mnt/
rmmod lab5fs_mod

