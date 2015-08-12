/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) - 2013 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <stdio.h>

#ifndef __CONVERT_TOOLS_H__
#define __CONVERT_TOOLS_H__

#define SIGNED          -1
#define UNSIGNED        1

#define LITTLE_ENDIAN   1
#define BIG_ENDIAN      -1

#define TYPE_LONG_LONG  8
#define TYPE_INT       4
#define TYPE_SHORT      2
#define TYPE_CHAR       1

void swap_generic(char* _in, char* _out, int _len);
short swap_short(short _val);
int swap_int(int _val);
char swap_char(char _val);
long long swap_long_long(long long _val);

int writeInt(int _val, FILE* _pF, int _iEndian);
int writeLongLong(long long _val, FILE* _pF, int _iEndian);
int writeShort(short _val, FILE* _pF, int _iEndian);
int writeChar(char _val, FILE* _pF, int _iEndian);

int readInt(FILE* _pF, int _iEndian, unsigned int* val);
int readLongLong(FILE* _pF, int _iEndian, unsigned long long* val);
int readShort(FILE* _pF, int _iEndian, unsigned short* val);
int readChar(FILE* _pF, int _iEndian, unsigned char* val);

int checkEndian(char _endian);
int checkType(char _type);

#endif /* !__CONVERT_TOOLS_H__ */