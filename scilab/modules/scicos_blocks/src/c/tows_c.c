/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*  Copyright (C) Scilab Enterprises - 2013 - Paul Bignier
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* getenv */
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "scicos_print.h"
#include "cvstr.h"
#include "mopen.h"
#include "mput.h"
#include "mclose.h"
#include "expandPathVariable.h"
#include "scicos.h"
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "localization.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
//extern int C2F(namstr)(int* id, int* i, int* j, int* k);
/*--------------------------------------------------------------------------*/
/*YAPASDETROUDANSLESLISTESDANSLESFICHERS*/
/*ONLITPASPTR_I[8-9-10-11]*/
#define codehm \
	ptr_i[6]   = 17; \
	ptr_i[7]   = 3;  \
	ptr_i[8]   = 1;  \
	ptr_i[9]   = ptr_i[8]  + 11; \
	ptr_i[10]  = ptr_i[9]  + 4;  \
	ptr_i[11]  = ptr_i[10] + 2 + nz*nu*nu2; \
	ptr_i[12]  = 10; \
	ptr_i[13]  = 1; \
	ptr_i[14]  = 3; \
	ptr_i[15]  = 0; \
	ptr_i[16]  = 1; \
	ptr_i[17]  = ptr_i[16] + 2; \
	ptr_i[18]  = ptr_i[17] + 4; \
	ptr_i[19]  = ptr_i[18] + 7; \
	i = (int)strlen(str_hmlst[0]);   \
	C2F(cvstr)(&i,&ptr_i[20],str_hmlst[0],(l=0,&l),(unsigned long) i); \
	j = (int)strlen(str_hmlst[1]); \
	C2F(cvstr)(&j,&ptr_i[20+i],str_hmlst[1],(l=0,&l),(unsigned long) j); \
	k = (int)strlen(str_hmlst[2]); \
	C2F(cvstr)(&k,&ptr_i[20+i+j],str_hmlst[2],(l=0,&l),(unsigned long) k); \
	ptr_i[33]  = 8; \
	ptr_i[34]  = 1; \
	ptr_i[35]  = 3; \
	ptr_i[36]  = 4; \
	ptr_i[37]  = nu; \
	ptr_i[38]  = nu2; \
	ptr_i[39]  = nz;
/*--------------------------------------------------------------------------*/
//static int id[nsiz];
/*--------------------------------------------------------------------------*/
static char *str_hmlst[] = {"hm", "dims", "entries"};
/*--------------------------------------------------------------------------*/
#ifdef hppa
#undef FILENAME_MAX
#define FILENAME_MAX 4096
#endif
/*--------------------------------------------------------------------------*/
/* work struct for that block */
typedef struct
{
    int cnt;
    int loop;
    void *work;
    void *workt;
} towork_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void tows_c(scicos_block *block, int flag);
SCICOS_BLOCKS_IMPEXP void tows_c(scicos_block *block, int flag)
/* Copyright INRIA */
/* Put a typed vector in a Scilab file.
* Independant platform.
*
* Author A.Layec - initial rev. 18/09/07
*/
{
    /* generic pointer */
    SCSREAL_COP *u_d, *u_cd, *ptr_d, *sav_d;
    SCSINT8_COP *u_c, *ptr_c, *sav_c;
    SCSUINT8_COP *u_uc, *ptr_uc, *sav_uc;
    SCSINT16_COP *u_s, *ptr_s, *sav_s;
    SCSUINT16_COP *u_us, *ptr_us, *sav_us;
    SCSINT_COP *ptr_i;
    SCSINT32_COP *u_l, *ptr_l, *sav_l;
    SCSUINT32_COP *u_ul, *ptr_ul, *sav_ul;
    double sav_t;
    /* the struct ptr of that block */
    towork_struct** work = (towork_struct**) block->work;
    towork_struct *ptr;
    /* */
    int nu, nu2, ut;
    int nz;
    double t, t_old;
    /* local */
    int i, j, k, l;

    int ismat = 0;

    SciErr sciErr;
    int nRows, nCols, nCols2 = 1, varNameLen;
    char * varName = NULL;
    /* Matrices to be copies of the work array, to pass to Scilab */
    double * MatDouble  = NULL;
    double * MatComplexReal = NULL;
    double * MatComplexImg  = NULL;
    char  *  MatInt8  = NULL;
    short *  MatInt16 = NULL;
    int   *  MatInt32 = NULL;
    unsigned char  *  MatUInt8  = NULL;
    unsigned short *  MatUInt16 = NULL;
    unsigned int   *  MatUInt32 = NULL;
    double * Time = NULL;

    /* retrieve param of that block */
    nu  = GetInPortRows(block, 1); /* number of rows of inputs*/
    nu2 = GetInPortCols(block, 1); /* number of cols of inputs*/
    ut  = GetInType(block, 1);    /* input type */
    nz  = block->ipar[0];         /* buffer size */
    varNameLen = block->ipar[1];

    /* check if u is a matrix */
    if (nu2 != 1)
    {
        ismat = 1;
    }

    if (flag == 4)
    {
        /* init */
        /* begin campaign of allocations */
        if ((*work = (towork_struct*) scicos_malloc(sizeof(towork_struct))) == NULL)
        {
            set_block_error(-16);
            return;
        }

        ptr = *work;

        /*
        * t
        */
        if ((ptr->workt = (void *) scicos_malloc(6 * sizeof(int) + 4 * sizeof(int) + \
                          nz * sizeof(double))) == NULL)
        {
            set_block_error(-16);
            scicos_free(ptr);
            *work = NULL;
            return;
        }
        ptr_i    = (int*) ptr->workt;
        ptr_i[6] = 1;
        ptr_i[7] = nz;
        ptr_i[8] = 1;
        ptr_i[9] = 0;

        ptr_d = (SCSREAL_COP *) & (ptr_i[10]);
        for (i = 0; i < nz; i++)
        {
            ptr_d[i] = -1;
        }

        /* Scilab variable code name */
        //        C2F(namstr)(id, (i = 29, &i), (j = 1, &j), (k = 0, &k));

        ptr_i    = (int*) ptr->workt;
        //        ptr_i[0] = id[0];
        //        ptr_i[1] = id[1];
        //        ptr_i[2] = id[2];
        //        ptr_i[3] = id[3];
        //        ptr_i[4] = id[4];
        //        ptr_i[5] = id[5];

        /*
        * x
        */
        switch (ut)
        {
            case SCSREAL_N    :
                if (ismat)
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + \
                                                            2 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (2 + 4 + 7) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (3) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            nz * nu * nu2 * sizeof(double))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    codehm
                    ptr_i[40]  = 1;                     /*head of entries for hm*/
                    ptr_i[41]  = nu * nu2 * nz;
                    ptr_i[42]  = 1;
                    ptr_i[43]  = 0;
                    ptr_d      = (SCSREAL_COP *) & (ptr_i[44]);

                }
                else
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + 4 * sizeof(int) + \
                                                            nz * nu * sizeof(double))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    ptr_i[6]   = 1;
                    ptr_i[7]   = nz;
                    ptr_i[8]   = nu;
                    ptr_i[9]   = 0;
                    ptr_d      = (SCSREAL_COP *) & (ptr_i[10]);
                }

                for (i = 0; i < nu * nu2 * nz; i++)
                {
                    ptr_d[i] = 0.;
                }
                break;

            case SCSCOMPLEX_N :
                if (ismat)
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + \
                                                            2 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (2 + 4 + 7) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (3) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            2 * nz * nu * nu2 * sizeof(double))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    codehm
                    ptr_i[40]  = 1;                     /*head of entries for hm*/
                    ptr_i[41]  = nu * nu2 * nz;
                    ptr_i[42]  = 1;
                    ptr_i[43]  = 1;
                    ptr_d      = (SCSREAL_COP *) & (ptr_i[44]);

                }
                else
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + 4 * sizeof(int) + \
                                                            2 * nz * nu * sizeof(double))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    ptr_i[6]   = 1;
                    ptr_i[7]   = nz;
                    ptr_i[8]   = nu;
                    ptr_i[9]   = 1;
                    ptr_d      = (SCSREAL_COP *) & (ptr_i[10]);
                }

                for (i = 0; i < 2 * nu * nu2 * nz; i++)
                {
                    ptr_d[i] = 0.;
                }
                break;

            case SCSINT8_N    :
                if (ismat)
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + \
                                                            2 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (2 + 4 + 7) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (3) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            nz * nu * nu2 * sizeof(char))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    codehm
                    ptr_i[40]  = 8;                     /*head of entries for hm*/
                    ptr_i[41]  = nu * nu2 * nz;
                    ptr_i[42]  = 1;
                    ptr_i[43]  = 1;
                    ptr_c      = (SCSINT8_COP *) & (ptr_i[44]);

                }
                else
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + 4 * sizeof(int) + \
                                                            nz * nu * sizeof(char))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    ptr_i[6]   = 8;
                    ptr_i[7]   = nz;
                    ptr_i[8]   = nu;
                    ptr_i[9]   = 1;
                    ptr_c      = (SCSINT8_COP *) & (ptr_i[10]);
                }
                for (i = 0; i < nu * nu2 * nz; i++)
                {
                    ptr_c[i] = 0;
                }
                break;

            case SCSINT16_N   :
                if (ismat)
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + \
                                                            2 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (2 + 4 + 7) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (3) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            nz * nu * nu2 * sizeof(short))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    codehm
                    ptr_i[40]  = 8;                     /*head of entries for hm*/
                    ptr_i[41]  = nu * nu2 * nz;
                    ptr_i[42]  = 1;
                    ptr_i[43]  = 2;
                    ptr_s      = (SCSINT16_COP *) & (ptr_i[44]);
                }
                else
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + 4 * sizeof(int) + \
                                                            nz * nu * sizeof(short))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    ptr_i[6]   = 8;
                    ptr_i[7]   = nz;
                    ptr_i[8]   = nu;
                    ptr_i[9]   = 2;
                    ptr_s      = (SCSINT16_COP *) & (ptr_i[10]);
                }
                for (i = 0; i < nu * nu2 * nz; i++)
                {
                    ptr_s[i] = 0;
                }
                break;

            case SCSINT32_N   :
                if (ismat)
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + \
                                                            2 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (2 + 4 + 7) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (3) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            nz * nu * nu2 * sizeof(int))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    codehm
                    ptr_i[40]  = 8;                     /*head of entries for hm*/
                    ptr_i[41]  = nu * nu2 * nz;
                    ptr_i[42]  = 1;
                    ptr_i[43]  = 4;
                    ptr_l      = (SCSINT32_COP *) & (ptr_i[44]);
                }
                else
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + 4 * sizeof(int) + \
                                                            nz * nu * sizeof(int))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    ptr_i[6]   = 8;
                    ptr_i[7]   = nz;
                    ptr_i[8]   = nu;
                    ptr_i[9]   = 4;
                    ptr_l      = (SCSINT32_COP *) & (ptr_i[10]);
                }
                for (i = 0; i < nu * nu2 * nz; i++)
                {
                    ptr_l[i] = 0;
                }
                break;

            case SCSUINT8_N   :
                if (ismat)
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + \
                                                            2 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (2 + 4 + 7) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (3) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            nz * nu * nu2 * sizeof(unsigned char))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    codehm
                    ptr_i[40]  = 8;                     /*head of entries for hm*/
                    ptr_i[41]  = nu * nu2 * nz;
                    ptr_i[42]  = 1;
                    ptr_i[43]  = 11;
                    ptr_uc     = (SCSUINT8_COP *) & (ptr_i[44]);
                }
                else
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + 4 * sizeof(int) + \
                                                            nz * nu * sizeof(unsigned char))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i       = (int*) ptr->work;
                    ptr_i[6]    = 8;
                    ptr_i[7]    = nz;
                    ptr_i[8]    = nu;
                    ptr_i[9]    = 11;
                    ptr_uc      = (SCSUINT8_COP *) & (ptr_i[10]);
                }
                for (i = 0; i < nu * nu2 * nz; i++)
                {
                    ptr_uc[i] = 0;
                }
                break;

            case SCSUINT16_N  :
                if (ismat)
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + \
                                                            2 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (2 + 4 + 7) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (3) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            nz * nu * nu2 * sizeof(unsigned short))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    codehm
                    ptr_i[40]  = 8;                     /*head of entries for hm*/
                    ptr_i[41]  = nu * nu2 * nz;
                    ptr_i[42]  = 1;
                    ptr_i[43]  = 12;
                    ptr_us     = (SCSUINT16_COP *) & (ptr_i[44]);
                }
                else
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + 4 * sizeof(int) + \
                                                            nz * nu * sizeof(unsigned short))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i = (int*) ptr->work;
                    ptr_i[6]    = 8;
                    ptr_i[7]    = nz;
                    ptr_i[8]    = nu;
                    ptr_i[9]    = 12;
                    ptr_us      = (SCSUINT16_COP *) & (ptr_i[10]);
                }
                for (i = 0; i < nu * nu2 * nz; i++)
                {
                    ptr_us[i] = 0;
                }
                break;

            case SCSUINT32_N  :
                if (ismat)
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + \
                                                            2 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (2 + 4 + 7) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            (3) * sizeof(int) + \
                                                            4 * sizeof(int) + \
                                                            nz * nu * nu2 * sizeof(unsigned int))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i      = (int*) ptr->work;
                    codehm
                    ptr_i[40]  = 8;                     /*head of entries for hm*/
                    ptr_i[41]  = nu * nu2 * nz;
                    ptr_i[42]  = 1;
                    ptr_i[43]  = 14;
                    ptr_ul     = (SCSUINT32_COP *) & (ptr_i[44]);
                }
                else
                {
                    if ((ptr->work = (void *) scicos_malloc(6 * sizeof(int) + 4 * sizeof(int) + \
                                                            nz * nu * sizeof(unsigned int))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr->workt);
                        scicos_free(ptr);
                        *work = NULL;
                        return;
                    }
                    ptr_i = (int*) ptr->work;
                    ptr_i[6]    = 8;
                    ptr_i[7]    = nz;
                    ptr_i[8]    = nu;
                    ptr_i[9]    = 14;
                    ptr_ul      = (SCSUINT32_COP *) & (ptr_i[10]);
                }
                for (i = 0; i < nu * nu2 * nz; i++)
                {
                    ptr_ul[i] = 0;
                }
                break;

            default  : /* Add a message here */
                break;
        }

        /* Scilab variable code name */
        //        C2F(namstr)(id, (i = 33, &i), (j = 1, &j), (k = 0, &k));

        ptr_i    = (int*) ptr->work;
        //        ptr_i[0] = id[0];
        //        ptr_i[1] = id[1];
        //        ptr_i[2] = id[2];
        //        ptr_i[3] = id[3];
        //        ptr_i[4] = id[4];
        //        ptr_i[5] = id[5];

        /*
        * cnt
        */
        ptr->cnt = 0;

        /*
        * loop
        */
        ptr->loop = 0;
    }

    else if (flag == 5)
    {
        /* finish */

        ptr = *work;

        if (ptr != NULL)
        {
            /* check loop */

            /* we are not at the end of the buffer :
            * only first records will be saved
            */
            if ((ptr->cnt == 0) && (ptr->loop == 0))
            {
                /* nothing has been stored */
                ptr_i = (int*) ptr->workt;
                ptr_i[6] = 1;
                ptr_i[7] = 0;
                ptr_i[8] = 0;
                ptr_i[9] = 0;
                ptr_i = (int*) ptr->work;
                ptr_i[6] = 1;
                ptr_i[7] = 0;
                ptr_i[8] = 0;
                ptr_i[9] = 0;
            }

            if ((ptr->cnt != 0) && (ptr->cnt != nz) && (ptr->loop == 0))
            {
                /* something stored */
                /* but we are not at the end */
                ptr_i    = (int*) ptr->workt;
                ptr_i[7] = ptr->cnt;
                ptr_i    = (int*) ptr->work;
                /* hmat */
                if (ismat)
                {
                    ptr_i[39] = ptr->cnt;
                    ptr_i[41] = ptr->cnt * nu * nu2;
                }
                /* vector or matrix */
                else
                {
                    /* vector case */
                    /* adjust only done for complex numbers */
                    if (ptr_i[8] == 1)
                    {
                        switch (ut)
                        {
                            case SCSCOMPLEX_N :
                                for (j = 0; j < ptr->cnt; j++)
                                {
                                    *((double *)(&ptr_i[10]) + (j + ptr->cnt)) = \
                                            *((double *)(&ptr_i[10]) + (j + nz * nu));
                                }
                                break;
                        }
                    }
                    /* matrix case */
                    else
                    {
                        /* */
                        k = nz - ptr->cnt;
                        /* */
                        switch (ut)
                        {
                            case SCSREAL_N    :
                                for (i = 0; i < ptr_i[8] - 1; i++)
                                {
                                    for (j = 0; j < ptr->cnt; j++)
                                    {
                                        *((double *)(&ptr_i[10]) + (((i + 1)*nz) + j - (i + 1)*k)) = \
                                                *((double *)(&ptr_i[10]) + (((i + 1) * nz) + j));
                                    }
                                }
                                break;

                            case SCSCOMPLEX_N :
                                for (i = 0; i < ptr_i[8] - 1; i++)
                                {
                                    for (j = 0; j < ptr->cnt; j++)
                                    {
                                        *((double *)(&ptr_i[10]) + (((i + 1)*nz) + j - (i + 1)*k)) = \
                                                *((double *)(&ptr_i[10]) + (((i + 1) * nz) + j));
                                        *((double *)(&ptr_i[10]) + (((i + 1)*nz) + j - (i + 1)*k) + nz * nu) = \
                                                *((double *)(&ptr_i[10]) + (((i + 1) * nz) + j) + nz * nu);
                                    }
                                }
                                break;

                            case SCSINT8_N :
                                for (i = 0; i < ptr_i[8] - 1; i++)
                                {
                                    for (j = 0; j < ptr->cnt; j++)
                                    {
                                        *((char *)(&ptr_i[10]) + (((i + 1)*nz) + j - (i + 1)*k)) = \
                                                *((char *)(&ptr_i[10]) + (((i + 1) * nz) + j));
                                    }
                                }
                                break;

                            case SCSINT16_N :
                                for (i = 0; i < ptr_i[8] - 1; i++)
                                {
                                    for (j = 0; j < ptr->cnt; j++)
                                    {
                                        *((short *)(&ptr_i[10]) + (((i + 1)*nz) + j - (i + 1)*k)) = \
                                                *((short *)(&ptr_i[10]) + (((i + 1) * nz) + j));
                                    }
                                }
                                break;

                            case SCSINT32_N :
                                for (i = 0; i < ptr_i[8] - 1; i++)
                                {
                                    for (j = 0; j < ptr->cnt; j++)
                                    {
                                        *((int *)(&ptr_i[10]) + (((i + 1)*nz) + j - (i + 1)*k)) = \
                                                *((int *)(&ptr_i[10]) + (((i + 1) * nz) + j));
                                    }
                                }
                                break;

                            case SCSUINT8_N :
                                for (i = 0; i < ptr_i[8] - 1; i++)
                                {
                                    for (j = 0; j < ptr->cnt; j++)
                                    {
                                        *((unsigned char *)(&ptr_i[10]) + (((i + 1)*nz) + j - (i + 1)*k)) = \
                                                *((unsigned char *)(&ptr_i[10]) + (((i + 1) * nz) + j));
                                    }
                                }
                                break;

                            case SCSUINT16_N :
                                for (i = 0; i < ptr_i[8] - 1; i++)
                                {
                                    for (j = 0; j < ptr->cnt; j++)
                                    {
                                        *((unsigned short *)(&ptr_i[10]) + (((i + 1)*nz) + j - (i + 1)*k)) = \
                                                *((unsigned short *)(&ptr_i[10]) + (((i + 1) * nz) + j));
                                    }
                                }
                                break;

                            case SCSUINT32_N :
                                for (i = 0; i < ptr_i[8] - 1; i++)
                                {
                                    for (j = 0; j < ptr->cnt; j++)
                                    {
                                        *((unsigned int *)(&ptr_i[10]) + (((i + 1)*nz) + j - (i + 1)*k)) = \
                                                *((unsigned int *)(&ptr_i[10]) + (((i + 1) * nz) + j));
                                    }
                                }
                                break;

                        }
                    }
                    ptr_i[7] = ptr->cnt;
                }
            }
            /* sort data */
            else if ((ptr->cnt != 0) && (ptr->cnt != nz) && (ptr->loop != 0))
            {
                /* shift time data */
                for (i = 0; i < (nz - ptr->cnt); i++)
                {
                    ptr_i = (int*) ptr->workt;
                    sav_t = *((double *)(&ptr_i[10]) + (nz - 1));
                    for (j = (nz - 1); j >= 1; j--)
                    {
                        *((double *)(&ptr_i[10]) + j) = *((double *)(&ptr_i[10]) + (j - 1));
                    }
                    *((double *)(&ptr_i[10])) = sav_t;
                }
                /* shift x data */
                switch (ut)
                {
                    case SCSREAL_N    :
                        if ((sav_d = (double *) scicos_malloc(nu * nu2 * sizeof(double))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            *work = NULL;
                            return;
                        }
                        ptr_i = (int*) ptr->work;
                        if (ismat)
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    sav_d[k] = *((double *)(&ptr_i[44]) + k + (nz - 1) * nu * nu2);
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu * nu2; k++)
                                    {
                                        *((double *)(&ptr_i[44]) + k + j * (nu * nu2)) = *((double *)(&ptr_i[44]) + k + (j - 1) * (nu * nu2));
                                    }
                                }
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    *((double *)(&ptr_i[44]) + k)  = sav_d[k];
                                }
                            }
                        }
                        else
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu; k++)
                                {
                                    sav_d[k] = *((double *)(&ptr_i[10]) + k * nz + (nz - 1));
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu; k++)
                                    {
                                        *((double *)(&ptr_i[10]) + k * nz + j) = *((double *)(&ptr_i[10]) + k * nz + j - 1);
                                    }
                                }
                                for (k = 0; k < nu; k++)
                                {
                                    *((double *)(&ptr_i[10]) + k * nz)  = sav_d[k];
                                }
                            }
                        }
                        scicos_free(sav_d);
                        break;
                    case SCSCOMPLEX_N :
                        if ((sav_d = (double *) scicos_malloc(2 * nu * nu2 * sizeof(double))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            *work = NULL;
                            return;
                        }
                        ptr_i = (int *) ptr->work;
                        if (ismat)
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    sav_d[k]        = *((double *)(&ptr_i[44]) + k + (nz - 1) * nu * nu2);
                                    sav_d[k + nu * nu2] = *((double *)(&ptr_i[44]) + k + (nz - 1) * nu * nu2 + nz * nu * nu2);
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu * nu2; k++)
                                    {
                                        *((double *)(&ptr_i[44]) + k + j * (nu * nu2)) = \
                                                *((double *)(&ptr_i[44]) + k + (j - 1) * (nu * nu2));
                                        *((double *)(&ptr_i[44]) + k + j * (nu * nu2) + nz * nu * nu2) = \
                                                *((double *)(&ptr_i[44]) + k + (j - 1) * (nu * nu2) + nz * nu * nu2);
                                    }
                                }
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    *((double *)(&ptr_i[44]) + k)           = sav_d[k];
                                    *((double *)(&ptr_i[44]) + k + nz * nu * nu2) = sav_d[k + nu * nu2];
                                }
                            }
                        }
                        else
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu; k++)
                                {
                                    sav_d[k]    = *((double *)(&ptr_i[10]) + k * nz + (nz - 1));
                                    sav_d[k + nu] = *((double *)(&ptr_i[10]) + k * nz + (nz - 1) + nz * nu);
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu; k++)
                                    {
                                        *((double *)(&ptr_i[10]) + k * nz + j) = \
                                                *((double *)(&ptr_i[10]) + k * nz + j - 1);
                                        *((double *)(&ptr_i[10]) + k * nz + j + nz * nu) = \
                                                *((double *)(&ptr_i[10]) + k * nz + j - 1 + nz * nu);
                                    }
                                }
                                for (k = 0; k < nu; k++)
                                {
                                    *((double *)(&ptr_i[10]) + k * nz)        = sav_d[k];
                                    *((double *)(&ptr_i[10]) + k * nz + nz * nu)  = sav_d[k + nu];
                                }
                            }
                        }
                        scicos_free(sav_d);
                        break;
                    case SCSINT8_N    :
                        if ((sav_c = (char *) scicos_malloc(nu * nu2 * sizeof(char))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            *work = NULL;
                            return;
                        }
                        ptr_i = (int *) ptr->work;
                        if (ismat)
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    sav_c[k] = *((char *)(&ptr_i[44]) + k + (nz - 1) * nu * nu2);
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu * nu2; k++)
                                    {
                                        *((char *)(&ptr_i[44]) + k + j * (nu * nu2)) = *((char *)(&ptr_i[44]) + k + (j - 1) * (nu * nu2));
                                    }
                                }
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    *((char *)(&ptr_i[44]) + k)  = sav_c[k];
                                }
                            }
                        }
                        else
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu; k++)
                                {
                                    sav_c[k] = *((char *)(&ptr_i[10]) + k * nz + (nz - 1));
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu; k++)
                                    {
                                        *((char *)(&ptr_i[10]) + k * nz + j) = *((char *)(&ptr_i[10]) + k * nz + j - 1);
                                    }
                                }
                                for (k = 0; k < nu; k++)
                                {
                                    *((char *)(&ptr_i[10]) + k * nz)  = sav_c[k];
                                }
                            }
                        }
                        scicos_free(sav_c);
                        break;
                    case SCSINT16_N   :
                        if ((sav_s = (short *) scicos_malloc(nu * nu2 * sizeof(short))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            *work = NULL;
                            return;
                        }
                        ptr_i = (int *) ptr->work;
                        if (ismat)
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    sav_s[k] = *((short *)(&ptr_i[44]) + k + (nz - 1) * nu * nu2);
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu * nu2; k++)
                                    {
                                        *((short *)(&ptr_i[44]) + k + j * (nu * nu2)) = *((short *)(&ptr_i[44]) + k + (j - 1) * (nu * nu2));
                                    }
                                }
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    *((short *)(&ptr_i[44]) + k)  = sav_s[k];
                                }
                            }
                        }
                        else
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu; k++)
                                {
                                    sav_s[k] = *((short *)(&ptr_i[10]) + k * nz + (nz - 1));
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu; k++)
                                    {
                                        *((short *)(&ptr_i[10]) + k * nz + j) = *((short *)(&ptr_i[10]) + k * nz + j - 1);
                                    }
                                }
                                for (k = 0; k < nu; k++)
                                {
                                    *((short *)(&ptr_i[10]) + k * nz)  = sav_s[k];
                                }
                            }
                        }
                        scicos_free(sav_s);
                        break;
                    case SCSINT32_N   :
                        if ((sav_l = (int *) scicos_malloc(nu * nu2 * sizeof(int))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            *work = NULL;
                            return;
                        }
                        ptr_i = (int *) ptr->work;
                        if (ismat)
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    sav_l[k] = *((int *)(&ptr_i[44]) + k + (nz - 1) * nu * nu2);
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu * nu2; k++)
                                    {
                                        *((int *)(&ptr_i[44]) + k + j * (nu * nu2)) = *((int *)(&ptr_i[44]) + k + (j - 1) * (nu * nu2));
                                    }
                                }
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    *((int *)(&ptr_i[44]) + k)  = sav_l[k];
                                }
                            }
                        }
                        else
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu; k++)
                                {
                                    sav_l[k] = *((int *)(&ptr_i[10]) + k * nz + (nz - 1));
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu; k++)
                                    {
                                        *((int *)(&ptr_i[10]) + k * nz + j) = *((int *)(&ptr_i[10]) + k * nz + j - 1);
                                    }
                                }
                                for (k = 0; k < nu; k++)
                                {
                                    *((int *)(&ptr_i[10]) + k * nz)  = sav_l[k];
                                }
                            }
                        }
                        scicos_free(sav_l);
                        break;
                    case SCSUINT8_N   :
                        if ((sav_uc = (unsigned char *) scicos_malloc(nu * nu2 * sizeof(unsigned char))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            *work = NULL;
                            return;
                        }
                        ptr_i = (int *) ptr->work;
                        if (ismat)
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    sav_uc[k] = *((unsigned char *)(&ptr_i[44]) + k + (nz - 1) * nu * nu2);
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu * nu2; k++)
                                    {
                                        *((unsigned char *)(&ptr_i[44]) + k + j * (nu * nu2)) = *((unsigned char *)(&ptr_i[44]) + k + (j - 1) * (nu * nu2));
                                    }
                                }
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    *((unsigned char *)(&ptr_i[44]) + k)  = sav_uc[k];
                                }
                            }
                        }
                        else
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu; k++)
                                {
                                    sav_uc[k] = *((unsigned char *)(&ptr_i[10]) + k * nz + (nz - 1));
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu; k++)
                                    {
                                        *((unsigned char *)(&ptr_i[10]) + k * nz + j) = *((unsigned char *)(&ptr_i[10]) + k * nz + j - 1);
                                    }
                                }
                                for (k = 0; k < nu; k++)
                                {
                                    *((unsigned char *)(&ptr_i[10]) + k * nz)  = sav_uc[k];
                                }
                            }
                        }
                        scicos_free(sav_uc);
                        break;
                    case SCSUINT16_N  :
                        if ((sav_us = (unsigned short *) scicos_malloc(nu * nu2 * sizeof(unsigned short))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            *work = NULL;
                            return;
                        }
                        ptr_i = (int *) ptr->work;
                        if (ismat)
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    sav_us[k] = *((unsigned short *)(&ptr_i[44]) + k + (nz - 1) * nu * nu2);
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu * nu2; k++)
                                    {
                                        *((unsigned short *)(&ptr_i[44]) + k + j * (nu * nu2)) = *((unsigned short *)(&ptr_i[44]) + k + (j - 1) * (nu * nu2));
                                    }
                                }
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    *((unsigned short *)(&ptr_i[44]) + k)  = sav_us[k];
                                }
                            }
                        }
                        else
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu; k++)
                                {
                                    sav_us[k] = *((unsigned short *)(&ptr_i[10]) + k * nz + (nz - 1));
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu; k++)
                                    {
                                        *((unsigned short *)(&ptr_i[10]) + k * nz + j) = *((unsigned short *)(&ptr_i[10]) + k * nz + j - 1);
                                    }
                                }
                                for (k = 0; k < nu; k++)
                                {
                                    *((unsigned short *)(&ptr_i[10]) + k * nz)  = sav_us[k];
                                }
                            }
                        }
                        scicos_free(sav_us);
                        break;
                    case SCSUINT32_N  :
                        if ((sav_ul = (unsigned int *) scicos_malloc(nu * nu2 * sizeof(unsigned int))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            *work = NULL;
                            return;
                        }
                        ptr_i = (int *) ptr->work;
                        if (ismat)
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    sav_ul[k] = *((unsigned int *)(&ptr_i[44]) + k + (nz - 1) * nu * nu2);
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu * nu2; k++)
                                    {
                                        *((unsigned int *)(&ptr_i[44]) + k + j * (nu * nu2)) = *((unsigned int *)(&ptr_i[44]) + k + (j - 1) * (nu * nu2));
                                    }
                                }
                                for (k = 0; k < nu * nu2; k++)
                                {
                                    *((unsigned int *)(&ptr_i[44]) + k)  = sav_ul[k];
                                }
                            }
                        }
                        else
                        {
                            for (i = 0; i < (nz - ptr->cnt); i++)
                            {
                                for (k = 0; k < nu; k++)
                                {
                                    sav_ul[k] = *((unsigned int *)(&ptr_i[10]) + k * nz + (nz - 1));
                                }
                                for (j = (nz - 1); j >= 1; j--)
                                {
                                    for (k = 0; k < nu; k++)
                                    {
                                        *((unsigned int *)(&ptr_i[10]) + k * nz + j) = *((unsigned int *)(&ptr_i[10]) + k * nz + j - 1);
                                    }
                                }
                                for (k = 0; k < nu; k++)
                                {
                                    *((unsigned int *)(&ptr_i[10]) + k * nz)  = sav_ul[k];
                                }
                            }
                        }
                        scicos_free(sav_ul);
                        break;
                    default  : /* Add a message here */
                        break;
                }
            }

            /* write x */
            ptr_i = (int*) ptr->work;
            if ((varName = (char*) scicos_malloc((varNameLen + 6) * sizeof(char))) == NULL)
            {
                set_block_error(-16);
                scicos_free(ptr->workt);
                scicos_free(ptr);
                *work = NULL;
                return;
            }
            memset(varName, 0x0, (varNameLen + 6) * sizeof(char));
            /* save the Scilab name into varName */
            for (i = 0; i < varNameLen; ++i)
            {
                varName[i] = (char) block->ipar[i + 2];
            }
            if (ismat)
            {
                nRows  = ptr_i[39]; /* nz  */
                nCols  = ptr_i[37]; /* nu  */
                nCols2 = ptr_i[38]; /* nu2 */
            }
            else
            {
                nRows = ptr_i[7]; /* nz */
                nCols = ptr_i[8]; /* nu */
            }
            if ((ptr->cnt != 0) || (ptr->loop != 0))
            {
                /* The values will be stored in 'variableName'_val */
                varName = strcat(varName, "_val");
                switch (ut)
                {
                    // By default, nCols = 1, so should the input be a matrix, scalar or vector, perform the same operations
                    case SCSREAL_N    :
                        ptr_d = (SCSREAL_COP *) ((ismat) ? &ptr_i[44] : &ptr_i[10]);
                        if ((MatDouble = (double *) scicos_malloc(nRows * nCols * nCols2 * sizeof(double))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            *work = NULL;
                            return;
                        }
                        memcpy(MatDouble, ptr_d, nRows * nCols * nCols2 * sizeof(double));
                        /* pass Matrix to Scilab */
                        sciErr = createNamedMatrixOfDouble(NULL, varName, nRows * nCols2, nCols, MatDouble);
                        if (sciErr.iErr)
                        {
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            *work = NULL;
                            printError(&sciErr, 0);
                            return;
                        }
                        scicos_free(MatDouble);
                        break;
                    case SCSCOMPLEX_N :
                        ptr_d = (SCSREAL_COP *) ((ismat) ? &ptr_i[44] : &ptr_i[10]);
                        if (((MatComplexReal = (double *) scicos_malloc(nRows * nCols * nCols2 * sizeof(double))) == NULL)
                                || ((MatComplexImg = (double *) scicos_malloc(nRows * nCols * nCols2 * sizeof(double))) == NULL))
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            *work = NULL;
                            return;
                        }
                        memcpy(MatComplexReal, ptr_d, nRows * nCols * nCols2 * sizeof(double));
                        memcpy(MatComplexImg, ptr_d + nRows * nCols * nCols2, nRows * nCols * nCols2 * sizeof(double));
                        /* pass Matrix to Scilab */
                        sciErr = createNamedComplexMatrixOfDouble(NULL, varName, nRows * nCols2, nCols, MatComplexReal, MatComplexImg);
                        if (sciErr.iErr)
                        {
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            *work = NULL;
                            printError(&sciErr, 0);
                            return;
                        }
                        scicos_free(MatComplexReal);
                        scicos_free(MatComplexImg);
                        break;
                    case SCSINT8_N    :
                        ptr_c = (SCSINT8_COP *) ((ismat) ? &ptr_i[44] : &ptr_i[10]);
                        if ((MatInt8 = (char *) scicos_malloc(nRows * nCols * nCols2 * sizeof(char))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            *work = NULL;
                            return;
                        }
                        for (i = 0; i <  nRows * nCols * nCols2; ++i)
                        {
                            MatInt8[i] = (char) ptr_c[i];
                        }
                        /* pass Matrix to Scilab */
                        sciErr = createNamedMatrixOfInteger8(NULL, varName, nRows * nCols2, nCols, MatInt8);
                        if (sciErr.iErr)
                        {
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            scicos_free(MatInt8);
                            *work = NULL;
                            printError(&sciErr, 0);
                            return;
                        }
                        scicos_free(MatInt8);
                        break;
                    case SCSINT16_N   :
                        ptr_s = (SCSINT16_COP *) ((ismat) ? &ptr_i[44] : &ptr_i[10]);
                        if ((MatInt16 = (short *) scicos_malloc(nRows * nCols * nCols2 * sizeof(short))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            *work = NULL;
                            return;
                        }
                        for (i = 0; i <  nRows * nCols * nCols2; ++i)
                        {
                            MatInt16[i] = (short) ptr_s[i];
                        }
                        /* pass Matrix to Scilab */
                        sciErr = createNamedMatrixOfInteger16(NULL, varName, nRows * nCols2, nCols, MatInt16);
                        if (sciErr.iErr)
                        {
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            scicos_free(MatInt16);
                            *work = NULL;
                            printError(&sciErr, 0);
                            return;
                        }
                        scicos_free(MatInt16);
                        break;
                    case SCSINT32_N   :
                        ptr_l = (SCSINT32_COP *) ((ismat) ? &ptr_i[44] : &ptr_i[10]);
                        if ((MatInt32 = (int *) scicos_malloc(nRows * nCols * nCols2 * sizeof(int))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            *work = NULL;
                            return;
                        }
                        for (i = 0; i <  nRows * nCols * nCols2; ++i)
                        {
                            MatInt32[i] = (int) ptr_l[i];
                        }
                        /* pass Matrix to Scilab */
                        sciErr = createNamedMatrixOfInteger32(NULL, varName, nRows * nCols2, nCols, MatInt32);
                        if (sciErr.iErr)
                        {
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            scicos_free(MatInt32);
                            *work = NULL;
                            printError(&sciErr, 0);
                            return;
                        }
                        scicos_free(MatInt32);
                        break;
                    case SCSUINT8_N   :
                        ptr_uc = (SCSUINT8_COP *) ((ismat) ? &ptr_i[44] : &ptr_i[10]);
                        if ((MatUInt8 = (unsigned char *) scicos_malloc(nRows * nCols * nCols2 * sizeof(unsigned char))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            *work = NULL;
                            return;
                        }
                        for (i = 0; i <  nRows * nCols * nCols2; ++i)
                        {
                            MatUInt8[i] = (unsigned char) ptr_uc[i];
                        }
                        /* pass Matrix to Scilab */
                        sciErr = createNamedMatrixOfUnsignedInteger8(NULL, varName, nRows * nCols2, nCols, MatUInt8);
                        if (sciErr.iErr)
                        {
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            scicos_free(MatUInt8);
                            *work = NULL;
                            printError(&sciErr, 0);
                            return;
                        }
                        scicos_free(MatUInt8);
                        break;
                    case SCSUINT16_N  :
                        ptr_us = (SCSUINT16_COP *) ((ismat) ? &ptr_i[44] : &ptr_i[10]);
                        if ((MatUInt16 = (unsigned short *) scicos_malloc(nRows * nCols * nCols2 * sizeof(unsigned short))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            *work = NULL;
                            return;
                        }
                        for (i = 0; i <  nRows * nCols * nCols2; ++i)
                        {
                            MatUInt16[i] = (unsigned short) ptr_us[i];
                        }
                        /* pass Matrix to Scilab */
                        sciErr = createNamedMatrixOfUnsignedInteger16(NULL, varName, nRows * nCols2, nCols, MatUInt16);
                        if (sciErr.iErr)
                        {
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            scicos_free(MatUInt16);
                            *work = NULL;
                            printError(&sciErr, 0);
                            return;
                        }
                        scicos_free(MatUInt16);
                        break;
                    case SCSUINT32_N  :
                        ptr_ul = (SCSUINT32_COP *) ((ismat) ? &ptr_i[44] : &ptr_i[10]);
                        if ((MatUInt32 = (unsigned int *) scicos_malloc(nRows * nCols * nCols2 * sizeof(unsigned int))) == NULL)
                        {
                            set_block_error(-16);
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            *work = NULL;
                            return;
                        }
                        for (i = 0; i <  nRows * nCols * nCols2; ++i)
                        {
                            MatUInt32[i] = (unsigned int) ptr_ul[i];
                        }
                        /* pass Matrix to Scilab */
                        sciErr = createNamedMatrixOfUnsignedInteger32(NULL, varName, nRows * nCols2, nCols, MatUInt32);
                        if (sciErr.iErr)
                        {
                            scicos_free(ptr->workt);
                            scicos_free(ptr);
                            scicos_free(varName);
                            scicos_free(MatUInt32);
                            *work = NULL;
                            printError(&sciErr, 0);
                            return;
                        }
                        scicos_free(MatUInt32);
                        break;
                    default  : /* Add a message here */
                        break;
                }
            }

            /* write t */
            ptr_i = (int*) ptr->workt;
            if ((ptr->cnt != 0) || (ptr->loop != 0))
            {
                ptr_d = (SCSREAL_COP *) &ptr_i[10];
                /* The TIME values will be stored in 'variableName'_valt */
                varName = strcat(varName, "t");
                /* Using variable Time to hold time values */
                if ((Time = (double *) scicos_malloc(nRows * sizeof(double))) == NULL)
                {
                    set_block_error(-16);
                    scicos_free(ptr->workt);
                    scicos_free(ptr);
                    scicos_free(varName);
                    scicos_free(Time);
                    *work = NULL;
                    return;
                }
                memcpy(Time, ptr_d, nRows * sizeof(double));
                /* pass Matrix to Scilab */
                sciErr = createNamedMatrixOfDouble(NULL, varName, nRows, 1, Time);
                if (sciErr.iErr)
                {
                    scicos_free(ptr->workt);
                    scicos_free(ptr);
                    scicos_free(varName);
                    scicos_free(Time);
                    *work = NULL;
                    printError(&sciErr, 0);
                    return;
                }
                scicos_free(Time);
            }

            /* free */
            scicos_free(varName);
            scicos_free(ptr->work);
            scicos_free(ptr->workt);
            scicos_free(ptr);
        }

    }

    else if ((flag == 2) || (flag == 0))
    {
        /* update state */
        ptr = *work;

        ptr_i = (int*) ptr->work;

        /* check data dimension */
        if (ismat)
        {
            if ((nz != ptr_i[39]) || (nu != ptr_i[37]) || (nu2 != ptr_i[38]))
            {
                Coserror(_("Size of buffer or input size have changed!\n"));
                /*set_block_error(-1);*/
                return;
            }
        }
        else
        {
            if ((nz != ptr_i[7]) || (nu != ptr_i[8]))
            {
                Coserror(_("Size of buffer or input size have changed!\n"));
                /*set_block_error(-1);*/
                return;
            }
        }

        /*
        * test on time
        */

        ptr_i = (int*) ptr->workt;
        ptr_d = (SCSREAL_COP *) & (ptr_i[10]);

        /* get current time */
        t     = get_scicos_time();

        /* get old time */
        if (ptr->cnt == 0)
        {
            t_old = ptr_d[nz - 1];
        }
        else
        {
            t_old = ptr_d[ptr->cnt - 1];
        }

        /* get current index of cnt */
        while (t_old >= t)
        {
            /*decrease*/
            if (ptr->cnt == 0)
            {
                ptr->cnt = nz - 1;
            }
            else
            {
                ptr->cnt = ptr->cnt - 1;
            }

            /* get old time */
            if (ptr->cnt == 0)
            {
                t_old = ptr_d[nz - 1];
            }
            else
            {
                t_old = ptr_d[ptr->cnt - 1];
            }
        }
        /*fprintf(stderr,"flag=%d, ptr->cnt=%d, t=%20.18f, t_old=%20.18f, t_old>=t=%d\n",flag,ptr->cnt,t,t_old,t_old>=t);*/

        /*
        *store
        */

        ptr_i = (int*) ptr->work;

        /* x */
        switch (ut)
        {
            case SCSREAL_N    :
                u_d   = GetRealInPortPtrs(block, 1);
                if (ismat)
                {
                    ptr_d = (SCSREAL_COP *) & (ptr_i[44]);
                    for (i = 0; i < nu * nu2; i++)
                    {
                        ptr_d[ptr->cnt * (nu * nu2) + i] = u_d[i];
                    }
                }
                else
                {
                    ptr_d = (SCSREAL_COP *) & (ptr_i[10]);
                    for (i = 0; i < nu * nu2; i++)
                    {
                        ptr_d[ptr->cnt + i * nz] = u_d[i];
                    }
                }
                break;

            case SCSCOMPLEX_N :
                u_d  = GetRealInPortPtrs(block, 1);
                u_cd = GetImagInPortPtrs(block, 1);
                if (ismat)
                {
                    ptr_d = (SCSREAL_COP *) & (ptr_i[44]);
                    for (i = 0; i < nu * nu2; i++)
                    {
                        ptr_d[ptr->cnt * (nu * nu2) + i] = u_d[i];
                        ptr_d[nz * nu * nu2 + ptr->cnt * (nu * nu2) + i] = u_cd[i];
                    }
                }
                else
                {
                    ptr_d = (SCSREAL_COP *) & (ptr_i[10]);
                    for (i = 0; i < nu; i++)
                    {
                        ptr_d[ptr->cnt + i * nz]       = u_d[i];
                        ptr_d[nz * nu + ptr->cnt + i * nz] = u_cd[i];
                    }
                }
                break;

            case SCSINT8_N    :
                u_c   = Getint8InPortPtrs(block, 1);
                if (ismat)
                {
                    ptr_c = (SCSINT8_COP *) & (ptr_i[44]);
                    for (i = 0; i < nu * nu2; i++)
                    {
                        ptr_c[ptr->cnt * (nu * nu2) + i] = u_c[i];
                    }
                }
                else
                {
                    ptr_c = (SCSINT8_COP *) & (ptr_i[10]);
                    for (i = 0; i < nu; i++)
                    {
                        ptr_c[ptr->cnt + i * nz] = u_c[i];
                    }
                }
                break;

            case SCSINT16_N   :
                u_s   = Getint16InPortPtrs(block, 1);
                if (ismat)
                {
                    ptr_s = (SCSINT16_COP *) & (ptr_i[44]);
                    for (i = 0; i < nu * nu2; i++)
                    {
                        ptr_s[ptr->cnt * (nu * nu2) + i] = u_s[i];
                    }
                }
                else
                {
                    ptr_s = (SCSINT16_COP *) & (ptr_i[10]);
                    for (i = 0; i < nu; i++)
                    {
                        ptr_s[ptr->cnt + i * nz] = u_s[i];
                    }
                }
                break;

            case SCSINT32_N   :
                u_l   = Getint32InPortPtrs(block, 1);
                if (ismat)
                {
                    ptr_l = (SCSINT32_COP *) & (ptr_i[44]);
                    for (i = 0; i < nu * nu2; i++)
                    {
                        ptr_l[ptr->cnt * (nu * nu2) + i] = u_l[i];
                    }
                }
                else
                {
                    ptr_l = (SCSINT32_COP *) & (ptr_i[10]);
                    for (i = 0; i < nu; i++)
                    {
                        ptr_l[ptr->cnt + i * nz] = u_l[i];
                    }
                }
                break;

            case SCSUINT8_N   :
                u_uc   = Getuint8InPortPtrs(block, 1);
                if (ismat)
                {
                    ptr_uc = (SCSUINT8_COP *) & (ptr_i[44]);
                    for (i = 0; i < nu * nu2; i++)
                    {
                        ptr_uc[ptr->cnt * (nu * nu2) + i] = u_uc[i];
                    }
                }
                else
                {
                    ptr_uc = (SCSUINT8_COP *) & (ptr_i[10]);
                    for (i = 0; i < nu; i++)
                    {
                        ptr_uc[ptr->cnt + i * nz] = u_uc[i];
                    }
                }
                break;

            case SCSUINT16_N  :
                u_us   = Getuint16InPortPtrs(block, 1);
                if (ismat)
                {
                    ptr_us = (SCSUINT16_COP *) & (ptr_i[44]);
                    for (i = 0; i < nu * nu2; i++)
                    {
                        ptr_us[ptr->cnt * (nu * nu2) + i] = u_us[i];
                    }
                }
                else
                {
                    ptr_us = (SCSUINT16_COP *) & (ptr_i[10]);
                    for (i = 0; i < nu; i++)
                    {
                        ptr_us[ptr->cnt + i * nz] = u_us[i];
                    }
                }
                break;

            case SCSUINT32_N  :
                u_ul = Getuint32InPortPtrs(block, 1);
                if (ismat)
                {
                    ptr_ul = (SCSUINT32_COP *) & (ptr_i[44]);
                    for (i = 0; i < nu * nu2; i++)
                    {
                        ptr_ul[ptr->cnt * (nu * nu2) + i] = u_ul[i];
                    }
                }
                else
                {
                    ptr_ul = (SCSUINT32_COP *) & (ptr_i[10]);
                    for (i = 0; i < nu; i++)
                    {
                        ptr_ul[ptr->cnt + i * nz] = u_ul[i];
                    }
                }
                break;

            default  : /* Add a message here */
                break;
        }

        /* t */
        ptr_i           = (int*) ptr->workt;
        ptr_d           = (SCSREAL_COP *) & (ptr_i[10]);
        ptr_d[ptr->cnt]  = t;

        /*
        * update cnt
        */
        ptr->cnt++;
        if (ptr->cnt == nz)
        {
            ptr->cnt = 0;
            ptr->loop++;
        }
    }
}
/*--------------------------------------------------------------------------*/
