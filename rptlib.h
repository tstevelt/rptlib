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

/*------------------------------------------------------------------
	documentation: http://libxlsxwriter.github.io/index.html
------------------------------------------------------------------*/

#include    "xlsxwriter.h"

/*--------------------------------------------------------------------------------
	these can not be the same as any stock index defines in the 'invest' software
	what the hell does this mean?
--------------------------------------------------------------------------------*/
#define		RPT_FORMAT_CSV			'C'
#define		RPT_FORMAT_EXCEL		'X'
#define		RPT_FORMAT_TEXT			'T'
#define		RPT_FORMAT_PDF_VIEW		'V'
#define		RPT_FORMAT_PDF_EMAIL	'E'
#define		RPT_FORMAT_HTML			'H'
#define		RPT_FORMAT_EMAIL		'M'
#define		RPT_FORMAT_APPEND_W		'A'
#define		RPT_FORMAT_REPLACE_W	'R'
#define		RPT_FORMAT_TEMP			'Z'

#define		MAXRPTLINELENGTH	2048
#define		MAXFIELDS			100

typedef struct 
{
	int		HeaderRows;
	int		Format;
	char	*Title;
	char	*Subtitle;
	int		TitleWidth;
	int		TotalWidth;
	int		BreakCount;
	int		LinesPerPage;
	int		CurrentLineNumber;
	int		CurrentPageNumber;
	int		WritePageNumbers;

	/*----------------------------------------------------------
		set OutputFilename = stdout for output to stdout.
	----------------------------------------------------------*/
	char	OutputFilename[256];
	FILE	*fpOutput;
	int		IsStdout;

} REPORT_OPTIONS;

#define		COLUMN_STRING	1
#define		COLUMN_LONG		2
#define		COLUMN_DOUBLE	4
#define		COLUMN_TOTAL	8
#define		COLUMN_LEFT		256
#define		COLUMN_CENTER	512
#define		COLUMN_RIGHT	1024
#define		COLUMN_RED		65536
#define		COLUMN_GREEN	131072
#define		COLUMN_BLUE		262144
#define		COLUMN_YELLOW	524288
#define		COLUMN_VIOLET	1048576
#define		COLUMN_CYAN		2097152
#define		COLUMN_COLORS	(COLUMN_RED|COLUMN_GREEN|COLUMN_BLUE|COLUMN_YELLOW|COLUMN_VIOLET|COLUMN_CYAN)

#define		INIT_STRING_LEFT			(COLUMN_STRING | COLUMN_LEFT)
#define		INIT_STRING_CENTER			(COLUMN_STRING | COLUMN_CENTER)
#define		INIT_STRING_RIGHT			(COLUMN_STRING | COLUMN_RIGHT)
#define		INIT_LONG_LEFT				(COLUMN_LONG | COLUMN_LEFT)
#define		INIT_LONG_CENTER			(COLUMN_LONG | COLUMN_CENTER)
#define		INIT_LONG_RIGHT				(COLUMN_LONG | COLUMN_RIGHT)
#define		INIT_DOUBLE_LEFT			(COLUMN_DOUBLE | COLUMN_LEFT)
#define		INIT_DOUBLE_CENTER			(COLUMN_DOUBLE | COLUMN_CENTER)
#define		INIT_DOUBLE_RIGHT			(COLUMN_DOUBLE | COLUMN_RIGHT)
#define		INIT_LONG_LEFT_TOTAL		(COLUMN_LONG | COLUMN_LEFT | COLUMN_TOTAL)
#define		INIT_LONG_CENTER_TOTAL		(COLUMN_LONG | COLUMN_CENTER | COLUMN_TOTAL)
#define		INIT_LONG_RIGHT_TOTAL		(COLUMN_LONG | COLUMN_RIGHT | COLUMN_TOTAL)
#define		INIT_DOUBLE_LEFT_TOTAL		(COLUMN_DOUBLE | COLUMN_LEFT | COLUMN_TOTAL)
#define		INIT_DOUBLE_CENTER_TOTAL	(COLUMN_DOUBLE | COLUMN_CENTER | COLUMN_TOTAL)
#define		INIT_DOUBLE_RIGHT_TOTAL		(COLUMN_DOUBLE | COLUMN_RIGHT | COLUMN_TOTAL)

typedef union
{
	long	longTotal[2];
	double	doubleTotal[2];
} UTOTAL;

typedef struct
{
	char	*Row1;
	char	*Row2;
	unsigned long	Options;
	int		Width;
	UTOTAL	Total;
	lxw_format	*HeadFormat;
	lxw_format	*DataFormat;
} COLUMN_HEADINGS;

/*----------------------------
:.,$d
:r ! mkproto -p *.c
----------------------------*/


/* ValidReportFormat.c */
char ValidReportFormat ( char *FormatArg );

/* rptcat.c */
void rptcat ( char *FileName );

/* rptclose.c */
void rptclose ( REPORT_OPTIONS *ReportOptions );

/* rptdeliv.c */
void rptdeliv ( char *FromAddr , char *ToAddr , char *Subject , char *BodyFile, char *AttachFile , char Format, int Silent );

/* rptextension.c */
char *rptextension ( int Format );

/* rptfuncs.c */
void rptNewLine ( REPORT_OPTIONS *ReportOptions );
void PrintColumnBgcolor ( REPORT_OPTIONS *ReportOptions , COLUMN_HEADINGS *Column );
void rptPaintFormat ( char *Label , char ReportFormat );
int CountColumnsWithTotals ( int *ColumnsWithTotals, int *FirstColumnWithTotals, COLUMN_HEADINGS ColumnArray[], int ColumnCount );

/* rptheader.c */
void rptheader ( REPORT_OPTIONS *ReportOptions , COLUMN_HEADINGS ColumnArray [], int ColumnCount );

/* rptinit.c */
FILE *rptinit ( char *DataFilename , REPORT_OPTIONS *ReportOptions , COLUMN_HEADINGS ColumnArray [], int ColumnCount );

/* rptline.c */
void rptline ( REPORT_OPTIONS *ReportOptions , COLUMN_HEADINGS ColumnArray [], int ColumnCount , char *tokens [], int tokcnt );

/* rpttotal.c */
void rpttotal ( REPORT_OPTIONS *ReportOptions , COLUMN_HEADINGS ColumnArray [], int ColumnCount, int Level );
