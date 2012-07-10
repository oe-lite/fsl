#!/bin/bash 
DEVICE="${1-${DEVICE-/dev/sdf}}"
[ -a "${DEVICE}" ] || exit 1
shift
PART_ROOTFS="${1-${PART_ROOTFS-/dev/sdf1}}"
shift
UBOOT="${1-${UBOOT-u-boot.bin}}"
[ -r "${UBOOT}" ] || exit 1
shift
KERNEL="${1-${KERNEL-uImage}}"
shift
ROOTFS="${1-${ROOTFS-rootfs.tar.gz}}"
shift

set -e -x

dev_umount() {
    for i in $( grep ${1} /proc/mounts | awk '{print $1}' );
    do 
        umount $i
    done
}

#######################
ls -l $DEVICE
sleep 5

dev_umount $DEVICE

dd if=$UBOOT of=$DEVICE bs=1k seek=1 skip=1

[ -r $KERNEL ]
dd if=$KERNEL of=$DEVICE bs=1k seek=1024 skip=0

[ -r $ROOTFS ]
dd if=/dev/zero of=$DEVICE bs=512 count=1
partprobe $DEVICE

sfdisk -uM $DEVICE <<EOF
8,16,83
EOF

sleep 3

dev_umount $DEVICE

mkfs.ext4 -L rootfs $PART_ROOTFS
mkdir -p /mnt/tmp-sdrootfs
mount -t ext4 $PART_ROOTFS /mnt/tmp-sdrootfs
tar -xf $ROOTFS -C /mnt/tmp-sdrootfs
sleep 5
umount /mnt/tmp-sdrootfs
rm -rf /mnt/tmp-sdrootfs
