SRC_URI = "git://oe-lite.org/u-boot/u-boot-imx.git;protocol=git;${SRC_REV}"
SRC_REV ?= "tag=rel_imx_${@d.get('PV').replace('-','_')}"
S = "${SRCDIR}/u-boot-imx"
