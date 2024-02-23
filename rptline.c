/*----------------------------------------------------------------------------
	Program : rptline.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Output a single line of the report.
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

static	int	DebugRptLine = 0;

static	char	BreakControl[1024];
static	char	BreakTest   [1024];

static char *StripCommas ( char *String )
{
	for ( int ndx = 0; ndx < nsStrlen(String); ndx++ )
	{
		if ( String[ndx] == ',' )
		{
			String[ndx] = ' ';
		}
	}
	return ( String );
}

void rptline ( REPORT_OPTIONS *ReportOptions, COLUMN_HEADINGS ColumnArray[], int ColumnCount, char *tokens[], int tokcnt )
{
	int		xl, xc, xt, Align, ColumnsWithTotals, FirstColumnWithTotals;
	static	int		firstpass = 1;

	if ( DebugRptLine )
	{
		printf ( "tokcnt %d, CurrentPageNumber %d CurrentLineNumber %d LinesPerPage %d<br>", 
				tokcnt,
				ReportOptions->CurrentPageNumber, 
				ReportOptions->CurrentLineNumber, 
				ReportOptions->LinesPerPage );

	}

	/*----------------------------------------------------------
		initialize break control if there are breaks;
	----------------------------------------------------------*/
	if ( firstpass == 1 )
	{
		firstpass = 0;
		if ( ReportOptions->BreakCount > 0 )
		{
			memset ( BreakControl, 'z', sizeof(BreakControl) );
			CountColumnsWithTotals ( &ColumnsWithTotals, &FirstColumnWithTotals, ColumnArray, ColumnCount );
		}
	}

	if (( ReportOptions->CurrentPageNumber == 0 ) ||
		( ReportOptions->LinesPerPage > 0 && ReportOptions->CurrentLineNumber >= ReportOptions->LinesPerPage ))
	{
		if ( DebugRptLine )
		{
			printf ( "Call Header<br>\n" );
		}

		rptheader ( ReportOptions, ColumnArray, ColumnCount );
	}

	switch ( ReportOptions->Format )
	{
		case RPT_FORMAT_CSV:
			if ( ReportOptions->BreakCount > 0 )
			{
				memset ( BreakTest, '\0', sizeof(BreakTest) );

				for ( xc = 0, xt = 0; xt < ReportOptions->BreakCount && xt < tokcnt; xc++, xt++ )
				{
					strcat ( BreakTest, tokens[xt] );
				}

				xl = nsStrlen ( BreakTest );
				if ( nsStrncmp ( BreakControl, BreakTest, xl ) != 0 )
				{
					if (( nsStrncmp ( BreakControl, "zzzzz", 5 ) != 0 ) && 
						( ColumnsWithTotals > 0                     ))
					{
						rpttotal ( ReportOptions, ColumnArray, ColumnCount, 0 );
					}

					for ( xc = 0, xt = 0; xt < ReportOptions->BreakCount && xt < tokcnt; xc++, xt++ )
					{	
						fprintf ( ReportOptions->fpOutput, "%s", StripCommas(tokens[xt]) );

						if ( xt + 1 < tokcnt )
						{
							fprintf ( ReportOptions->fpOutput, "," );
						}
					}
					rptNewLine ( ReportOptions );
					sprintf ( BreakControl, "%s", BreakTest );
				}
			}

			for ( xc = 0, xt = ReportOptions->BreakCount; xt < tokcnt; xc++, xt++ )
			{	
				fprintf ( ReportOptions->fpOutput, "%s", StripCommas(tokens[xt]) );
				if ( xt + 1 < tokcnt )
				{
					fprintf ( ReportOptions->fpOutput, "," );
				}
			}
			rptNewLine ( ReportOptions );
			break;

		case RPT_FORMAT_TEXT:
		case RPT_FORMAT_PDF_VIEW:
		case RPT_FORMAT_PDF_EMAIL:
			if ( ReportOptions->BreakCount > 0 )
			{
				memset ( BreakTest, '\0', sizeof(BreakTest) );

				for ( xc = 0, xt = 0; xt < ReportOptions->BreakCount && xt < tokcnt; xc++, xt++ )
				{
					strcat ( BreakTest, tokens[xt] );
				}

				xl = nsStrlen ( BreakTest );
				if ( nsStrncmp ( BreakControl, BreakTest, xl ) != 0 )
				{
					if (( nsStrncmp ( BreakControl, "zzzzz", 5 ) != 0 ) && 
						( ColumnsWithTotals > 0                     ))
					{
						rpttotal ( ReportOptions, ColumnArray, ColumnCount, 0 );
					}

					for ( xc = 0, xt = 0; xt < ReportOptions->BreakCount && xt < tokcnt; xc++, xt++ )
					{	
						fprintf ( ReportOptions->fpOutput, "%s", tokens[xt] );

						if ( xt + 1 < tokcnt )
						{
							fprintf ( ReportOptions->fpOutput, " " );
						}
					}
					rptNewLine ( ReportOptions );
					sprintf ( BreakControl, "%s", BreakTest );
				}
			}

			for ( xc = 0, xt = ReportOptions->BreakCount; xt < tokcnt; xc++, xt++ )
			{	
				xl = ColumnArray[xc].Width;
				if ( (ColumnArray[xc].Options & COLUMN_RIGHT) == COLUMN_RIGHT )
				{
					fprintf ( ReportOptions->fpOutput, "%*.*s", xl, xl, tokens[xt] );
				}
				else
				{
					fprintf ( ReportOptions->fpOutput, "%-*.*s", xl, xl, tokens[xt] );
				}
				if ( xt + 1 < tokcnt )
				{
					fprintf ( ReportOptions->fpOutput, " " );
				}
			}
			rptNewLine ( ReportOptions );
			break;

		case RPT_FORMAT_HTML:
		case RPT_FORMAT_EMAIL:
			if ( ReportOptions->BreakCount > 0 )
			{
				memset ( BreakTest, '\0', sizeof(BreakTest) );

				for ( xc = 0, xt = 0; xt < ReportOptions->BreakCount && xt < tokcnt; xc++, xt++ )
				{
					strcat ( BreakTest, tokens[xt] );
				}

				xl = nsStrlen ( BreakTest );
				if ( nsStrncmp ( BreakControl, BreakTest, xl ) != 0 )
				{
					if (( nsStrncmp ( BreakControl, "zzzzz", 5 ) != 0 ) && 
						( ColumnsWithTotals > 0                     ))
					{
						rpttotal ( ReportOptions, ColumnArray, ColumnCount, 0 );
					}
					//tms 05/26/2021 rptNewLine ( ReportOptions );

					//tms 05/26/2021
					fprintf ( ReportOptions->fpOutput, "<tr>" );
					for ( xc = 0, xt = 0; xt < ReportOptions->BreakCount && xt < tokcnt; xc++, xt++ )
					{	
						Align = ColumnArray[xc].Options & (COLUMN_LEFT|COLUMN_CENTER|COLUMN_RIGHT);
						switch ( Align )
						{
							case COLUMN_LEFT:
								fprintf ( ReportOptions->fpOutput, "<td" );
								break;
							case COLUMN_CENTER:
								fprintf ( ReportOptions->fpOutput, "<td align='center'" );
								break;
							case COLUMN_RIGHT:
								fprintf ( ReportOptions->fpOutput, "<td align='right'" );
								break;
						}

						PrintColumnBgcolor ( ReportOptions, &ColumnArray[xc] );
						fprintf ( ReportOptions->fpOutput, ">%s</td>", tokens[xt] );
					}
					rptNewLine ( ReportOptions );
					sprintf ( BreakControl, "%s", BreakTest );
				}
			}
			//tms 05/26/2021 rptNewLine ( ReportOptions );

			//tms 05/26/2021
			fprintf ( ReportOptions->fpOutput, "<tr>" );
			for ( xc = 0, xt = ReportOptions->BreakCount; xt < tokcnt; xc++, xt++ )
			{	
				Align = ColumnArray[xc].Options & (COLUMN_LEFT|COLUMN_CENTER|COLUMN_RIGHT);
				switch ( Align )
				{
					case COLUMN_LEFT:
						fprintf ( ReportOptions->fpOutput, "<td" );
						break;
					case COLUMN_CENTER:
						fprintf ( ReportOptions->fpOutput, "<td align='center'" );
						break;
					case COLUMN_RIGHT:
						fprintf ( ReportOptions->fpOutput, "<td align='right'" );
						break;
				}

				PrintColumnBgcolor ( ReportOptions, &ColumnArray[xc] );
				fprintf ( ReportOptions->fpOutput, ">%s</td>", tokens[xt] );
			}
			rptNewLine ( ReportOptions );
			break;

		case RPT_FORMAT_EXCEL:
			if ( ReportOptions->BreakCount > 0 )
			{
				memset ( BreakTest, '\0', sizeof(BreakTest) );

				for ( xc = 0, xt = 0; xt < ReportOptions->BreakCount && xt < tokcnt; xc++, xt++ )
				{
					strcat ( BreakTest, tokens[xt] );
				}

				xl = nsStrlen ( BreakTest );
				if ( nsStrncmp ( BreakControl, BreakTest, xl ) != 0 )
				{
					if (( nsStrncmp ( BreakControl, "zzzzz", 5 ) != 0 ) && 
						( ColumnsWithTotals > 0                     ))
					{
						rpttotal ( ReportOptions, ColumnArray, ColumnCount, 0 );
					}

					for ( xc = 0, xt = 0; xt < ReportOptions->BreakCount && xt < tokcnt; xc++, xt++ )
					{	
						if ( (ColumnArray[xc].Options & COLUMN_STRING) == COLUMN_STRING )
						{
							worksheet_write_string ( worksheet, ReportOptions->CurrentLineNumber, xc, tokens[xt], ColumnArray[xc].DataFormat );
						}
						else if ( (ColumnArray[xc].Options & COLUMN_LONG) == COLUMN_LONG )
						{
							worksheet_write_number ( worksheet, ReportOptions->CurrentLineNumber, xc, nsAtol(tokens[xt]), ColumnArray[xc].DataFormat );
						}
						else if ( (ColumnArray[xc].Options & COLUMN_DOUBLE) == COLUMN_DOUBLE )
						{
							worksheet_write_number ( worksheet, ReportOptions->CurrentLineNumber, xc, nsAtof(tokens[xt]), ColumnArray[xc].DataFormat );
						}
					}
					rptNewLine ( ReportOptions );
					sprintf ( BreakControl, "%s", BreakTest );
				}
			}

			for ( xc = 0, xt = ReportOptions->BreakCount; xt < tokcnt; xc++, xt++ )
			{
				if ( (ColumnArray[xc].Options & COLUMN_STRING) == COLUMN_STRING )
				{
					worksheet_write_string ( worksheet, ReportOptions->CurrentLineNumber, xc, tokens[xt], ColumnArray[xc].DataFormat );
				}
				else if ( (ColumnArray[xc].Options & COLUMN_LONG) == COLUMN_LONG )
				{
					worksheet_write_number ( worksheet, ReportOptions->CurrentLineNumber, xc, nsAtol(tokens[xt]), ColumnArray[xc].DataFormat );
				}
				else if ( (ColumnArray[xc].Options & COLUMN_DOUBLE) == COLUMN_DOUBLE )
				{
					worksheet_write_number ( worksheet, ReportOptions->CurrentLineNumber, xc, nsAtof(tokens[xt]), ColumnArray[xc].DataFormat );
				}
			}
			rptNewLine ( ReportOptions );

			break;
	}

	for ( xc = 0, xt = ReportOptions->BreakCount; xt < tokcnt; xc++, xt++ )
	{
		if ( (ColumnArray[xc].Options & COLUMN_TOTAL) == COLUMN_TOTAL )
		{
			if ( (ColumnArray[xc].Options & COLUMN_LONG) == COLUMN_LONG )
			{
				ColumnArray[xc].Total.longTotal[0] += nsAtol ( tokens[xt] );
				ColumnArray[xc].Total.longTotal[1] += nsAtol ( tokens[xt] );
			}
			else if ( (ColumnArray[xc].Options & COLUMN_DOUBLE) == COLUMN_DOUBLE )
			{
				ColumnArray[xc].Total.doubleTotal[0] += nsAtof ( tokens[xt] );
				ColumnArray[xc].Total.doubleTotal[1] += nsAtof ( tokens[xt] );
			}
		}
	}
}
