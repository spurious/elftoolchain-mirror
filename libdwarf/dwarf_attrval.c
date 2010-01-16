/*-
 * Copyright (c) 2007 John Birrell (jb@freebsd.org)
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

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "_libdwarf.h"

int
dwarf_attrval_flag(Dwarf_Die die, uint64_t attr, Dwarf_Bool *valp, Dwarf_Error *err)
{
	Dwarf_Attribute at;

	if (die == NULL || valp == NULL) {
		DWARF_SET_ERROR(err, DWARF_E_ARGUMENT);
		return (DW_DLV_ERROR);
	}

	*valp = 0;

	if ((at = _dwarf_attr_find(die, attr)) == NULL) {
		DWARF_SET_ERROR(err, DWARF_E_NO_ENTRY);
		return (DW_DLV_NO_ENTRY);
	}

	switch (at->at_form) {
	case DW_FORM_flag:
		*valp = (Dwarf_Bool) at->u[0].u64;
		break;
	default:
		DWARF_SET_ERROR(err, DWARF_E_BAD_FORM);
		return (DW_DLV_ERROR);
	}

	return (DW_DLV_OK);
}

int
dwarf_attrval_string(Dwarf_Die die, uint64_t attr, const char **strp, Dwarf_Error *err)
{
	Dwarf_Attribute at;

	if (die == NULL || strp == NULL) {
		DWARF_SET_ERROR(err, DWARF_E_ARGUMENT);
		return (DW_DLV_ERROR);
	}

	*strp = NULL;

	if ((at = _dwarf_attr_find(die, attr)) == NULL) {
		DWARF_SET_ERROR(err, DWARF_E_NO_ENTRY);
		return (DW_DLV_NO_ENTRY);
	}

	switch (at->at_form) {
	case DW_FORM_strp:
		*strp = at->u[1].s;
		break;
	case DW_FORM_string:
		*strp = at->u[0].s;
		break;
	default:
		DWARF_SET_ERROR(err, DWARF_E_BAD_FORM);
		return (DW_DLV_ERROR);
	}

	return (DW_DLV_OK);
}

int
dwarf_attrval_signed(Dwarf_Die die, uint64_t attr, Dwarf_Signed *valp, Dwarf_Error *err)
{
	Dwarf_Attribute at;

	if (die == NULL || valp == NULL) {
		DWARF_SET_ERROR(err, DWARF_E_ARGUMENT);
		return (DW_DLV_ERROR);
	}

	*valp = 0;

	if ((at = _dwarf_attr_find(die, attr)) == NULL) {
		DWARF_SET_ERROR(err, DWARF_E_NO_ENTRY);
		return (DW_DLV_NO_ENTRY);
	}

	switch (at->at_form) {
	case DW_FORM_data1:
	case DW_FORM_sdata:
		*valp = at->u[0].s64;
		break;
	default:
		DWARF_SET_ERROR(err, DWARF_E_BAD_FORM);
		return (DW_DLV_ERROR);
	}

	return (DW_DLV_OK);
}

int
dwarf_attrval_unsigned(Dwarf_Die die, uint64_t attr, Dwarf_Unsigned *valp, Dwarf_Error *err)
{
	Dwarf_Attribute at;
	Dwarf_Die die1;
	Dwarf_Unsigned val;

	if (die == NULL || valp == NULL) {
		DWARF_SET_ERROR(err, DWARF_E_ARGUMENT);
		return (DW_DLV_ERROR);
	}

	*valp = 0;

	if ((at = _dwarf_attr_find(die, attr)) == NULL && attr != DW_AT_type) {
		DWARF_SET_ERROR(err, DWARF_E_NO_ENTRY);
		return (DW_DLV_NO_ENTRY);
	}

	if (at == NULL &&
	    (at = _dwarf_attr_find(die, DW_AT_abstract_origin)) != NULL) {
		switch (at->at_form) {
		case DW_FORM_addr:
		case DW_FORM_data1:
		case DW_FORM_data2:
		case DW_FORM_data4:
		case DW_FORM_data8:
		case DW_FORM_ref1:
		case DW_FORM_ref2:
		case DW_FORM_ref4:
		case DW_FORM_ref8:
		case DW_FORM_ref_udata:
			val = at->u[0].u64;
			if ((die1 = _dwarf_die_find(die, val)) == NULL ||
			    (at = _dwarf_attr_find(die1, attr)) == NULL) {
				DWARF_SET_ERROR(err, DWARF_E_NO_ENTRY);
				return (DW_DLV_NO_ENTRY);
			}
			break;
		default:
			DWARF_SET_ERROR(err, DWARF_E_BAD_FORM);
			return (DW_DLV_ERROR);
		}
	}

	switch (at->at_form) {
	case DW_FORM_addr:
	case DW_FORM_data1:
	case DW_FORM_data2:
	case DW_FORM_data4:
	case DW_FORM_data8:
	case DW_FORM_ref1:
	case DW_FORM_ref2:
	case DW_FORM_ref4:
	case DW_FORM_ref8:
	case DW_FORM_ref_udata:
		*valp = at->u[0].u64;
		break;
	default:
		DWARF_SET_ERROR(err, DWARF_E_BAD_FORM);
		return (DW_DLV_ERROR);
	}

	return (DW_DLV_OK);
}