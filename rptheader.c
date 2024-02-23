/*----------------------------------------------------------------------------
	Program : rptheader.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Output the beginning for a report. Called for every new page
				of paged reports (eg not CSV).
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

static	int	DebugRptHeader = 0;

void rptheader ( REPORT_OPTIONS *ReportOptions, COLUMN_HEADINGS ColumnArray[], int ColumnCount )
{
	int		xc, xl, indent, spacer, Align;

	ReportOptions->CurrentPageNumber++;
	ReportOptions->CurrentLineNumber = 0;

	switch ( ReportOptions->Format )
	{
		case RPT_FORMAT_CSV:
			fprintf ( ReportOptions->fpOutput, "%s", ReportOptions->Title );
			rptNewLine ( ReportOptions );
			if ( nsStrlen ( ReportOptions->Subtitle ) > 0 )
			{
				fprintf ( ReportOptions->fpOutput, "%s", ReportOptions->Subtitle );
				rptNewLine ( ReportOptions );
			}
			if ( ReportOptions->HeaderRows < 1 )
			{
				break;
			}
			for ( xc = 0; xc < ColumnCount; xc++ )
			{
				fprintf ( ReportOptions->fpOutput, "%s", ColumnArray[xc].Row1 );
				if ( xc + 1 < ColumnCount )
				{
					fprintf ( ReportOptions->fpOutput, "," );
				}
			}
			rptNewLine ( ReportOptions );
			if ( ReportOptions->HeaderRows == 2 )
			{
				for ( xc = 0; xc < ColumnCount; xc++ )
				{
					fprintf ( ReportOptions->fpOutput, "%s", ColumnArray[xc].Row2 );
					if ( xc + 1 < ColumnCount )
					{
						fprintf ( ReportOptions->fpOutput, "," );
					}
				}
				rptNewLine ( ReportOptions );
			}
			break;

		case RPT_FORMAT_TEXT:
		case RPT_FORMAT_PDF_VIEW:
		case RPT_FORMAT_PDF_EMAIL:
			if ( ReportOptions->CurrentPageNumber > 1 )
			{
				if ( DebugRptHeader )
				{
					printf ( "Form Feed<br>\n" );
				}

				fputc ( '\f', ReportOptions->fpOutput );
			}

			indent = (ReportOptions->TotalWidth - ReportOptions->TitleWidth) / 2;
			if ( ReportOptions->WritePageNumbers )
			{
				spacer = ReportOptions->TotalWidth - ReportOptions->TitleWidth - indent - 9;
				fprintf ( ReportOptions->fpOutput, "%*.*s%s%*.*sPAGE: %3d", 
					indent, indent, " ",  ReportOptions->Title,
					spacer, spacer,  " ", (ReportOptions->CurrentPageNumber) );
			}
			else
			{
				fprintf ( ReportOptions->fpOutput, "%*.*s%s", indent, indent, " ",  ReportOptions->Title );
			}
			rptNewLine ( ReportOptions );
			if ( nsStrlen ( ReportOptions->Subtitle ) > 0 )
			{
				fprintf ( ReportOptions->fpOutput, "%*.*s%s", indent, indent, " ", ReportOptions->Subtitle );
				rptNewLine ( ReportOptions );
			}
			for ( xc = 0; xc < ReportOptions->TotalWidth; xc++ )
			{
				fputc ( '-', ReportOptions->fpOutput );
			}
			rptNewLine ( ReportOptions );
			if ( ReportOptions->HeaderRows < 1 )
			{
				break;
			}
			for ( xc = 0; xc < ColumnCount; xc++ )
			{
				xl = ColumnArray[xc].Width;
				if ( (ColumnArray[xc].Options & COLUMN_RIGHT) == COLUMN_RIGHT )
				{
					fprintf ( ReportOptions->fpOutput, "%*.*s", xl, xl, ColumnArray[xc].Row1 );
				}
				else
				{
					fprintf ( ReportOptions->fpOutput, "%-*.*s", xl, xl, ColumnArray[xc].Row1 );
				}
				if ( xc + 1 < ColumnCount )
				{
					fprintf ( ReportOptions->fpOutput, " " );
				}
			}
			rptNewLine ( ReportOptions );
			if ( ReportOptions->HeaderRows < 1 )
			{
				break;
			}
			if ( ReportOptions->HeaderRows == 2 )
			{
				for ( xc = 0; xc < ColumnCount; xc++ )
				{
					xl = ColumnArray[xc].Width;
					if ( (ColumnArray[xc].Options & COLUMN_RIGHT) == COLUMN_RIGHT )
					{
						fprintf ( ReportOptions->fpOutput, "%*.*s", xl, xl, ColumnArray[xc].Row2 );
					}
					else
					{
						fprintf ( ReportOptions->fpOutput, "%-*.*s", xl, xl, ColumnArray[xc].Row2 );
					}
					if ( xc + 1 < ColumnCount )
					{
						fprintf ( ReportOptions->fpOutput, " " );
					}
				}
				rptNewLine ( ReportOptions );
			}
			for ( xc = 0; xc < ReportOptions->TotalWidth; xc++ )
			{
				fputc ( '-', ReportOptions->fpOutput );
			}
			rptNewLine ( ReportOptions );
			break;

		case RPT_FORMAT_HTML:
		case RPT_FORMAT_EMAIL:
			fprintf ( ReportOptions->fpOutput, "<tr><td colspan='%d'>%s</td>", ColumnCount, ReportOptions->Title );
			rptNewLine ( ReportOptions );
			if ( nsStrlen ( ReportOptions->Subtitle ) > 0 )
			{
				fprintf ( ReportOptions->fpOutput, "<tr><td colspan='%d'>%s</td>", ColumnCount, ReportOptions->Subtitle );
				rptNewLine ( ReportOptions );
			}
			if ( ReportOptions->HeaderRows < 1 )
			{
				break;
			}
			fprintf ( ReportOptions->fpOutput, "<tr>\n" );
			for ( xc = 0; xc < ColumnCount; xc++ )
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
				fprintf ( ReportOptions->fpOutput, ">%s</td>", ColumnArray[xc].Row1 );
			}
			rptNewLine ( ReportOptions );
			if ( ReportOptions->HeaderRows == 2 )
			{
				fprintf ( ReportOptions->fpOutput, "<tr>\n" );
				for ( xc = 0; xc < ColumnCount; xc++ )
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
					fprintf ( ReportOptions->fpOutput, ">%s</td>", ColumnArray[xc].Row2 );
				}

				rptNewLine ( ReportOptions );
			}
			break;

		case RPT_FORMAT_EXCEL:
			worksheet_merge_range ( worksheet, ReportOptions->CurrentLineNumber, 0, ReportOptions->CurrentLineNumber, ColumnCount - 1, ReportOptions->Title, ColumnArray[0].HeadFormat );
			rptNewLine ( ReportOptions );
			if ( nsStrlen ( ReportOptions->Subtitle ) > 0 )
			{
				worksheet_merge_range ( worksheet, ReportOptions->CurrentLineNumber, 0, ReportOptions->CurrentLineNumber, ColumnCount - 1, ReportOptions->Subtitle, ColumnArray[0].HeadFormat );
				rptNewLine ( ReportOptions );
			}
			if ( ReportOptions->HeaderRows < 1 )
			{
				break;
			}

			for ( xc = 0; xc < ColumnCount; xc++ )
			{
				worksheet_write_string ( worksheet, ReportOptions->CurrentLineNumber, xc, ColumnArray[xc].Row1, ColumnArray[xc].HeadFormat );
			}
			rptNewLine ( ReportOptions );

			if ( ReportOptions->HeaderRows == 2 )
			{
				for ( xc = 0; xc < ColumnCount; xc++ )
				{
					worksheet_write_string ( worksheet, ReportOptions->CurrentLineNumber, xc, ColumnArray[xc].Row2, ColumnArray[xc].HeadFormat );
				}
				rptNewLine ( ReportOptions );
			}

			break;
	}
}
