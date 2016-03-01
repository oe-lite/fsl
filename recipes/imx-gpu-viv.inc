# Copyright (C) 2012-2013 Freescale Semiconductor
# Copyright (C) 2012-2014 O.S. Systems Software LTDA.
# Released under the MIT license (see COPYING.MIT for the terms)

inherit fsl-eula-unpack 

ACCEPT_FSL_EULA = "1"

DESCRIPTION = "GPU driver and apps for imx6"

DEPENDS += "libx11 libxdamage libxext libxfixes mesa-dri-libgl-dev"  
DEPENDS_${PN} += "libx11 libxdamage libxext libxfixes" 
RDEPENDS_${PN} += "libx11 libxdamage libxext libxfixes mesa-dri-libglapi"

SRC_URI = "${FSL_MIRROR}/${PN}-${PV}.bin;fsl-eula=true;unpack=fsl \
           file://egl.pc \
           file://egl_x11.pc \
           file://glesv1_cm.pc \
           file://glesv1_cm_x11.pc \
           file://glesv2.pc \
           file://glesv2_x11.pc \
           file://vg.pc \
           file://vg_x11.pc \
           file://gc_wayland_protocol.pc \
           file://wayland-egl.pc \
           file://wayland-viv.pc \
           file://directfbrc \
           file://glproto.pc \
           file://gl.pc \
           file://dri.pc"

PACKAGES =+ "libclc libclc-dev libclc-dbg \
	libgl libgl-dev libgl-dbg \
	libglesv1 libglesv1-dev libglesv1-dbg \
	libglesv2 libglesv2-dev libglesv2-dbg \
	libglslc libglslc-dev libglslc-dbg \
	libopencl libopencl-dev libopencl-dbg \
	libopenvg libopenvg-dev libopenvg-dbg \
	libvdk libvdk-dev libvdk-dbg \
	libegl libegl-dev libegl-dbg \
	libgal libgal-dev libgal-dbg \
	libvivante libvivante-dev libvivante-dbg \
	libvivante-dri 	libvivante-dri-dbg \
	libvivante-dfb \
	libwayland-viv libwayland-viv-dev libwayland-viv-dbg \
	libgc-wayland-protocol libgc-wayland-protocol-dev libgc-wayland-protocol-dbg \
	libwayland-egl-dev libvsc libvsc-dbg \
"

# FIXME: The provided binary doesn't provide soname. If in future BSP
  # release the libraries are fixed, we can drop this hack.
REALSOLIBS = "${SOLIBS}"
SOLIBS = "${SOLIBSDEV}"
ALLSOLIBS = ".so*"

do_install () {
    install -d ${D}${libdir}
    install -d ${D}${includedir}
    install -d ${D}${bindir}

    cp -P ${S}/gpu-core/usr/lib/*.so* ${D}${libdir}
    rm -f ${D}${libdir}*wl*
    #rm -f ${D}${libdir}/libGL.so*

    install -d ${D}${libdir}/dri
    cp -axr ${MACHINE_SYSROOT}/usr/lib/dri/* ${D}${libdir}/dri/

    #cp  ${S}/g2d/usr/lib/*.so* ${D}${libdir}
    cp -r ${S}/gpu-core/usr/include/* ${D}${includedir}
    cp -Pr ${S}/g2d/usr/include/* ${D}${includedir}
    cp -r ${S}/gpu-demos/opt ${D}
    cp -r ${S}/gpu-tools/gmem-info/usr/bin/* ${D}${bindir}

    #we need GL headers from mesa in stage
    cp -axr ${HOST_SYSROOT}/usr/include/GL ${D}${includedir}/.

    install -d ${D}${libdir}/pkgconfig

    #install gl.pc and glproto.pc
    install -m 0644 ${SRCDIR}/gl.pc ${D}${libdir}/pkgconfig/gl.pc
    install -m 0644 ${SRCDIR}/glproto.pc ${D}${libdir}/pkgconfig/glproto.pc
    install -m 0644 ${SRCDIR}/dri.pc ${D}${libdir}/pkgconfig/dri.pc

    # The preference order, based in DISTRO_FEATURES, is x11, wayland, directfb and fb
        cp -r ${S}/gpu-core/usr/lib/dri ${D}${libdir}
        #cp -r ${S}/apitrace/x11/usr/lib/* ${D}${libdir}
        #cp -r ${S}/apitrace/x11/usr/bin/* ${D}${bindir}

        #backend=x11

        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/egl_x11.pc ${D}${libdir}/pkgconfig/egl.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/glesv1_cm_x11.pc ${D}${libdir}/pkgconfig/glesv1_cm.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/glesv2_x11.pc ${D}${libdir}/pkgconfig/glesv2.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/vg_x11.pc ${D}${libdir}/pkgconfig/vg.pc

    # Install Vendor ICDs for OpenCL's installable client driver loader (ICDs Loader)
    install -d ${D}${sysconfdir}/OpenCL/vendors/
    install -m 0644 ${S}/gpu-core/etc/Vivante.icd ${D}${sysconfdir}/OpenCL/vendors/Vivante.icd

    # We'll only have one backend here so we rename it to generic name
    # and avoid rework in other packages, when possible
    #mv ${D}${libdir}/libGL.so.1.2 ${D}${libdir}/libGL.so.1.2.0
    #ln -sf libGL.so.1.2.0 ${D}${libdir}/libGL.so.1.2
    #ln -sf libGL.so.1.2.0 ${D}${libdir}/libGL.so.1
    #ln -sf libGL.so.1.2.0 ${D}${libdir}/libGL.so
    mv ${D}${libdir}/libEGL-x11.so ${D}${libdir}/libEGL.so.1.0
    ln -sf libEGL.so.1.0 ${D}${libdir}/libEGL.so.1
    ln -sf libEGL.so.1.0 ${D}${libdir}/libEGL.so
    mv ${D}${libdir}/libGAL-x11.so ${D}${libdir}/libGAL.so
    mv ${D}${libdir}/libVIVANTE-x11.so ${D}${libdir}/libVIVANTE.so

    # update libglesv2 as backend dependent
    rm -rf ${D}${libdir}/libGLESv2*
    cp ${S}/gpu-core/usr/lib/libGLESv2-x11.so ${D}${libdir}/libGLESv2.so.2.0.0
    ln -sf libGLESv2.so.2.0.0 ${D}${libdir}/libGLESv2.so.2
    ln -sf libGLESv2.so.2.0.0 ${D}${libdir}/libGLESv2.so

    # skip packaging wayland libraries if no support is requested
        rm ${D}${libdir}/libgc_wayland_protocol.*
        rm ${D}${libdir}/libwayland-viv.*
        rm ${D}${libdir}/*dfb.so
        rm ${D}${libdir}/*wl.so
	rm ${D}${libdir}/*fb.so

    #for backend in wl x11 fb dfb dri; do
    #    find ${D}${libdir} -name "*-$backend.so" -exec rm '{}' ';'
    #    find ${D}${libdir} -name "*.$backend.so" -exec rm '{}' ';'
    #done

    find ${D}${libdir} -type f -exec chmod 644 {} \;
    find ${D}${includedir} -type f -exec chmod 644 {} \;
}

FILES_${PN} += "/opt"
FILES_${PN}-dev = "${includedir}"
FILES_${PN}-dbg = "${bindir}/.debug ${libdir}/.debug /opt/viv_samples/*/*/.debug"

FILES_libclc = "${libdir}/libCLC${SOLIBS}"
FILES_libclc-dev = "${includedir}/CL ${libdir}/libCLC${SOLIBSDEV}"
FILES_libclc-dbg = "${libdir}/.debug/libCLC${SOLIBS}"

# libEGL.so is used by some demo apps from Freescale
FILES_libegl = "${libdir}/libEGL${REALSOLIBS} ${libdir}/libEGL${SOLIBSDEV} ${libdir}/libEGL${ALLSOLIBS}"
FILES_libegl-dev = "${includedir}/EGL ${includedir}/KHR ${libdir}/pkgconfig/egl.pc"
FILES_libegl-dbg = "${libdir}/.debug/libEGL${SOLIBS}"

FILES_libgal = "${libdir}/libGAL${SOLIBS}"
FILES_libgal-dev = "${libdir}/libGAL${SOLIBSDEV}"
FILES_libgal-dbg = "${libdir}/.debug/libGAL${SOLIBS}"

FILES_libgl = "${libdir}/libGL${REALSOLIBS} ${libdir}/libGL${SOLIBS} ${libdir}/libGL${ALLSOLIBS}"
FILES_libgl-dev = "${includedir}/GL ${libdir}/libGL${SOLIBSDEV} ${libdir}/pkgconfig/glproto.pc ${libdir}/pkgconfig/gl.pc"
FILES_libgl-dbg = "${libdir}/.debug/libGL.${SOLIBS}"
DEPENDS_libgl += "libgl-dev libgal libdrm libvivante-dri"
RDEPENDS_libgl += "libgal libdrm libvivante-dri"

# libEGL needs to open libGLESv1.so
FILES_libglesv1 = "${libdir}/libGLESv1*${REALSOLIBS} ${libdir}/libGLESv1*${SOLIBS} ${libdir}/libGLES_*${SOLIBS} ${libdir}/libGLES_*${ALLSOLIBS} ${libdir}/libGLESv1_*${ALLSOLIBS}"
FILES_libglesv1-dev = "${includedir}/GLES ${libdir}/libGLESv1*${SOLIBS} ${libdir}/libGLES_*${SOLIBSDEV} ${libdir}/pkgconfig/glesv1_cm.pc"
FILES_libglesv1-dbg = "${libdir}/.debug/libGLESv1*${SOLIBS} ${libdir}/.debug/libGLES_*${SOLIBS}"
DEPENDS_libglesv1 += "libglesv1-dev libgal"

# libEGL needs to open libGLESv2.so
FILES_libglesv2 = "${libdir}/libGLESv2${REALSOLIBS} ${libdir}/libGLESv2${SOLIBS} ${libdir}/libGLESv2${ALLSOLIBS}"
FILES_libglesv2-dev = "${includedir}/GLES2 ${libdir}/libGLESv2${SOLIBSDEV} ${libdir}/pkgconfig/glesv2.pc"
FILES_libglesv2-dbg = "${libdir}/.debug/libGLESv2${SOLIBS}"
RDEPENDS_libglesv2 = "libglslc libvsc libgal"
DEPENDS_libglesv2 += "libglslc libglslc-dev libgal libvsc"

FILES_libglslc = "${libdir}/libGLSLC${SOLIBS}"
FILES_libglslc-dev = "${includedir}/CL ${libdir}/libGLSLC${SOLIBSDEV}"
FILES_libglslc-dbg = "${libdir}/.debug/libGLSLC${SOLIBS}"

FILES_libopencl = "${libdir}/libOpenCL${SOLIBS}"
FILES_libopencl-dev = "${includedir}/CL ${libdir}/libOpenCL${SOLIBSDEV}"
FILES_libopencl-dbg = "${libdir}/.debug/libOpenCL${SOLIBS}"
RDEPENDS_libopencl = "libclc"

FILES_libvsc = "${libdir}/libVSC${SOLIBS}"
FILES_libvsc-dbg = "${libdir}/.debug/libVSC${SOLIBS}"

FILES_libopenvg = "${libdir}/libOpenVG*${SOLIBS}"
FILES_libopenvg-dev = "${includedir}/VG ${libdir}/libOpenVG*${SOLIBSDEV} ${libdir}/pkgconfig/vg.pc"
FILES_libopenvg-dbg = "${libdir}/.debug/libOpenVG*${SOLIBS}"

FILES_libvdk = "${libdir}/libVDK${SOLIBS}"
FILES_libvdk-dev = "${includedir}/*vdk.h ${libdir}/libVDK${SOLIBSDEV}"
FILES_libvdk-dbg = "${libdir}/.debug/libVDK${SOLIBS}"

FILES_libvivante = "${libdir}/libVIVANTE${SOLIBS}"
FILES_libvivante-dev = "${libdir}/libVIVANTE${SOLIBSDEV}"
FILES_libvivante-dbg = "${libdir}/.debug/libVIVANTE${SOLIBS}"

FILES_libvivante-dri = "${libdir}/dri/*.so ${libdir}/pkgconfig/dri.pc"
FILES_libvivante-dri-dbg = "${libdir}/dri/.debug/vivante_dri.so"

FILES_libvivante-dfb = "${libdir}/bVIVANTE-dfb.so"

FILES_libwayland-viv =  "${libdir}/libwayland-viv${REALSOLIBS}  ${libdir}/libwayland-viv${SOLIBS} ${libdir}/libwayland-viv${ALLSOLIBS}"
FILES_libwayland-viv-dev = "${libdir})/libwayland-viv${SOLIBSDEV} ${libdir}/pkgconfig/wayland-viv.pc"
FILES_libwayland-viv-dbg = "${libdir}/.debug/libwayland-viv${SOLIBS}"

FILES_libgc-wayland-protocol = "${libdir}/libgc_wayland_protocol${REALSOLIBS} ${libdir}/libgc_wayland_protocol${SOLIBS} ${libdir}/libgc_wayland_protocol${ALLSOLIBS}"
FILES_libgc-wayland-protocol-dev = "${libdir}/libgc_wayland_protocol${SOLIBSDEV} ${libdir}/pkgconfig/gc_wayland_protocol.pc"
FILES_libgc-wayland-protocol-dbg = "${libdir}/libgc_wayland_protocol${SOLIBS}"

FILES_libwayland-egl-dev = "${libdir}/pkgconfig/wayland-egl.pc"

FILES_libvivante-dfb += "${sysconfdir}/directfbrc"

COMPATIBLE_MACHINES = ".*"
RECIPE_FLAGS = "mx6"
COMPATIBLE_IF_FLAGS = "mx6"

