/*----------------------------------------------------------------------------
	Program : rptcat.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Read file and write to stdout.  This is used to send reports
				to web screen if called from CGI programs.
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

void rptcat ( char *FileName )
{
	FILE	*fp;
	char	xbuffer[1024];
	int		nr;

	if (( fp = fopen ( FileName, "r" )) == (FILE *)0 )
	{
		return;
	}

	while (( nr = fread ( xbuffer, 1, 1024, fp )) > 0 )
	{
		fwrite ( xbuffer, 1, nr, stdout );
	}

	nsFclose ( fp );
}
