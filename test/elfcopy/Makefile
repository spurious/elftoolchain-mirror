# $Id$

TOP=		../..

ELFCOPY=	${TOP}/elfcopy/elfcopy

TEST_LOG=	test.log

.MAIN:	all

.PHONY:	clobber execute test

execute test: ${ELFCOPY}
	/bin/sh run.sh

clobber:	clean
	rm -f ${TEST_LOG}

SUBDIR=		plugin

.include <bsd.subdir.mk>
