require bootstream.inc

FILESPATHPKG .= ":barebox-bootstream"
SRC_URI += "file://barebox.bd"

DEPENDS += "barebox-elf"

BDFILE ?= "barebox.bd"
SBFILE ?= "barebox.sb"
DEPLOY_FILENAME ?= "${MACHINE}-barebox"

BDFILE_MANGLE_VARS = "S HOST_SYSROOT bootdir"
