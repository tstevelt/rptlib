/*----------------------------------------------------------------------------
	Program : rptinit.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Initialize the report system.
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

#define		RPT_MAIN
#include	"rptprivate.h"
#include	"rptlib.h"

FILE *rptinit ( char *DataFilename, REPORT_OPTIONS *ReportOptions, COLUMN_HEADINGS ColumnArray[], int ColumnCount )
{
	char	xbuffer[MAXRPTLINELENGTH];
	char	*tokens[MAXFIELDS];
	int		tokcnt, xt, xc, xl;
	int		lineno, HowManyLines;
	FILE	*fpData;
	int		OpenOkay;
	int		Align;

	switch ( ReportOptions->Format )
	{
		case RPT_FORMAT_TEXT:
		case RPT_FORMAT_PDF_VIEW:
		case RPT_FORMAT_PDF_EMAIL:
			HowManyLines = 100;
			break;
		case RPT_FORMAT_CSV:
		case RPT_FORMAT_HTML:
		case RPT_FORMAT_EMAIL:
		case RPT_FORMAT_EXCEL:
			HowManyLines = 10;
			ReportOptions->LinesPerPage = 0;
			break;
	}

	for ( xc = 0; xc < ColumnCount; xc++ )
	{
		ColumnArray[xc].Width = 0;
	}

	if ( ReportOptions->HeaderRows > 0 )
	{
		for ( xc = 0; xc < ColumnCount; xc++ )
		{
			xl = nsStrlen ( ColumnArray[xc].Row1 );
			if ( ColumnArray[xc].Width < xl )
			{
				ColumnArray[xc].Width = xl;
			}
			if ( ReportOptions->HeaderRows == 2 )
			{
				xl = nsStrlen ( ColumnArray[xc].Row2 );
				if ( ColumnArray[xc].Width < xl )
				{
					ColumnArray[xc].Width = xl;
				}
			}
		}
	}

	if (( fpData = fopen ( DataFilename, "r" )) == (FILE *) 0 )
	{
		fprintf ( stderr, "rptinit: Cannot open %s for reading\n", DataFilename );
		exit ( 1 );
	}


	lineno = 0;
	while ( lineno < HowManyLines && fgets ( xbuffer, sizeof(xbuffer), fpData ) != (char *)0 )
	{
		lineno++;

		// tms 04/13/2023 nutrition has commas in food names. 
		// tokcnt = GetTokensA ( xbuffer, "|,\t\n\r", tokens, MAXFIELDS );
		tokcnt =    GetTokensA ( xbuffer, "|\t\n\r", tokens, MAXFIELDS );

		for ( xc = 0, xt = ReportOptions->BreakCount; xt < tokcnt; xc++, xt++ )
		{
			xl = nsStrlen ( tokens[xt] );
			if ( ColumnArray[xc].Width < xl )
			{
				ColumnArray[xc].Width = xl;
			}
		}
	}

	rewind ( fpData );
	
	/*----------------------------------------------------------
		if TEXT or PDF, add 2 to each column which totals
	----------------------------------------------------------*/
	switch ( ReportOptions->Format )
	{
		case RPT_FORMAT_TEXT:
		case RPT_FORMAT_PDF_VIEW:
		case RPT_FORMAT_PDF_EMAIL:
			for ( xc = 0; xc < ColumnCount; xc++ )
			{
				if ( (ColumnArray[xc].Options & COLUMN_TOTAL) == COLUMN_TOTAL )
				{
					ColumnArray[xc].Width += 2;
				}
			}
			break;
	}

	ReportOptions->TotalWidth = 0;
	for ( xc = 0; xc < ColumnCount; xc++ )
	{
		ReportOptions->TotalWidth += ColumnArray[xc].Width;
		switch ( ReportOptions->Format )
		{
			case RPT_FORMAT_TEXT:
			case RPT_FORMAT_PDF_VIEW:
			case RPT_FORMAT_PDF_EMAIL:
				if ( xc + 1 < ColumnCount )
				{
					ReportOptions->TotalWidth++;
				}
				break;
		}
	}

	ReportOptions->TitleWidth = lmax ( nsStrlen(ReportOptions->Title), nsStrlen(ReportOptions->Subtitle) );

	OpenOkay = 1;
	if (( nsStrlen(ReportOptions->OutputFilename) == 0              ) ||
		( nsStrcmp ( ReportOptions->OutputFilename, "stdout" ) == 0 ))
	{
		ReportOptions->fpOutput = stdout;
	}
	else if ( ReportOptions->Format == RPT_FORMAT_EXCEL )
	{
		workbook = workbook_new ( ReportOptions->OutputFilename );
		if ( workbook == (lxw_workbook *)0 )
		{
			fprintf ( stderr, "rptinit: workbook_new returned NULL %s for output\n", ReportOptions->OutputFilename );
			OpenOkay = 0;
		}
	}
	else if (( ReportOptions->fpOutput = fopen ( ReportOptions->OutputFilename, "w" )) == (FILE *) 0 )
	{
		fprintf ( stderr, "rptinit: Cannot open %s for output\n", ReportOptions->OutputFilename );
		OpenOkay = 0;
	}

	if ( OpenOkay )
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
				fprintf ( ReportOptions->fpOutput, "<table width='98%%'>\n" );
				break;
			case RPT_FORMAT_EXCEL:
				worksheet = workbook_add_worksheet ( workbook, NULL );
				for ( xc = 0; xc < ColumnCount; xc++ )
				{
					worksheet_set_column ( worksheet, xc, xc, ColumnArray[xc].Width, NULL );

					ColumnArray[xc].HeadFormat = workbook_add_format ( workbook );
					format_set_pattern  ( ColumnArray[xc].HeadFormat, LXW_PATTERN_SOLID );
					format_set_bg_color ( ColumnArray[xc].HeadFormat, 0xFFD700 );
					Align = ColumnArray[xc].Options & (COLUMN_LEFT|COLUMN_CENTER|COLUMN_RIGHT);
					switch ( Align )
					{
						case COLUMN_LEFT:
							format_set_align ( ColumnArray[xc].HeadFormat, LXW_ALIGN_LEFT );
							break;
						case COLUMN_CENTER:
							format_set_align ( ColumnArray[xc].HeadFormat, LXW_ALIGN_CENTER );
							break;
						case COLUMN_RIGHT:
							format_set_align ( ColumnArray[xc].HeadFormat, LXW_ALIGN_RIGHT );
							break;
					}

					ColumnArray[xc].DataFormat = workbook_add_format ( workbook );
					Align = ColumnArray[xc].Options & (COLUMN_LEFT|COLUMN_CENTER|COLUMN_RIGHT);
					switch ( Align )
					{
						case COLUMN_LEFT:
							format_set_align ( ColumnArray[xc].DataFormat, LXW_ALIGN_LEFT );
							break;
						case COLUMN_CENTER:
							format_set_align ( ColumnArray[xc].DataFormat, LXW_ALIGN_CENTER );
							break;
						case COLUMN_RIGHT:
							format_set_align ( ColumnArray[xc].DataFormat, LXW_ALIGN_RIGHT );
							break;
					}
				}
				break;
		}
	}

	return ( fpData );
}
