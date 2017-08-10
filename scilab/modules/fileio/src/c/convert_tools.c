/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
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
#include "convert_tools.h"

void swap_generic(char* _in, char* _out, int _len)
{
    int i = 0;
    for (i = 0 ; i < _len ; i++)
    {
        _out[i] = _in[(_len - 1) - i];
    }
}
/*--------------------------------------------------------------------------*/
short swap_short(short _val)
{
    short res = 0;
    swap_generic((char*)&_val, (char*)&res, 2);
    return res;
}
/*--------------------------------------------------------------------------*/
int swap_int(int _val)
{
    int res = 0;
    swap_generic((char*)&_val, (char*)&res, sizeof(int));
    return res;
}
/*--------------------------------------------------------------------------*/
long long swap_long_long(long long _val)
{
    long long res = 0;
    swap_generic((char*)&_val, (char*)&res, sizeof(long long));
    return res;
}
/*--------------------------------------------------------------------------*/
char swap_char(char _val)
{
    return _val;
}
/*--------------------------------------------------------------------------*/
int writeInt(int _val, FILE* _pF, int _iEndian)
{
    //reverse _val bytes and write
    int newVal = _val;
    if (_iEndian == BIG_ENDIAN)
    {
        newVal = swap_int(_val);
    }

    return fwrite(&newVal, sizeof(int), 1, _pF) != 1 ? -1 : 0;
}
/*--------------------------------------------------------------------------*/
int writeLongLong(long long _val, FILE* _pF, int _iEndian)
{
    //reverse _val bytes and write
    long long newVal = _val;
    if (_iEndian == BIG_ENDIAN)
    {
        newVal = swap_long_long(_val);
    }

    return fwrite(&newVal, sizeof(long long), 1, _pF) != 1 ? -1 : 0;
}
/*--------------------------------------------------------------------------*/
int writeShort(short _val, FILE* _pF, int _iEndian)
{
    //reverse _val bytes and write
    short newVal = _val;
    if (_iEndian == BIG_ENDIAN)
    {
        newVal = swap_short(_val);
    }

    return fwrite(&newVal, sizeof(short), 1, _pF) != 1 ? -1 : 0;
}
/*--------------------------------------------------------------------------*/
int writeChar(char _val, FILE* _pF, int _iEndian)
{
    return fwrite(&_val, sizeof(char), 1, _pF) != 1 ? -1 : 0;
}
/*--------------------------------------------------------------------------*/
int readInt(FILE* _pF, int _iEndian, unsigned int* val)
{
    if (fread(val, sizeof(int), 1, _pF) != 1)
    {
        return -1;
    }

    if (_iEndian == BIG_ENDIAN)
    {
        *val = swap_int(*val);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int readLongLong(FILE* _pF, int _iEndian, unsigned long long* val)
{
    if (fread(val, sizeof(long long), 1, _pF) != 1)
    {
        return -1;
    }

    if (_iEndian == BIG_ENDIAN)
    {
        *val = swap_long_long(*val);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int readShort(FILE* _pF, int _iEndian, unsigned short* val)
{
    if (fread(val, sizeof(short), 1, _pF) != 1)
    {
        return -1;
    }

    if (_iEndian == BIG_ENDIAN)
    {
        *val = swap_short(*val);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int readChar(FILE* _pF, int _iEndian, unsigned char* val)
{
    if (fread(val, sizeof(char), 1, _pF) != 1)
    {
        return -1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int checkType(char _type)
{
    if (_type == 'l')
    {
        return TYPE_LONG_LONG;
    }

    if (_type == 'i')
    {
        return TYPE_INT;
    }

    if (_type == 's')
    {
        return TYPE_SHORT;
    }

    if (_type == 'c')
    {
        return TYPE_CHAR;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int checkEndian(char _endian)
{
    if (_endian == 'b')
    {
        return BIG_ENDIAN;
    }

    if (_endian == 'l')
    {
        return LITTLE_ENDIAN;
    }

    return 0;
}
