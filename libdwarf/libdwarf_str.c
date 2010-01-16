/*-
 * Copyright (c) 2009 Kai Wang
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
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "_libdwarf.h"

#define	_INIT_DWARF_STRTAB_SIZE 1024

int
_dwarf_strtab_add(Dwarf_Debug dbg, char *string, uint64_t *off,
    Dwarf_Error *error)
{
	size_t len;

	assert(dbg != NULL && string != NULL);

	len = strlen(string) + 1;
	while (dbg->dbg_strtab_size + len > dbg->dbg_strtab_cap) {
		dbg->dbg_strtab_cap *= 2;
		dbg->dbg_strtab = realloc(dbg->dbg_strtab, dbg->dbg_strtab_cap);
		if (dbg->dbg_strtab == NULL) {
			DWARF_SET_ERROR(error, DWARF_E_MEMORY);
			return (DWARF_E_MEMORY);
		}
	}

	if (off != NULL)
		*off = dbg->dbg_strtab_size;

	strncpy(&dbg->dbg_strtab[dbg->dbg_strtab_size], string, len - 1);
	dbg->dbg_strtab_size += len;
	dbg->dbg_strtab[dbg->dbg_strtab_size - 1] = '\0';

	return (DWARF_E_NONE);
}

char *
_dwarf_strtab_get_table(Dwarf_Debug dbg)
{

	assert(dbg != NULL);

	return (dbg->dbg_strtab);
}

int
_dwarf_strtab_init(Dwarf_Debug dbg, Dwarf_Error *error)
{
	Dwarf_Section *ds;

	assert(dbg != NULL);

	if (dbg->dbg_mode == DW_DLC_READ || dbg->dbg_mode == DW_DLC_RDWR) {
		ds = _dwarf_find_section(dbg, ".debug_str");
		if (ds == NULL) {
			dbg->dbg_strtab = NULL;
			dbg->dbg_strtab_cap = dbg->dbg_strtab_size = 0;
			return (DWARF_E_NONE);
		}
		
		dbg->dbg_strtab_cap = dbg->dbg_strtab_size = ds->ds_size;

		if (dbg->dbg_mode == DW_DLC_RDWR) {
			if ((dbg->dbg_strtab = malloc(ds->ds_size)) == NULL) {
				DWARF_SET_ERROR(error, DWARF_E_MEMORY);
				return (DWARF_E_MEMORY);
			}
			memcpy(dbg->dbg_strtab, ds->ds_data, ds->ds_size);
		} else 
			dbg->dbg_strtab = (char *) ds->ds_data;
	} else {
		/* DW_DLC_WRITE */

		dbg->dbg_strtab_cap = _INIT_DWARF_STRTAB_SIZE;
		dbg->dbg_strtab_size = 0;

		if ((dbg->dbg_strtab = malloc(dbg->dbg_strtab_cap)) == NULL) {
			DWARF_SET_ERROR(error, DWARF_E_MEMORY);
			return (DWARF_E_MEMORY);
		}

		dbg->dbg_strtab[0] = '\0';
	}

	return (DWARF_E_NONE);
}

void
_dwarf_strtab_cleanup(Dwarf_Debug dbg)
{

	assert(dbg != NULL);

	if (dbg->dbg_mode == DW_DLC_RDWR || dbg->dbg_mode == DW_DLC_WRITE)
		free(dbg->dbg_strtab);
}