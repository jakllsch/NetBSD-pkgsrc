# $NetBSD: options.mk,v 1.3 2018/05/30 06:45:58 adam Exp $

PKG_OPTIONS_VAR=	PKG_OPTIONS.fftw
PKG_SUPPORTED_OPTIONS=	fftw-fortran
# Enable fortran support by default on platforms supported by lang/g95.
.if (!empty(MACHINE_ARCH:Mi386) || !empty(MACHINE_ARCH:Mx86_64) || \
	!empty(MACHINE_ARCH:Mia64) || !empty(MACHINE_ARCH:Mpowerpc*) || \
	!empty(MACHINE_ARCH:Mhppa) || !empty(MACHINE_ARCH:Msparc*) || \
	!empty(MACHINE_ARCH:Malpha) || !empty(MACHINE_ARCH:Mmips*))
# ...but disable it until lang/g95 issue is resolved.
#PKG_SUGGESTED_OPTIONS=	fftw-fortran
.endif

.include "../../mk/bsd.options.mk"

.if !empty(PKG_OPTIONS:Mfftw-fortran)
USE_LANGUAGES+=		fortran77
.else
CONFIGURE_ARGS+=	--disable-fortran
.endif
