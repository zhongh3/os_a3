#!/bin/sh
module="onebyte.ko"
device="onebyte"
mode="664"
major=61  # defined in onebyte.c
group="staff"  # /etc/group


# invoke insmod with all arguments we got
sudo insmod ./${module} $* || exit 1

# remove stale nodes
sudo rm -f /dev/${device}

sudo mknod /dev/${device} c $major 0

sudo chgrp $group /dev/${device}
sudo chmod $mode  /dev/${device}
