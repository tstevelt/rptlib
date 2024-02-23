/*----------------------------------------------------------------------------
	Program : rptextension.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Return file name extension depending on selected report format.
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/
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


#include	"rptprivate.h"
#include	"rptlib.h"

char *rptextension ( int Format )
{
	switch ( Format )
	{
		case RPT_FORMAT_CSV:
			return ( ".csv" );
		case RPT_FORMAT_EXCEL:
			return ( ".xlsx" );
		case RPT_FORMAT_TEXT:
			return ( ".txt" );
		case RPT_FORMAT_PDF_VIEW:
		case RPT_FORMAT_PDF_EMAIL:
			return ( ".pdf" );
		case RPT_FORMAT_HTML:
		case RPT_FORMAT_EMAIL:
			return ( ".html" );
	}
	return ( ".data" );
}

