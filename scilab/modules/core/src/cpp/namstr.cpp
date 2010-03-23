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
#include "machine.h" // C2F
#include "stack-def.h" // nsiz, nlgh

extern "C" 
{
	void C2F(namstr)(int* id, int* str, int* n, char* job);
}

namespace
{
	int const blank=40; /* @TOD: What is 40 */
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
	if(*job)  // id -> str
	{

		unsigned int i1(0);
	
		for(unsigned int j= 0; j != nsiz; ++j, i1 += 4) /* @TODO: add comment */
		{
			int id_j(id[j]);
			for(unsigned int i(i1); i!=i1+4; ++i, id_j >>= 8) /* @TODO: add comment */
			{
				int ch((id_j & 0x80) ? (id_j |0xffffff00) :  (id_j & 0xff));
/* @TODO: add comment: what is 0x80 0xffffff00 0xff */
				if(ch == blank) 
				{
					*n = i;
					return;
				}
				id_j += 128;
				str[i]= ch;
			}
		}

		*n= nlgh;

	}
	else
	{ // str -> id
		// n (<= nsiz*4 = nlgh) int in str packed into id
		unsigned int j;
		unsigned const int full_ids((*n)>>2); /* @TODO: add comment: 2 ?*/

		for(j= 0; j!=full_ids; ++j) 
		{
			id[j]= str[4*j+0] +(str[4*j+1]<<8)+(str[4*j+2]<<16)+(str[4*j+3]<<24); /* @TODO: add comment */
		}
		if(j!= nsiz) 
		{
			switch((*n)&0x3)  /* @TODO: add comment */ 
			{
				case 3: 
				{
					id[j]= one_blank + (str[4*j+2] << 16) + (str[4*j+1] << 8) + str[4*j + 0]; /* @TODO: add comment */
					++j;
					break;
				}
				case 2: 
				{
					id[j]= two_blanks + (str[4*j+1]<<8) + str[4*j+0]; /* @TODO: add comment */
					++j;
					break;
				}
				case 1: 
				{
					id[j]= three_blanks + str[4*j+0]; /* @TODO: add comment */
					++j;
					break;
				}
			}

			for(;j != nsiz; ++j)  /* @TODO: add comment */
			{
				id[j]= four_blanks;      
			}
		}
	}
	return;
}
