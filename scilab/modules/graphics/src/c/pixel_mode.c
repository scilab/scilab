/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*------------------------------------------------------------------------*/
/* file: pixel_mode.c                                                     */
/* desc : make the correspondence between pixel mode name in scilab       */
/*        and the corresponding index                                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "pixel_mode.h"

/**
* number of different pixel drawing modes.
*/
#define NB_PIXEL_MODES 16

/**
 * the array of correspondence
 */
static char * pmodes[NB_PIXEL_MODES] = { "clear"       , /* 0 */
                                         "and"         , /* 1 */
                                         "andReverse"  , /* 2 */
                                         "copy"        , /* 3 */
                                         "andInverted" , /* 4 */
                                         "noop"        , /* 5 */
                                         "xor"         , /* 6 */
                                         "or"          , /* 7 */
                                         "nor"         , /* 8 */
                                         "equiv"       , /* 9 */
                                         "invert"      , /* 10 */
                                         "orReverse"   , /* 11 */
                                         "copyInverted", /* 12 */
                                         "orInverted"  , /* 13 */
                                         "nand"        , /* 14 */
                                         "set"         , /* 15 */
                                       };

/*----------------------------------------------------------------------------------*/
char * getPixelMode(int ind)
{
    return pmodes[ind];
}
/*----------------------------------------------------------------------------------*/
int getPixelModeIndex(const char * modeName)
{
    /* I'm lazy. I won't use a hash table. Sorry if you are adding a new mode. */
    /* But I don't think it will happen often */

    int nameLen = 0;

    nameLen =  (int)strlen(modeName);

    switch (modeName[0])
    {
        case 'a' :
        {
            /* check input is longer than the shorter string */
            if (nameLen < 3)
            {
                break ;
            }
            switch (modeName[3])
            {
                case 0 : /* and */
                    return 1;
                    break;
                case 'R' : /* andReverse */
                    return 2;
                    break;
                case 'I' : /* andInverted */
                    return 4;
                    break;
            }
            break;
        }
        case 'c':
        {
            if (nameLen < 4)
            {
                break ;
            }
            switch (modeName[4])
            {
                case 'r' : /* clear */
                    return 0;
                    break;
                case 0 : /* copy */
                    return 3;
                    break;
                case 'I' : /* copyInverted */
                    return 12;
                    break;
            }
            break;
        }
        case 'e':
        {
            if (nameLen < 5)
            {
                break ;
            }
            return 9 ; /* equiv */
            break;
        }
        case 'i':
        {
            if (nameLen < 6)
            {
                break ;
            }
            return 10 ; /* invert */
            break;
        }
        case 'n':
        {
            if (nameLen < 3)
            {
                break ;
            }
            switch (modeName[2])
            {
                case 'o' : /* noop */
                    return 5;
                    break;
                case 'r' : /* nor */
                    return 8;
                    break;
                case 'n' : /* nand */
                    return 14;
                    break;
            }
            break;
        }
        case 'o':
        {
            if (nameLen < 2)
            {
                break ;
            }
            switch (modeName[2])
            {
                case 0 : /* or */
                    return 7;
                    break;
                case 'R' : /* orReverse */
                    return 11;
                    break;
                case 'I' : /* orInverted */
                    return 13;
                    break;
            }
            break;
        }
        case 's':
        {
            if (nameLen < 3)
            {
                break ;
            }
            return 15 ; /* set */
            break;
        }
        case 'x':
        {
            if (nameLen < 3)
            {
                break ;
            }
            return 6 ; /* xor */
            break;
        }
    }
    return -1;
}
/*----------------------------------------------------------------------------------*/
int getNbPixelModes(void)
{
    return NB_PIXEL_MODES;
}
/*----------------------------------------------------------------------------------*/
