/*----------------------------------------------------------------------------
	Program : rptdeliv.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Deliver report in selected format to selected output.
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

static	char	PDF_File[1024];
static	char	TempFile[1024];
static	int		DebugRptDeliver = 0;

static void MakePDF ( char *TextFile )
{
	int		xl;
	char	CommandLine[1024];

	xl = nsStrlen ( TextFile );
	if ( nsStrncmp ( &TextFile[xl-4], ".txt", 4 ) == 0 )
	{
		xl -= 4;
		sprintf ( TempFile, "%*.*s.ps", xl, xl, TextFile );
		sprintf ( PDF_File, "%*.*s.pdf", xl, xl, TextFile );
	}
	else
	{
		sprintf ( TempFile, "%s.ps", TextFile );
		sprintf ( PDF_File, "%s.pdf", TextFile );
	}

	snprintf ( CommandLine, sizeof(CommandLine), 
			"/usr/local/bin/txt2ps %s -of %s -f 1 -magic -ff", TextFile, TempFile );

	if ( DebugRptDeliver )
	{
		printf ( "%s<br>\n", CommandLine );
	}

	system ( CommandLine );

#ifdef UNIX
	snprintf ( CommandLine, sizeof(CommandLine), "/usr/bin/ps2pdf %s %s", TempFile, PDF_File );
#else
	snprintf ( CommandLine, sizeof(CommandLine), "/usr/local/bin/ps2pdf %s %s", TempFile, PDF_File );
#endif

	if ( DebugRptDeliver )
	{
		printf ( "%s<br>\n", CommandLine );
	}

    system ( CommandLine );
}

void rptdeliv ( char *FromAddr, char *ToAddr, char *Subject, char *BodyFile, char *AttachFile, char Format, int Silent )
{
	char	*AttachFileArray[2];

	switch ( Format )
	{
		case RPT_FORMAT_HTML:
			rptcat ( AttachFile );
			break;
		case RPT_FORMAT_EMAIL:
			AttachFileArray[0] = (char *)0;
			SendAttached ( FromAddr, ToAddr, "", "", Subject, 1, BodyFile, 0, AttachFileArray );
			if ( Silent == 0 )
			{
				printf ( "Sent to %s<br>\n", ToAddr );
			}
			break;
		case RPT_FORMAT_CSV:
		case RPT_FORMAT_EXCEL:
		case RPT_FORMAT_TEXT:
			AttachFileArray[0] = AttachFile;
			AttachFileArray[1] = (char *)0;
			SendAttached ( FromAddr, ToAddr, "", "", Subject, 0, BodyFile, 1, AttachFileArray );
			if ( Silent == 0 )
			{
				printf ( "Sent to %s<br>\n", ToAddr );
			}
			break;

		case RPT_FORMAT_PDF_EMAIL:
		case RPT_FORMAT_PDF_VIEW:
			MakePDF ( AttachFile );
			switch ( Format )
			{
				case RPT_FORMAT_PDF_EMAIL:
					AttachFileArray[0] = PDF_File;
					AttachFileArray[1] = (char *)0;
					SendAttached ( FromAddr, ToAddr, "", "", Subject, 0, BodyFile, 1, AttachFileArray );
					if ( Silent == 0 )
					{
						printf ( "Sent to %s<br>\n", ToAddr );
					}
					break;
				case RPT_FORMAT_PDF_VIEW:
					rptcat ( PDF_File );
					break;
			}
			unlink ( TempFile );
			unlink ( PDF_File );
			return;

		case RPT_FORMAT_APPEND_W:
		case RPT_FORMAT_REPLACE_W:
		case RPT_FORMAT_TEMP:
		default:
			if ( Silent == 0 )
			{
				printf ( "Format not supported<br>\n" );
			}
			return;
	}

}
