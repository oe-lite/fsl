SUMMARY = "i.MX USB/Serial download tool"
DESCRIPTION = "This utility allows to download and execute code on Freescale i.MX5/i.MX6 and Vybrid SoCs through the Serial Download Protocol (SDP)."
LICENSE = "LGPL-2.1"

RECIPE_TYPES = "native sdk"

COMPATIBLE_HOST_ARCHS = ".*linux"

USBLOADER_COMMON_DEPENDS = "libc libusb-1.0"
DEPENDS = "${USBLOADER_COMMON_DEPENDS}"

inherit make c pkgconfig
EXTRA_OEMAKE += "sysconfdir=${sysconfdir} bindir=${bindir} prefix=${prefix}"

GITVER="349286e25c3fd9b2d31b31e962340123bbc62d44"
SRC_URI = "https://github.com/boundarydevices/imx_usb_loader/archive/${GITVER}.tar.gz"
S = "${SRCDIR}/imx_usb_loader-${GITVER}"
DEPENDS_${PN} += "${USBLOADER_COMMON_DEPENDS}"
RDEPENDS_${PN} += "${USBLOADER_COMMON_DEPENDS}"
PACKAGES =+ "${PN}-conf"
FILES_${PN}-conf += "${sysconfdir}"
RDEPENDS_${PN} += "${PN}-conf"
