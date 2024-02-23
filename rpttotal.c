/*----------------------------------------------------------------------------
	Program : rpttotal.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Output subtotals and grand totals.
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

void rpttotal ( REPORT_OPTIONS *ReportOptions, COLUMN_HEADINGS ColumnArray[], int ColumnCount, int Level )
{
	int		xi, xl, xc, Align, DataType, ColumnsWithTotals, FirstColumnWithTotals;

	if ( ReportOptions->CurrentPageNumber == 0 )
	{
		return;
	}

	CountColumnsWithTotals ( &ColumnsWithTotals, &FirstColumnWithTotals, ColumnArray, ColumnCount );
	if ( ColumnsWithTotals == 0 )
	{
		return;
	}

	if ( ReportOptions->LinesPerPage > 0 && ReportOptions->CurrentLineNumber + 2 >= ReportOptions->LinesPerPage )
	{
		rptheader ( ReportOptions, ColumnArray, ColumnCount );
	}

	switch ( ReportOptions->Format )
	{
		case RPT_FORMAT_TEXT:
		case RPT_FORMAT_PDF_VIEW:
		case RPT_FORMAT_PDF_EMAIL:
			/*----------------------------------------------------------
				write extra row with equal signs ===
			----------------------------------------------------------*/
			for ( xc = 0; xc < ColumnCount; xc++ )
			{	
				xl = ColumnArray[xc].Width;
				if ( (ColumnArray[xc].Options & COLUMN_TOTAL) == COLUMN_TOTAL )
				{
					for ( xi = 0; xi < xl; xi++ )
					{
						fputc ( '=',  ReportOptions->fpOutput );
					}
				}
				else
				{
					for ( xi = 0; xi < xl; xi++ )
					{
						fputc ( ' ',  ReportOptions->fpOutput );
					}
				}

				if ( xc + 1 < ColumnCount )
				{
					fputc ( ' ', ReportOptions->fpOutput );
				}
			}
			rptNewLine ( ReportOptions );

			/*----------------------------------------------------------
				write row with totals
			----------------------------------------------------------*/
			for ( xc = 0; xc < ColumnCount; xc++ )
			{	
				xl = ColumnArray[xc].Width;
				if ( (ColumnArray[xc].Options & COLUMN_TOTAL) == COLUMN_TOTAL )
				{
					Align    = ColumnArray[xc].Options & (COLUMN_LEFT|COLUMN_CENTER|COLUMN_RIGHT);
					DataType = ColumnArray[xc].Options & (COLUMN_LONG|COLUMN_DOUBLE);
					if ( Align == COLUMN_RIGHT )
					{
						if ( DataType == COLUMN_LONG)
						{
							fprintf ( ReportOptions->fpOutput, "%*ld", xl, ColumnArray[xc].Total.longTotal[Level] );
						}
						else
						{
							fprintf ( ReportOptions->fpOutput, "%*.2f", xl, ColumnArray[xc].Total.doubleTotal[Level] );
						}
					}
					else
					{
						if ( DataType == COLUMN_LONG)
						{
							xi = fprintf ( ReportOptions->fpOutput, "%ld", ColumnArray[xc].Total.longTotal[Level] );
						}
						else
						{
							xi = fprintf ( ReportOptions->fpOutput, "%.2f", ColumnArray[xc].Total.doubleTotal[Level] );
						}
						for ( ; xi < xl; xi++ )
						{
							fputc ( ' ',  ReportOptions->fpOutput );
						}
					}
				}
				else
				{
					for ( xi = 0; xi < xl; xi++ )
					{
						fputc ( ' ',  ReportOptions->fpOutput );
					}
				}

				if ( xc + 1 < ColumnCount )
				{
					fputc ( ' ', ReportOptions->fpOutput );
				}
			}
			rptNewLine ( ReportOptions );
			break;

		case RPT_FORMAT_CSV:
			for ( xc = 0; xc < ColumnCount; xc++ )
			{	
				if ( (ColumnArray[xc].Options & COLUMN_TOTAL) == COLUMN_TOTAL )
				{
					DataType = ColumnArray[xc].Options & (COLUMN_LONG|COLUMN_DOUBLE);
					if ( DataType == COLUMN_LONG)
					{
						fprintf ( ReportOptions->fpOutput, "%ld", ColumnArray[xc].Total.longTotal[Level] );
					}
					else
					{
						fprintf ( ReportOptions->fpOutput, "%.2f", ColumnArray[xc].Total.doubleTotal[Level] );
					}
				}
				if ( xc + 1 < ColumnCount )
				{
					fprintf ( ReportOptions->fpOutput, "," );
				}
			}
			rptNewLine ( ReportOptions );
			break;

		case RPT_FORMAT_HTML:
		case RPT_FORMAT_EMAIL:
			fprintf ( ReportOptions->fpOutput, "<tr>\n" );
			if ( FirstColumnWithTotals == 1 )
			{
				fprintf ( ReportOptions->fpOutput, "<td>%s</td>", Level == 0 ? "SUB-TOTAL" : "GRAND-TOTAL" );
			}
			else
			{
				fprintf ( ReportOptions->fpOutput, "<td colspan='%d'>%s</td>", FirstColumnWithTotals, Level == 0 ? "SUB-TOTAL" : "GRAND-TOTAL" );
			}
			for ( xc = FirstColumnWithTotals; xc < ColumnCount; xc++ )
			{	
				if ( (ColumnArray[xc].Options & COLUMN_TOTAL) != COLUMN_TOTAL )
				{
					fprintf ( ReportOptions->fpOutput, "<td>&nbsp;</td>" );
					continue;
				}
				Align    = ColumnArray[xc].Options & (COLUMN_LEFT|COLUMN_CENTER|COLUMN_RIGHT);
				DataType = ColumnArray[xc].Options & (COLUMN_LONG|COLUMN_DOUBLE);
				switch ( Align )
				{
					case COLUMN_LEFT:
						if ( DataType == COLUMN_LONG)
						{
							fprintf ( ReportOptions->fpOutput, "<td>%ld</td>", ColumnArray[xc].Total.longTotal[Level] );
						}
						else
						{
							fprintf ( ReportOptions->fpOutput, "<td>%.2f</td>", ColumnArray[xc].Total.doubleTotal[Level] );
						}
						break;
					case COLUMN_CENTER:
						if ( DataType == COLUMN_LONG)
						{
							fprintf ( ReportOptions->fpOutput, "<td align='center'>%ld</td>", ColumnArray[xc].Total.longTotal[Level] );
						}
						else
						{
							fprintf ( ReportOptions->fpOutput, "<td align='center'>%.2f</td>", ColumnArray[xc].Total.doubleTotal[Level] );
						}
						break;
					case COLUMN_RIGHT:
						if ( DataType == COLUMN_LONG)
						{
							fprintf ( ReportOptions->fpOutput, "<td align='right'>%ld</td>", ColumnArray[xc].Total.longTotal[Level] );
						}
						else
						{
							fprintf ( ReportOptions->fpOutput, "<td align='right'>%.2f</td>", ColumnArray[xc].Total.doubleTotal[Level] );
						}
						break;
				}
			}
			rptNewLine ( ReportOptions );
			break;

		case RPT_FORMAT_EXCEL:
			if ( FirstColumnWithTotals > 0 )
			{
				if ( Level == 0 )
				{
					worksheet_write_string ( worksheet, ReportOptions->CurrentLineNumber, 0, "SUB-TOTAL", NULL );
				}
				else
				{
					worksheet_write_string ( worksheet, ReportOptions->CurrentLineNumber, 0, "GRAND-TOTAL", NULL );
				}
			}
			for ( xc = 0; xc < ColumnCount; xc++ )
			{	
				if ( (ColumnArray[xc].Options & COLUMN_TOTAL) == COLUMN_TOTAL )
				{
					DataType = ColumnArray[xc].Options & (COLUMN_LONG|COLUMN_DOUBLE);
					if ( DataType == COLUMN_LONG)
					{
						//fprintf ( ReportOptions->fpOutput, "%ld", ColumnArray[xc].Total.longTotal[Level] );
						worksheet_write_number ( worksheet, ReportOptions->CurrentLineNumber, xc, ColumnArray[xc].Total.longTotal[Level], NULL );
					}
					else
					{
						//fprintf ( ReportOptions->fpOutput, "%.2f", ColumnArray[xc].Total.doubleTotal[Level] );
						worksheet_write_number ( worksheet, ReportOptions->CurrentLineNumber, xc, ColumnArray[xc].Total.doubleTotal[Level], NULL );
					}
				}
			}
			rptNewLine ( ReportOptions );
			break;
	}


	for ( xc = 0; xc < ColumnCount; xc++ )
	{	
		ColumnArray[xc].Total.longTotal[Level] = 0L;
		ColumnArray[xc].Total.doubleTotal[Level] = 0.0;
	}
}
