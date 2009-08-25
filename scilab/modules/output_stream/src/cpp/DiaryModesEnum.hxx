/*--------------------------------------------------------------------------*/ 
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#ifndef __DIARYMODESENUM_HXX__
#define __DIARYMODESENUM_HXX__

typedef enum { 
	PREFIX_TIME_FORMAT_ERROR = -1,
	PREFIX_TIME_FORMAT_UNIX_EPOCH = 0,
	PREFIX_TIME_FORMAT_ISO_8601 = 1
} diary_prefix_time_format;

typedef enum { 
	PREFIX_FILTER_ERROR = -1,
	PREFIX_FILTER_INPUT_AND_OUTPUT = 0,
	PREFIX_FILTER_ONLY_INPUT = 1,
	PREFIX_FILTER_ONLY_OUTPUT = 2,
	PREFIX_FILTER_NONE = 3
} diary_prefix_time_filter;

typedef enum { 
	DIARY_FILTER_ERROR = -1,
	DIARY_FILTER_INPUT_AND_OUTPUT = 0,
	DIARY_FILTER_ONLY_INPUT = 1,
	DIARY_FILTER_ONLY_OUTPUT = 2,
} diary_filter;

#endif /* __DIARYMODESENUM_HXX__ */
/*--------------------------------------------------------------------------*/ 
