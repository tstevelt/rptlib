/*----------------------------------------------------------------------------
	Program : rptfuncs.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Various functions for creating reports.
				void rptNewLine ( ... )
				void PrintColumnBgcolor ( ... )
				void rptPaintFormat ( ... )
				int CountColumnsWithTotals ( ... )

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

void rptNewLine ( REPORT_OPTIONS *ReportOptions )
{
	switch ( ReportOptions->Format )
	{
		case RPT_FORMAT_CSV:
		case RPT_FORMAT_TEXT:
		case RPT_FORMAT_PDF_VIEW:
		case RPT_FORMAT_PDF_EMAIL:
			fprintf ( ReportOptions->fpOutput, "\n" );
			break;
		case RPT_FORMAT_HTML:
		case RPT_FORMAT_EMAIL:
			fprintf ( ReportOptions->fpOutput, "</tr>\n" );
			break;
		case RPT_FORMAT_EXCEL:
		case RPT_FORMAT_APPEND_W:
		case RPT_FORMAT_REPLACE_W:
		case RPT_FORMAT_TEMP:
			break;
	}

	ReportOptions->CurrentLineNumber++;
}

void PrintColumnBgcolor ( REPORT_OPTIONS *ReportOptions, COLUMN_HEADINGS *Column )
{
	long	Color;

	switch ( ReportOptions->Format )
	{
		case RPT_FORMAT_HTML:
		case RPT_FORMAT_EMAIL:
			Color = Column->Options & (COLUMN_COLORS);
			switch ( Color )
			{
				case COLUMN_RED:
					fprintf ( ReportOptions->fpOutput, " bgcolor='#ff9999'" );
					break;
				case COLUMN_GREEN:
					fprintf ( ReportOptions->fpOutput, " bgcolor='#99ff99'" );
					break;
				case COLUMN_BLUE:
					fprintf ( ReportOptions->fpOutput, " bgcolor='#9999ff'" );
					break;
				case COLUMN_YELLOW:
					fprintf ( ReportOptions->fpOutput, " bgcolor='#ffff99'" );
					break;
				case COLUMN_VIOLET:
					fprintf ( ReportOptions->fpOutput, " bgcolor='#ff99ff'" );
					break;
				case COLUMN_CYAN:
					fprintf ( ReportOptions->fpOutput, " bgcolor='#99ffff'" );
					break;
			}
			break;
	}
}

void rptPaintFormat ( char *Label, char ReportFormat )
{
	printf ("<tr>\n" );
	printf ("<td class='report-table'>%s</td>\n", Label );
	printf ("<td class='report-table' colspan='3'>\n" );

	printf ("<select name='ReportFormat' id='ReportFormat'>\n" );

	printf ("<option value='%c'%s>Screen</option>\n", RPT_FORMAT_HTML, ReportFormat == RPT_FORMAT_HTML ? " selected" : "" );
//	printf ("<option value='%c'%s>Email</option>\n", RPT_FORMAT_EMAIL, ReportFormat == RPT_FORMAT_EMAIL ? " selected" : "" );
	printf ("<option value='%c'%s>CSV</option>\n",  RPT_FORMAT_CSV, ReportFormat == RPT_FORMAT_CSV ? " selected" : "" );
	printf ("<option value='%c'%s>Text</option>\n", RPT_FORMAT_TEXT, ReportFormat == RPT_FORMAT_TEXT ? " selected" : "" );
	printf ("<option value='%c'%s>View PDF</option>\n", RPT_FORMAT_PDF_VIEW,  ReportFormat ==  RPT_FORMAT_PDF_VIEW ? " selected" : "" );
	printf ("<option value='%c'%s>Email PDF</option>\n", RPT_FORMAT_PDF_EMAIL,  ReportFormat ==  RPT_FORMAT_PDF_EMAIL ? " selected" : "" );
	printf ("<option value='%c'%s>Excel</option>\n", RPT_FORMAT_EXCEL, ReportFormat == RPT_FORMAT_EXCEL ? " selected" : "" );

	printf ("</select>\n" );
	printf ("</td>\n" );
	printf ("</tr>\n" );
}

int CountColumnsWithTotals ( int *ColumnsWithTotals, int *FirstColumnWithTotals, COLUMN_HEADINGS ColumnArray[], int ColumnCount )
{
	int		xc;

	/*---------------------------------------------------------------------------
		user should not call us unless he asked for totals, but check anyway.
		yep, this is a good test.
	---------------------------------------------------------------------------*/
	*ColumnsWithTotals = 0;
	*FirstColumnWithTotals = -1;
	for ( xc = 0; xc < ColumnCount; xc++ )
	{
		if ( (ColumnArray[xc].Options & COLUMN_TOTAL) == COLUMN_TOTAL )
		{
			*ColumnsWithTotals = *ColumnsWithTotals + 1;
			if ( *FirstColumnWithTotals == -1 )
			{
				*FirstColumnWithTotals = xc;
			}
		}
	}

	return ( *ColumnsWithTotals > 0 ? 1 : 0 );
}
