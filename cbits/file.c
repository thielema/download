/*-
 * Copyright (c) 2006 Aaron Griffin <aaronmgriffin@gmail.com>
 *
 * Based on original libfetch code from:
 * Copyright (c) 1998-2004 Dag-Erling Co�dan Sm�rgrav
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>

#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "download.h"
#include "common.h"

FILE *
downloadXGetFile(struct url *u, struct url_stat *us, const char *flags)
{
	FILE *f;

	if (us && downloadStatFile(u, us, flags) == -1)
		return (NULL);

	f = fopen(u->doc, "r");

	if (f == NULL)
		_download_syserr();

	if (u->offset && fseeko(f, u->offset, SEEK_SET) == -1) {
		fclose(f);
		_download_syserr();
	}

	return (f);
}

FILE *
downloadGetFile(struct url *u, const char *flags)
{
	return (downloadXGetFile(u, NULL, flags));
}

static int
_download_stat_file(const char *fn, struct url_stat *us)
{
	struct stat sb;

	us->size = -1;
	us->atime = us->mtime = 0;
	if (stat(fn, &sb) == -1) {
		_download_syserr();
		return (-1);
	}
	us->size = sb.st_size;
	us->atime = sb.st_atime;
	us->mtime = sb.st_mtime;
	return (0);
}

int
downloadStatFile(struct url *u, struct url_stat *us, const char *flags)
{
	return (_download_stat_file(u->doc, us));
}
