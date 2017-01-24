/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) - 2013 - Scilab Enterprises - Antoine ELIAS
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

#include <stdio.h>
#include "dynlib_fileio.h"

#ifndef __CONVERT_TOOLS_H__
#define __CONVERT_TOOLS_H__

#define SIGNED          -1
#define UNSIGNED        1

#ifdef LITTLE_ENDIAN
#undef LITTLE_ENDIAN
#endif
#define LITTLE_ENDIAN   1
#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif
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

FILEIO_IMPEXP int writeInt(int _val, FILE* _pF, int _iEndian);
FILEIO_IMPEXP int writeLongLong(long long _val, FILE* _pF, int _iEndian);
FILEIO_IMPEXP int writeShort(short _val, FILE* _pF, int _iEndian);
FILEIO_IMPEXP int writeChar(char _val, FILE* _pF, int _iEndian);

FILEIO_IMPEXP int readInt(FILE* _pF, int _iEndian, unsigned int* val);
FILEIO_IMPEXP int readLongLong(FILE* _pF, int _iEndian, unsigned long long* val);
FILEIO_IMPEXP int readShort(FILE* _pF, int _iEndian, unsigned short* val);
FILEIO_IMPEXP int readChar(FILE* _pF, int _iEndian, unsigned char* val);

FILEIO_IMPEXP int checkEndian(char _endian);
FILEIO_IMPEXP int checkType(char _type);

#endif /* !__CONVERT_TOOLS_H__ */
