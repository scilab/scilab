/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <algorithm>
#include "machine.h" /* C2F */
#include "stack-def.h" /* nsiz, nlgh */

extern "C"
{
    void C2F(namstr)(int* id, int* str, int* n, char* job);
}

namespace
{
    int const blank=40; /* a scilab encoded blank character: used for padding id when name has < nsiz*6 characters*/
/*  constant below are used for fast padding 1->4 blanks at once */
    int const one_blank= (blank<<24);
    int const two_blanks= one_blank + (blank<<16);
    int const three_blanks= two_blanks + (blank<<8);
    int const four_blanks= three_blanks + blank;
}

/*
 * convert id (array of nsiz ints) <-> str (array of n <= nlgh=4*nsiz ints)
 * each element of str is stored in 8 bits of an id. The reming bytes (after n)
 * are filled with blanks.
 *
 *
 * @param  id contains the id of nsiz ints
 * @param  str contains the string of nlgh ints
 * @param  n contains the address of the (int) length of the string input for str->id, output for id->str
 * @param job 0 => str->id, id-> str otherwise
 *
 * TODO: now that function search has been ported to C++, should expose a saner API that would not require strlen call nor a job arg.
 */
void C2F(namstr)(int* id,int* str, int* n, char* job)
{
    if (*job)  /* id -> str */
    {

        unsigned int i1(0); /* starting character from constructed str when processing an int from id */

        for (unsigned int j= 0; j != nsiz; ++j, i1 += 4) /* processing id[j] into str[i1...ii+4] */
        {
            int id_j(id[j]);/* caching id[j] because we could not tell the compiler that id and str do not alias */
            for (unsigned int i(i1); i!=i1+4; ++i, id_j >>= 8)/* processing each byte from id[j] into an int in str*/
            {
                /* extracting the *signed* lower byte: & 0x80 tests the 7th bit (sign bit of the byte)
                 * |0xffffff00 : lower <0, byte & 0xff : lower >0 byte */
                int ch((id_j & 0x80) ? (id_j |0xffffff00) :  (id_j & 0xff));
                if (ch == blank)/* we encounter a blank -> end of string. */
                {
                    *n = i;
                    return;
                }
                id_j += 128;/* propagate sign bit to upper bits */
                str[i]= ch;
            }
        }

        *n= nlgh;/* no blank encountered str length is nlgh= nsiz*8 */

    }
    else
    { /* str -> id */
        /* n (<= nsiz*4 = nlgh) int in str packed into id */
        unsigned int j;
        /* a full id contains 4 ints from str, so we have *n/4 full ids the remaing are padded with blanks
         we can get *n > nlgh (we  truncate @ nsiz ids) */
#ifndef _MSC_VER
        unsigned const int full_ids(std::min(*n/4, nsiz));
#else
        unsigned const int full_ids(min(*n/4, nsiz));
#endif

        for (j= 0; j!=full_ids; ++j)
        { /* str int are signed bytes in fact, we pack them using shifts */
            id[j]= str[4*j+0] +(str[4*j+1]<<8)+(str[4*j+2]<<16)+(str[4*j+3]<<24);
        }
        if (j!= nsiz)/* we had < nsiz int in str : padd with blank chars */
        {
            switch ((*n ) & 0x3)  /* pad partial id int according modulo 4= nb of remaining ints in str */
            {
            case 3: /* three remaining ints and one blank */
            {
                id[j]= one_blank + (str[4*j+2] << 16) + (str[4*j+1] << 8) + str[4*j + 0];
                ++j;
                break;
            }
            case 2: /* two remaining ints and two blanks */
            {
                id[j]= two_blanks + (str[4*j+1]<<8) + str[4*j+0];
                ++j;
                break;
            }
            case 1:/* one remaining int and three blanks*/
            {
                id[j]= three_blanks + str[4*j+0];
                ++j;
                break;
            }
            }

            for (;j != nsiz; ++j) /* pad empty ints in id with four blanks at once */
            {
                id[j]= four_blanks;
            }
        }
    }
    return;
}
