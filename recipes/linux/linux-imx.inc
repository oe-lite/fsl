DESCRIPTION = "Freescale iMX Linux 2.6 Kernel"
HOMEPAGE = "http://git.freescale.com/git/cgit.cgi/imx/linux-2.6-imx.git"
LICENSE = "GPL-2.0"

SRC_URI += "git://oe-lite.org/linux/linux-imx.git;protocol=git;${SRC_REV}"
SRC_REV ?= "tag=rel_imx_${@d.get('PV').replace('-','_')}"
S = "${SRCDIR}/linux-imx"
