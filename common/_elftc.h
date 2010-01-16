/*-
 * Copyright (c) 2009 Joseph Koshy
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id$
 */

/**
 ** Miscellanous definitions needed by multiple components.
 **/

#ifndef	_ELFTC_H
#define	_ELFTC_H

#ifndef	NULL
#define NULL 	((void *) 0)
#endif

/*
 * Supply macros missing from <sys/queue.h>
 */

#ifndef	STAILQ_FOREACH_SAFE
#define STAILQ_FOREACH_SAFE(var, head, field, tvar)            \
       for ((var) = STAILQ_FIRST((head));                      \
            (var) && ((tvar) = STAILQ_NEXT((var), field), 1);  \
            (var) = (tvar))
#endif

#ifndef	STAILQ_LAST
#define STAILQ_LAST(head, type, field)                                  \
        (STAILQ_EMPTY((head)) ?                                         \
                NULL :                                                  \
                ((struct type *)(void *)                                \
                ((char *)((head)->stqh_last) - offsetof(struct type, field))))
#endif

#ifndef	TAILQ_FOREACH_SAFE
#define TAILQ_FOREACH_SAFE(var, head, field, tvar)                      \
	for ((var) = TAILQ_FIRST((head));                               \
	    (var) && ((tvar) = TAILQ_NEXT((var), field), 1);            \
	    (var) = (tvar))
#endif

/*
 * Symbols that are sometimes missing in system headers.
 */

#ifndef	DT_GNU_HASH
#define	DT_GNU_HASH		0x6FFFFEF5U
#endif

#ifndef	ELFOSABI_86OPEN
#define	ELFOSABI_86OPEN		5
#endif

#ifndef	ELFOSABI_HURD
#define	ELFOSABI_HURD		4
#endif

#ifndef	ELFOSABI_NSK
#define	ELFOSABI_NSK		14
#endif

#ifndef	ELFOSABI_OPENVMS
#define	ELFOSABI_OPENVMS	13
#endif

#ifndef	PN_XNUM
#define	PN_XNUM			0xFFFFU
#endif

#ifndef	SHT_SUNW_verdef
#define	SHT_SUNW_verdef		0x6FFFFFFD
#endif

#ifndef	SHT_SUNW_verneed
#define	SHT_SUNW_verneed	0x6FFFFFFE
#endif

#ifndef	SHT_SUNW_versym
#define	SHT_SUNW_versym		0x6FFFFFFF
#endif

#ifndef	SHN_XINDEX
#define	SHN_XINDEX		0xFFFFU
#endif

#ifndef	SHT_GNU_HASH
#define	SHT_GNU_HASH		0x6FFFFFF6U
#endif


/*
 * VCS Ids.
 */

#ifndef	ELFTC_VCSID

#if defined(__FreeBSD__)
#define	ELFTC_VCSID(ID)		__FBSDID(ID)
#endif

#if defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)
#define	ELFTC_VCSID(ID)		/**/
#endif

#if defined(__NetBSD__)
#define	ELFTC_VCSID(ID)		__RCSID(ID)
#endif

#endif	/* ELFTC_VCSID */

/*
 * Provide an equivalent for getprogname(3).
 */

#ifndef	ELFTC_GETPROGNAME

#if defined(__FreeBSD__) || defined(__NetBSD__)

#include <stdlib.h>

#define	ELFTC_GETPROGNAME()	getprogname()

#endif	/* defined(__FreeBSD__) || defined(__NetBSD__) */


#if defined(__linux__)

/*
 * GLIBC based systems have a global 'char *' pointer referencing
 * the executable's name.
 */
extern const char *program_invocation_short_name;

#define	ELFTC_GETPROGNAME()	program_invocation_short_name

#endif	/* __linux__ */

#endif	/* ELFTC_GETPROGNAME */

/**
 ** Per-OS configuration.
 **/

#if defined(__linux__)

#include <endian.h>

#define	ELFTC_BYTE_ORDER			__BYTE_ORDER
#define	ELFTC_BYTE_ORDER_LITTLE_ENDIAN		__LITTLE_ENDIAN
#define	ELFTC_BYTE_ORDER_BIG_ENDIAN		__BIG_ENDIAN

/*
 * Debian GNU/Linux is missing strmode(3).
 */
#define	ELFTC_HAVE_STRMODE	0
#endif	/* __linux__ */


#if defined(__FreeBSD__)

#include <sys/endian.h>

#define	ELFTC_BYTE_ORDER			_BYTE_ORDER
#define	ELFTC_BYTE_ORDER_LITTLE_ENDIAN		_LITTLE_ENDIAN
#define	ELFTC_BYTE_ORDER_BIG_ENDIAN		_BIG_ENDIAN

#define	ELFTC_HAVE_STRMODE	1
#endif	/* __FreeBSD__ */


#if defined(__NetBSD__)

#include <sys/endian.h>

#define	ELFTC_BYTE_ORDER			_BYTE_ORDER
#define	ELFTC_BYTE_ORDER_LITTLE_ENDIAN		_LITTLE_ENDIAN
#define	ELFTC_BYTE_ORDER_BIG_ENDIAN		_BIG_ENDIAN

#define	ELFTC_HAVE_STRMODE	1
#endif	/* __NetBSD __ */

#endif	/* _ELFTC_H */