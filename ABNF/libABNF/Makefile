# 
#
DEV			=	/opt/INET-Products/share/etc
TARG_BINARY		=	ABNF
TOP			=	..
PREFIX			=	lib$(TARG_BINARY).so
ARCHIVENAME		=	lib$(TARG_BINARY).a
TARFILE			=	$(PREFIX).tar
FULLNAME		=	lib$(TARG_BINARY).so.$(FULLVERSION)
LIBNAME			=	$(FULLNAME)
NAME			=	$(LIBNAME)

include $(DEV)/makefile.top.gcc
VERSION_MINOR		=	1
VERSION_MAJOR		=	0
# NOTE: UPDATE RPMRELEASE and CHANGE TO '1' when RPMVERSION is UPDATED!
RPMRELEASE		=	1

LN			=	cp

SOURCES.cpp		=	\
				ABNF.cpp \
				Comment.cpp \
				Object.cpp \
				Parser.cpp \
				Rule.cpp \
				RuleName.cpp \
				Terminal.cpp

SOURCES.hpp		= \
				ABNF.hpp \
				Comment.hpp \
				Common.hpp \
				Object.hpp \
				Parser.hpp \
				Rule.hpp \
				RuleName.hpp \
				Terminal.hpp


SOURCES.c		= 

#SOURCES.msg		=	lib$(TARG_BINARY).msg

SOURCES.mk		=	makefile.top.gcc makefile.end

RPMVERSION		=	$(VERSION_MAJOR).$(VERSION_MINOR)

RPM_SUMMARY		=	ABNF libaries.
RPM_DESCRIPTION		=	$(RPM_SUMMARY)

RPMDEV_SUMMARY		=	ABNF Dev
RPMDEV_DESCRIPTION	=	$(RPMDEV_SUMMARY)

RPMLIBFILES		=	\
				$(PROTO)/$(PROTOSUBDIR)/lib/$(FULLNAME) \
				$(PROTO)/$(PROTOSUBDIR)/lib/$(PREFIX) \
				$(PROTO)/$(PROTOSUBDIR)/lib/libcrypto.a \
				$(PROTO)/$(PROTOSUBDIR)/lib/libneon.so.24 \
				$(PROTO)/$(PROTOSUBDIR)/lib/libneon.so.24.0.4 \
				$(PROTO)/$(PROTOSUBDIR)/lib/libssl.a \
				$(PROTO)/$(PROTOSUBDIR)/lib/libxmlsec.so \
				$(PROTO)/$(PROTOSUBDIR)/lib/libxmlsec.so.0 \
				$(PROTO)/$(PROTOSUBDIR)/lib/libxmlsec.so.0.0.14 \
				$(PROTO)/share/locale/C/lib$(TARG_BINARY).cat


RPMDIRS			=	\
				$(PROTO) \
				$(PROTO)/share \
				$(PROTO)/share/certs \
				$(PROTO)/share/doc \
				$(PROTO)/share/etc \
				$(PROTO)/share/internet-drafts \
				$(PROTO)/share/locale \
				$(PROTO)/share/locale/C \
				$(PROTO)/share/man \
				$(PROTO)/share/man/man1 \
				$(PROTO)/share/man/man1I \
				$(PROTO)/share/man/man2I \
				$(PROTO)/share/man/man3 \
				$(PROTO)/share/man/man3I \
				$(PROTO)/share/man/man4 \
				$(PROTO)/share/man/man4I \
				$(PROTO)/share/man/man5 \
				$(PROTO)/share/man/man5I \
				$(PROTO)/share/man/man6 \
				$(PROTO)/share/man/man6I \
				$(PROTO)/share/man/man7 \
				$(PROTO)/share/man/man7I \
				$(PROTO)/share/man/man8 \
				$(PROTO)/share/man/man8I \
				$(PROTO)/share/man/man9 \
				$(PROTO)/share/man/man9I \
				$(PROTO)/share/man/pdf \
				$(PROTO)/share/man/rfc \
				$(PROTO)/share/man/ps \
				$(PROTO)/share/man/txt \
				$(PROTO)/share/man/html \
				$(PROTO)/share/src \
				$(PROTO)/$(PROTOSUBDIR) \
				$(PROTO)/$(PROTOSUBDIR)/lib \
				$(PROTO)/$(PROTOSUBDIR)/bin \
				$(PROTO)/$(PROTOSUBDIR)/info \
				/var$(PROTO) \
				/var$(PROTO)/cache


RPM777FILES		=	/var$(PROTO)/cache/config
RPM777DIRS		=	/var$(PROTO)/tmp
RPMTOUCH		=	/var$(PROTO)/cache/config

RPMDEVFILES		:=	$(shell echo $(SOURCES.h) $(SOURCES.hpp) | sed 's/license.hpp//' )

RPMDEVFILES2		= \
				$(PROTO)/share/etc/makefile.top.gcc \
				$(PROTO)/share/etc/makefile.end \
				$(PROTO)/$(PROTOSUBIDR)share/include/openssl/*

RPMDEVDIRS		=	\
				$(PROTO)/share/rpm \
				$(PROTO)/share/rpm \
				$(PROTO)/share/rpm/BUILD \
				$(PROTO)/share/rpm/RPMS \
				$(PROTO)/share/rpm/SOURCES \
				$(PROTO)/share/rpm/SPEC \
				$(PROTO)/share/rpm/tmp \
				$(PROTO)/share/include \
				$(PROTO)/share/include/SaS \
				$(PROTO)/share/include/SaS/IETF \
				$(PROTO)/share/include/SaS/IETF/ABNF \
				$(PROTO)/$(PROTOSUBIDR)share/include/openssl \
				$(PROTO)/$(PROTOSUBIDR)share/include/neon


CATFILES		=	$(SOURCES.msg:.msg=.cat)

SCRIPTS.sh		=

SCRIPTS			=	$(SCRIPTS.sh:.sh=)

SOURCES.all		=	$(SOURCES.cpp) \
				$(SOURCES.c) \
				$(SOURCES.h) \
				$(SOURCES.hpp) \
				$(SOURCES.msg) \
				$(SCRIPTS.sh) \
				$(SCRIPTS.bat) \
				Makefile

INCLUDES		=	$(SOURCES.hpp) $(SOURCES.h)

CPPFLAGS		+=	-I. \
				-DCOMMON_LICENSE \
				-DBUILDING_LIBARAY \
				-DBUILDING_LIBARAY_COMMON


ifeq ($(OS), SunOS)
SOCKET			=	-lsocket
endif
MYLIBS			+=	\
				-lcrypto \
				-lssl \
				-liconv \
				$(SOCKET)

MYLIBS			+=
ifeq ($(OS),CYGWIN)
MYLIBS			+=	-lnsl $(ICONVLIB)
endif
ifeq ($(OS),Linux)
MYLIBS			+=	$(ICONVLIB)
endif

EVERYTHING		=	$(SUBDIR) \
				protoDirs \
				$(SUBDIR)/$(CATFILES) \
				$(PROTO)/share/locale/C/$(CATFILES) \
				$(PROTOINCFILES) .WAIT archive $(SUBDIR)/$(LIBNAME) \
				$(PROTO)/$(PROTOSUBDIR)/lib/$(LIBNAME) \
				/var$(PROTO)/cache \
				/var$(PROTO)/cache/config \
				$(SOURCES.all)

debug tcov opt : $(EVERYTHING)

/var$(PROTO)/cache :
	mkdir -p $@

/var$(PROTO)/cache/config :
	touch $@
	chmod 0777 $@

include $(DEV)/makefile.end

tar $(TARFILE) : $(SOURCES.all)
	tar cvf $(TARFILE) $(SOURCES.all)

clean ::

.WAIT:

