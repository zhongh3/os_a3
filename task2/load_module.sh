#!/bin/sh
module="onebyte"
device="onebyte"
mode="664"
major=61

# invoke insmod with all arguments we got
# and use a pathname, as newer modutils don't look in . by default
/sbin/insmod ./${module}.ko $* || exit 1

# remove stale nodes
rm -f /dev/${device}

mknod /dev/${device} c $major 0

# give appropriate group/permissions, and change the group.
group="staff"

chgrp $group /dev/${device}
chmod $mode  /dev/${device}