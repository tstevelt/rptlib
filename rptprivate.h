/*---------------------------------------------------------------------------
    Generate reports in various formats

    Copyright (C)  Nov 2020 Tom Stevelt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
---------------------------------------------------------------------------*/


#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>

#include	"xlsxwriter.h"

#include	"shslib.h"

#ifndef RPT_TYPE
#ifdef RPT_MAIN
#define	RPT_TYPE	/* */
#else
#define	RPT_TYPE	extern
#endif
#endif

RPT_TYPE	lxw_workbook	*workbook;
RPT_TYPE	lxw_worksheet	*worksheet;

