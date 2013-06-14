/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Sylvestre LEDRU
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* getenv */

#include "call_scilab.h" /* Provide functions to call Scilab engine */

/*------------------------------------------------------------*/
int main(void)
{
#ifdef _MSC_VER
    if ( StartScilab(NULL, NULL, NULL) == FALSE )
#else
    if ( StartScilab(getenv("SCI"), NULL, NULL) == FALSE )
#endif
    {
        fprintf(stderr, "Error while calling StartScilab\n");
        return -1;
    }

    DisableInteractiveMode();
    int code = SendScilabJob("plot3d()"); /* This will failed since plot3d is
										   among the disable features*/
    if (code != 0)
    {
        char lastjob[4096];
        if (GetLastJob(lastjob, 4096))
        {
            printf("Failed command: %s\n", lastjob);
        }
    }

    if ( TerminateScilab(NULL) == FALSE )
    {
        fprintf(stderr, "Error while calling TerminateScilab\n");
        return -2;
    }
    return 0;
}
/*------------------------------------------------------------*/
