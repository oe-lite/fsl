inherit imx-bootlets

RECIPE_FLAGS += "imxbootlets_version"
DEFAULT_USE_imxbootlets_version = "10.12.01"

FILESPATHPKG = "imx-bootlets-${USE_imxbootlets_version}:imx-bootlets"

SRC_URI += "${SRC_URI_IMX_BOOTLETS}"
SRC_URI_IMX_BOOTLETS = "http://oe-lite.org/mirror/imx/imx-bootlets-src-${USE_imxbootlets_version}.tar.gz"
S = "${SRCDIR}/imx-bootlets-src-${USE_imxbootlets_version}"

RECIPE_FLAGS += "imxbootlets_board"
#COMPATIBLE_IF_FLAGS += "imxbootlets_board"
BOARD ?= "${USE_imxbootlets_board}"

RECIPE_FLAGS += "imxbootlets_chipfamily"
#COMPATIBLE_IF_FLAGS += "imxbootlets_chipfamily"
ELFTOSB_CHIP_FAMILY ?= "${USE_imxbootlets_chipfamily}"

BDFILE_MANGLE_VARS = "S HOST_SYSROOT"
do_patch[postfuncs] += "do_patch_bdfile_mangle"
def do_patch_bdfile_mangle(d):
    bdfilename = os.path.join(d.get("SRCDIR"), d.get("BDFILE"))
    with open(bdfilename) as bdfile:
        bd = bdfile.read()
    for var in d.get("BDFILE_MANGLE_VARS").split():
        bd = bd.replace("${%s}"%(var), d.get(var))
    with open(bdfilename, "w") as bdfile:
        bdfile.write(bd)

do_compile[postfuncs] += "do_compile_bootstream"
do_compile_bootstream () {
	elftosb -z -f ${ELFTOSB_CHIP_FAMILY} -c ${SRCDIR}/${BDFILE} \
		-o ${B}/${SBFILE}
}

do_install () {
	install -d 755 ${D}${bootdir}
	install -m 644 ${B}/${SBFILE} ${D}${bootdir}/
}

addtask deploy after install before build
do_deploy[dirs] = "${IMAGE_DEPLOY_DIR}"
do_deploy() {
	install -m 644 ${D}${bootdir}/${SBFILE} \
		${IMAGE_DEPLOY_DIR}/${DEPLOY_FILENAME}-${DATETIME}.sb
	ln -sf ${DEPLOY_FILENAME}-${DATETIME}.sb \
		${IMAGE_DEPLOY_DIR}/${DEPLOY_FILENAME}.sb
}

PACKAGES = "${PN}"
FILES_${PN} = "${bootdir}/${SBFILE}"
