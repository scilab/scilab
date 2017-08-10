/*--------------------------------------------------------------------------*/
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#ifndef __DIARYMODESENUM_HXX__
#define __DIARYMODESENUM_HXX__

typedef enum
{
    PREFIX_TIME_FORMAT_ERROR = -1,
    PREFIX_TIME_FORMAT_UNIX_EPOCH = 0,
    PREFIX_TIME_FORMAT_ISO_8601 = 1
} diary_prefix_time_format;

typedef enum
{
    PREFIX_FILTER_ERROR = -1,
    PREFIX_FILTER_INPUT_AND_OUTPUT = 0,
    PREFIX_FILTER_ONLY_INPUT = 1,
    PREFIX_FILTER_ONLY_OUTPUT = 2,
    PREFIX_FILTER_NONE = 3
} diary_prefix_time_filter;

typedef enum
{
    DIARY_FILTER_ERROR = -1,
    DIARY_FILTER_INPUT_AND_OUTPUT = 0,
    DIARY_FILTER_ONLY_INPUT = 1,
    DIARY_FILTER_ONLY_OUTPUT = 2,
} diary_filter;

#endif /* __DIARYMODESENUM_HXX__ */
/*--------------------------------------------------------------------------*/
