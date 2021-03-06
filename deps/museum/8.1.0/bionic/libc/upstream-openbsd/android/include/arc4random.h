/*	$OpenBSD: arc4random_linux.h,v 1.7 2014/07/20 20:51:13 bcook Exp $	*/

/*
 * Copyright (c) 1996, David Mazieres <dm@uun.org>
 * Copyright (c) 2008, Damien Miller <djm@openbsd.org>
 * Copyright (c) 2013, Markus Friedl <markus@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Stub functions for portability.
 */

#include <sys/mman.h>

#include <pthread.h>
#include <signal.h>

#include <async_safe/log.h>

#include "private/bionic_prctl.h"

// Android gets these from "thread_private.h".
#include "thread_private.h"
//static pthread_mutex_t arc4random_mtx = PTHREAD_MUTEX_INITIALIZER;
//#define _ARC4_LOCK()   pthread_mutex_lock(&arc4random_mtx)
//#define _ARC4_UNLOCK() pthread_mutex_unlock(&arc4random_mtx)

#ifdef __GLIBC__
extern void *__dso_handle;
extern int __register_atfork(void (*)(void), void(*)(void), void (*)(void), void *);
#define _ARC4_ATFORK(f) __register_atfork(NULL, NULL, (f), __dso_handle)
#else
#define _ARC4_ATFORK(f) pthread_atfork(NULL, NULL, (f))
#endif

static inline void
_getentropy_fail(void)
{
	async_safe_fatal("getentropy failed");
}

volatile sig_atomic_t _rs_forked;

static inline void
_rs_forkdetect(void)
{
	static pid_t _rs_pid = 0;
	pid_t pid = getpid();

	if (_rs_pid == 0 || _rs_pid != pid || _rs_forked) {
		_rs_pid = pid;
		_rs_forked = 0;
		if (rs)
			memset(rs, 0, sizeof(*rs));
	}
}

static inline int
_rs_allocate(struct _rs **rsp, struct _rsx **rsxp)
{
	// OpenBSD's arc4random_linux.h allocates two separate mappings, but for
	// themselves they just allocate both structs into one mapping like this.
	struct {
		struct _rs rs;
		struct _rsx rsx;
	} *p;

	if ((p = mmap(NULL, sizeof(*p), PROT_READ|PROT_WRITE,
	    MAP_ANON|MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (-1);

	prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, p, sizeof(*p), "arc4random data");

	*rsp = &p->rs;
	*rsxp = &p->rsx;

	return (0);
}
