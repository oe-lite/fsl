#!/bin/bash 

# Call this script with something like:
# imx5x-mmc-prep.sh /dev/mmcblk0 \
#	tmp/images/imx53qsb-u-boot-imx.bin \
#	tmp/images/imx53qsb-linux-imx.bin \
#	tmp/images/imx53qsb-base-rootfs.tar.gz

DEVICE=$1
UBOOT=$2
KERNEL=$3
ROOTFS=$4

dev_umount() {
    for i in $( grep ${1} /proc/mounts | awk '{print $1}' );
    do 
        umount $i
    done
}

set -e -x

# sd-card device argument must be specified
[ -n "$DEVICE" ]
# and device must be writable
test -w $DEVICE

dev_umount $DEVICE

if [ -n "$UBOOT" ] ; then
dd if=$UBOOT of=$DEVICE bs=1k seek=1 skip=1
fi

if [ -n "$KERNEL" ] ; then
dd if=$KERNEL of=$DEVICE bs=1k seek=1024 skip=0
fi

if [ -n "$ROOTFS" ] ; then

dd if=/dev/zero of=$DEVICE bs=512 count=1
partprobe $DEVICE

sfdisk -uM $DEVICE <<EOF
8,16,83
EOF
sleep 3
dev_umount $DEVICE

# find rootfs partition
if [ -b "${DEVICE}p1" ]; then
	ROOTFS_DEVICE=${DEVICE}p1
elif [ -b "${DEVICE}1" ]; then
	ROOTFS_DEVICE=${DEVICE}1
else
	echo "Could not find device"
	exit 1;
fi

mkfs.ext4 -L rootfs $ROOTFS_DEVICE
ROOTFS_MNT=`mktemp -d`
mount -t ext4 $ROOTFS_DEVICE $ROOTFS_MNT
tar -xf $ROOTFS -C $ROOTFS_MNT
sleep 5
umount $ROOTFS_MNT
rm -rf $ROOTFS_MNT

fi

sync
