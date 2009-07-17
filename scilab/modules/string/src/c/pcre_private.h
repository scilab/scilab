
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#ifndef __PCRE_PRIVATE_H__
#define __PCRE_PRIVATE_H__

typedef enum {
	PCRE_EXIT = 1,
	PCRE_FINISHED_OK = 0,
	NO_MATCH = -1,
	NOT_ENOUGH_MEMORY_FOR_VECTOR = -2,
	DELIMITER_NOT_ALPHANUMERIC = -3,
	CAPTURING_SUBPATTERNS_ERROR = -4,
	PARTIAL_MATCHING_NOT_SUPPORTED = -5,
	CONTAINS_EXPLICIT_CR_OR_LF_MATCH = -6,
	DUPLICATE_NAME_STATUS_CHANGES = -7,
	TOO_BIG_FOR_OFFSET_SIZE = -8,
	LIMIT_NOT_RELEVANT_FOR_DFA_MATCHING = -9,
	CAN_NOT_COMPILE_PATTERN=-10,
	MATCH_LIMIT = -11,
	UTF8_NOT_SUPPORTED = -12
} pcre_error_code;


pcre_error_code pcre_private(char *INPUT_LINE,char *INPUT_PAT,int *Output_Start,int *Output_End);

#endif /* __PCRE_PRIVATE_H__ */
/*------------------------------------------------------------------------*/
