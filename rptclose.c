/*----------------------------------------------------------------------------
	Program : rptclose.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Close final report file, prior to sending to selected output.
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

void rptclose ( REPORT_OPTIONS *ReportOptions )
{
	switch ( ReportOptions->Format )
	{
		case RPT_FORMAT_CSV:
		case RPT_FORMAT_TEXT:
		case RPT_FORMAT_PDF_VIEW:
		case RPT_FORMAT_PDF_EMAIL:
			break;
		case RPT_FORMAT_HTML:
		case RPT_FORMAT_EMAIL:
			fprintf ( ReportOptions->fpOutput, "</table>\n" );
			break;
		case RPT_FORMAT_EXCEL:
			workbook_close ( workbook );
			break;
	}

	if ( ReportOptions->IsStdout == 0 && ReportOptions->Format != RPT_FORMAT_EXCEL )
	{
		nsFclose ( ReportOptions->fpOutput );
	}
}
