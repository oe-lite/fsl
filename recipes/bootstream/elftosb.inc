DESCRIPTION = "Elftosb freescale tool for bootstream creation"
HOMEPAGE = "http://www.freescale.com/imxtools"
LICENSE = "BSD"

RECIPE_TYPES = "native sdk"

SRC_URI = "http://oe-lite.org/mirror/imx/${PN}-${PV}.tar.gz"

inherit c make

do_install () {
    install -d ${D}/${bindir}
    install -m 0755 bld/linux/elftosb ${D}/${bindir}/
    install -m 0755 bld/linux/keygen ${D}/${bindir}/
    install -m 0755 bld/linux/sbtool ${D}/${bindir}/
}
