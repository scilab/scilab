
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#include "setScilabCharactersCodes.h"
#include "stack-def.h"
/*--------------------------------------------------------------------------*/ 
void setScilabCharactersCodes(void)
{
	static char alpha[csiz+1] ={ 
		"0" "1" "2" "3" "4" "5" "6" "7" "8" "9"
		"a" "b" "c" "d" "e" "f" "g" "h" "i" "j"
		"k" "l" "m" "n" "o" "p" "q" "r" "s" "t"
		"u" "v" "w" "x" "y" "z" "_" "#" "!" "$"
		" " "(" ")" ";" ":" "+" "-" "*" "/" "\\"
		"=" "." "," "'" "[" "]" "%" "|" "&" "<"
		">" "~" "^"};
	static char alphb[csiz+1] ={ 
		"0" "1" "2" "3" "4" "5" "6" "7" "8" "9"
		"A" "B" "C" "D" "E" "F" "G" "H" "I" "J"
		"K" "L"	"M" "N" "O" "P" "Q" "R" "S" "T"
		"U" "V" "W"	"X" "Y" "Z" "0" "0" "?" "0"
		"\t" "0" "0" "0" "0" "0" "0" "0" "0" "$"
		"0" "0" "0" "\"" "{" "}" "0" "0" "0" "`"
		"0" "@" "0"};

	int i = 0;
	for (i = 0; i < csiz; i++) 
	{
		*(unsigned char *)&C2F(cha1).alfa[i] = *(unsigned char *)&alpha[i];
		*(unsigned char *)&C2F(cha1).alfb[i] = *(unsigned char *)&alphb[i];
	}
}
/*--------------------------------------------------------------------------*/ 
