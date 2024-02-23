/*----------------------------------------------------------------------------
	Program : ValidReportFormat.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Verify format passed on the command line.
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

#include	"shslib.h"
#include	"rptlib.h"

char ValidReportFormat ( char *FormatArg )
{
	char	rv;

	if ( nsStrcmp ( FormatArg, "csv" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_CSV; 
	} 
	else if ( nsStrcmp ( FormatArg, "txt" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_TEXT; 
	} 
	else if ( nsStrcmp ( FormatArg, "html" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_HTML; 
	} 
	else if ( nsStrcmp ( FormatArg, "email" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_EMAIL; 
	} 
#ifdef INVEST_SYSTEM
	else if ( nsStrcmp ( FormatArg, "aw" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_APPEND_W; 
	} 
	else if ( nsStrcmp ( FormatArg, "rw" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_REPLACE_W; 
	} 
	else if ( nsStrcmp ( FormatArg, "tmp" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_TEMP; 
	} 
#endif
	else if ( nsStrcmp ( FormatArg, "view" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_PDF_VIEW; 
	} 
	else if ( nsStrcmp ( FormatArg, "pdf" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_PDF_EMAIL; 
	} 
	else if ( nsStrcmp ( FormatArg, "xlsx" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_EXCEL; 
	} 
	else 
	{ 
		printf ( "Unknown format\n" ); 
	} 

	return ( rv );
}
