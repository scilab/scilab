/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2016 - Scilab Enterprises - Cedric Delamarre
* Copyright (C) 2012 - 2016 - Scilab Enterprises
*
* This file is hereby licensed under the terms of the GNU GPL v2.0,
* pursuant to article 5.3.4 of the CeCILL v.2.1.
* This file was originally licensed under the terms of the CeCILL v2.1,
* and continues to be available under such terms.
* For more information, see the COPYING file which you should have received
* along with this program.
* === LICENSE_END ===
*
*/
#include <stdio.h>
#include "isatty.hxx"
#include "os_string.h"
#include "machine.h" // bsiz
#include "getpipeline.h"

char* getPipeLine(void)
{
    int len_line = 0;
    char buffer[bsiz];

    //read from stdin
    int eof = (fgets(buffer, bsiz, stdin) == NULL);
    if (eof)
    {
        //send command to quit to Scilab
        return os_strdup("quit");
    }

    //remove trailing \n
    len_line = (int)strlen(buffer);
    if (buffer[len_line - 1] == '\n')
    {
        buffer[len_line - 1] = '\0';
    }

    return os_strdup(buffer);
}
