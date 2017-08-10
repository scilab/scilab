
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
 * Copyright (C) 2005-2008 - INRIA - Pierrick MODE
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "core_math.h"
#include "sci_malloc.h" /* MALLOC */
#include "sciprint.h"
#include "returnanan.h"
#include "xls.h"
#include "mseek.h"
#include "mtell.h"
#include "mget.h"
/*------------------------------------------------------------------*/
#define  typ_short "s"
#define  typ_ushort "us"
#define  typ_char "c"
#define  typ_uchar "uc"
#define  typ_double "d"
#define  typ_int "i"
/*------------------------------------------------------------------*/
extern int ripole(char *inputfile, char *outputfile, int debug, int verbose);
/*------------------------------------------------------------------*/
/*Prototype*/
static double NumFromRk2(long rk);
static void getBoundsheets(int * fd, char ***Sheetnames, int** Abspos, int *nsheets, long long *cur_pos, int *err);
static void getSST(int *fd, short Len, int BIFF, int *ns, char ***sst, int *err);
static void getBOF(int *fd , int* Data, int *err);
static void getString(int *fd, short *count, short *Len, int flag, char **str, int *err);

/**
 ** Bruno : Defined but not used ... so what !!!!!!!!
 static int get_oleheader(int *fd);
**/
/*------------------------------------------------------------------*/
void xls_read(int *fd, int *cur_pos, double **data, int **chainesind, int *N, int *M, int *err)
{
    /*---------------Declaration Des Variables*--------------------*/
    unsigned short Opcode = 0, Len = 0;   /*Code Operationnel et Longueur du tag a lire*/
    double *valeur = NULL;    /*Tableau Recapitulatif (Final) des valeurs de la feuille Excel*/
    long long pos = 0;

    int one = 1;
    int three = 3;

    int i = 0;  /*Variables de boucle*/
    int hauteur = 0, longueur = 0, capacite = 0;   /*Hauteur, longueur de la feuille,  */
    /*int taille = 0; Nombre de types de caract�ers a enregistrer*/
    char *sheetname = NULL;   /*Nom de la feuille*/
    int rkvalue = 0; /*RK value*/
    /*for RK */
    unsigned short row = 0, col = 0, xf = 0;/*Index to row, to column, and to XF record*/
    /*for MULRK */
    unsigned short ixfe = 0;
    short colFirst = 0, colLast = 0, ncol = 0; /*Index to rox, to first column (fc)*/
    /* for LABELSST */
    short labelsst1[3];
    int indsst = 0; /*Index to SST record*/
    /* for DIMENSIONS */
    int f_row = 0, l_row = 0;
    unsigned short f_col = 0, l_col = 0, notused = 0;
    /* for FORMULA */
    double resultat = 0.;/*Result of the formula*/
    short optionflag = 0;/*Option flags*/
    int formula_notused = 0; /*Not used*/
    double NaN = C2F(returnanan)();

    int BOFData[7]; /*[BIFF  Version DataType Identifier Year HistoryFlags LowestXlsVersion]*/
    /* initialization of pointers corresponding to malloc's */
    valeur = (double *)NULL;
    sheetname = (char *)NULL;
    *chainesind = (int *) NULL;
    *err = 0;

    *err = mseek(*fd, (long long) * cur_pos, SEEK_SET);
    if (*err > 0)
    {
        goto ErrL;
    }

    /* first record should be a BOF */
    getBOF(fd , BOFData, err);

    if (*err > 0)
    {
        return;
    }
    if (BOFData[0] < 0) /* not a BOF */
    {
        *err = 2;
        return;
    }
    if (BOFData[0] != 8)   /* not a BIFF8 */
    {
        *err = 3;
        return;
    }

    pos = mtell(*fd);
    if (pos < 0)
    {
        *err = 2;
        goto ErrL;
    }

    while (1)
    {
        *err = mseek(*fd, pos, SEEK_SET);
        if (*err > 0)
        {
            goto ErrL;
        }
        /*Enregistrement de l'Opcode et de la Len du tag*/
        C2F(mgetnc) (fd, &Opcode, &one, typ_ushort, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        C2F(mgetnc) (fd, &Len, &one, typ_ushort, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        switch (Opcode)
        {
            case 10:/*EOF */
                *N = hauteur;
                *M = longueur;
                *data = valeur;
                pos = pos + 4 + Len;
                *cur_pos = (int)pos;
                return;
            case 638: /*RK*/
                C2F(mgetnc) (fd, (void*)&row, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*)&col, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                // Check col and row are in bounds
                if ((col >= longueur) || (row >= hauteur))
                {
                    *err = 2;
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*)&xf , &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*) &rkvalue , &one, typ_int, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                valeur[col * (hauteur) + row] = NumFromRk2(rkvalue);
                break;
            case 515: /*Number*/
                C2F(mgetnc) (fd, (void*)&row, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*)&col, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                // Check col and row are in bounds
                if ((col >= longueur) || (row >= hauteur))
                {
                    *err = 2;
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*)&xf , &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*) &resultat , &one, typ_double, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                valeur[col * (hauteur) + row] = resultat ;
                break;

            case 189: /*MULRK*/
                C2F(mgetnc) (fd, (void*)&row, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                C2F(mgetnc) (fd,  (void*)&colFirst, &one, typ_short, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                // Check col and row are in bounds
                if ((colFirst >= longueur) || (row >= hauteur))
                {
                    *err = 2;
                    goto ErrL;
                }
                /*List of nc=lc-fc+1  XF/RK structures*/
                ncol = (Len - 6) / 6;
                for (i = 0; i < ncol; i++)
                {
                    C2F(mgetnc) (fd, (void*) &ixfe, &one, typ_short, err);
                    if (*err > 0)
                    {
                        goto ErrL;
                    }
                    C2F(mgetnc) (fd, (void*) &rkvalue, &one, typ_int, err);
                    if (*err > 0)
                    {
                        goto ErrL;
                    }
                    valeur[row + (colFirst + i)*hauteur] = NumFromRk2(rkvalue);
                }

                /*Index of last column*/
                C2F(mgetnc) (fd, (void*) &colLast, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                break;

            case 253:/*LABELSST*/
                C2F(mgetnc) (fd, (void*) labelsst1, &three, typ_short, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*) &indsst , &one, typ_int, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                /*Allocation dans le tableau final*/
                col = labelsst1[1];
                row = labelsst1[0];
                // Check col and row are in bounds
                if ((col >= longueur) || (row >= hauteur))
                {
                    *err = 2;
                    goto ErrL;
                }
                (*chainesind)[col * (hauteur) + row] = indsst + 1;
                break;
            case 512:/* DIMENSIONS*/
                C2F(mgetnc) (fd, (void*) &f_row, &one, typ_int, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*) &l_row, &one, typ_int, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*) &f_col, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*) &l_col, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*) &notused, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }

                /*Calcul de longueur, hauteur et capacite de la feuille*/
                hauteur = l_row; /*-f_row;*/
                longueur = l_col; /*-f_col;*/
                capacite = hauteur * longueur;

                /*Declaration des tableaux de synthese*/
                if ((valeur = (void*) MALLOC((capacite + 1) * sizeof(double))) == NULL)
                {
                    goto ErrL;
                }
                if ((*chainesind = (int *) MALLOC((capacite + 1) * sizeof(int))) == NULL)
                {
                    goto ErrL;
                }
                for (i = 0; i <= capacite; i++)
                {
                    (*chainesind)[i] = 0;
                    valeur[i] = NaN;
                }
                break;
            case 6:/* FORMULA*/
                C2F(mgetnc) (fd, (void*) &row, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*) &col, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                // Check col and row are in bounds
                if ((col >= longueur) || (row >= hauteur))
                {
                    *err = 2;
                    goto ErrL;
                }
                C2F(mgetnc) (fd, (void*) &xf, &one, typ_ushort, err);
                if (*err > 0)
                {
                    goto ErrL;
                }

                C2F(mgetnc) (fd, (void*) &resultat, &one, typ_double, err);
                if (*err > 0)
                {
                    goto ErrL;
                }

                valeur[(col * hauteur + row)] = resultat;

                C2F(mgetnc) (fd, (void*)&optionflag, &one, typ_short, err);
                if (*err > 0)
                {
                    goto ErrL;
                }

                C2F(mgetnc) (fd, (void*) &formula_notused, &one, typ_int, err);
                if (*err > 0)
                {
                    goto ErrL;
                }

                /*Formuled data*/

                /*taille=Len-2-2-2-8-2-4;
                  char formuladata[taille];
                  C2F(mgetnc) (fd, (void*) formuladata, &taille, typ_char, err);
                  if (*err > 0) goto ErrL;*/

                break;
        }
        pos = pos + 4 + Len;
    }

    *cur_pos = (int)pos;
    return;
ErrL:
    {
        FREE(sheetname);
        FREE(valeur);
        FREE(*chainesind);
        if (*err == 0)
        {
            *err = 1;    /* malloc problem */
        }
        else
        {
            *err = 2;    /* read problem */
        }
        return;
    }
}


void xls_open(int *err, int *fd, char ***sst, int *ns, char ***Sheetnames, int** Abspos, int *nsheets)
{
    /* if opt==1 it is supposed that the current file position is at the beginning of oleheader
     * if opt==0 it is supposed that the current file position is at the  beginning of workbook stream
     */

    /* we suppose that the ole file as a simple structure:
     * Workbook stream should follows immediately the header
     * and is strored in sequential sections
     */

    /*return *err:
      0 = OK
      1 = not an OLE file
      2 = no Workbook included
      3 = memory allocation problem
      4 = incorrect file
      5 = not a BIFF8 xls file
     */
    /*---------------D�claration Des Variables*--------------------*/
    int k, one = 1;
    long long cur_pos, init_pos;
    unsigned short Opcode, Len;
    /*BOF data*/
    int BOFData[7]; /*[BIFF  Version DataType Identifier Year HistoryFlags LowestXlsVersion]*/
    *nsheets = 0;
    *err = 0;
    /*---------------D�claration Des Variables*--------------------*/

    /*  if (get_oleheader(fd)) {
      *err=1;
      return;
      }*/
    cur_pos = mtell(*fd);
    init_pos = cur_pos;

    /* first record should be a BOF */
    getBOF(fd , BOFData, err);
    if (*err > 0)
    {
        return;
    }

    if (BOFData[0] < 0) /* not a BOF */
    {
        *err = 4;
        return;
    }
    if (BOFData[0] != 8)   /* not a BIFF8 */
    {
        *err = 5;
        return;
    }

    cur_pos = mtell(*fd);
    if (cur_pos < 0)
    {
        goto Err2;
    }

    /* loops on records till an EOF is found */
    while (1)
    {
        *err = mseek(*fd, cur_pos, SEEK_SET);
        if (*err > 0)
        {
            goto Err2;
        }
        /*Enregistrement de l'Opcode et de la Len du tag*/
        C2F(mgetnc) (fd, &Opcode, &one, typ_ushort, err);
        if (*err > 0)
        {
            goto Err2;
        }
        C2F(mgetnc) (fd, &Len, &one, typ_ushort, err);
        if (*err > 0)
        {
            goto Err2;
        }

        switch (Opcode)
        {
            case 10: /*EOF*/
                cur_pos = cur_pos + 4 + Len;
                return ;
            case 133: /* Boundsheets */
                getBoundsheets(fd, Sheetnames, Abspos, nsheets, &cur_pos, err);
                for (k = 0; k < *nsheets; k++)
                {
                    (*Abspos)[k] += (int)init_pos;
                }
                if (*err > 0)
                {
                    return;
                }
                break;
            case 252: /* SST= Shared String table*/
                getSST(fd, Len, BOFData[0], ns, sst, err);
                if (*err > 0)
                {
                    return;
                }
                cur_pos = cur_pos + 4 + Len;
                break;
            default:
                cur_pos = cur_pos + 4 + Len;
        }
    }

Err2:
    *err = 4; /* read problem */
    return;

}

static double NumFromRk2(long rk)
{
    double num;
    if (rk & 0x02)
    {
        /* int*/
        num = (double) (rk >> 2);
    }
    else
    {
        /* hi words of IEEE num*/
        *((int *)&num + 1) = rk & 0xfffffffc;
        *((int *)&num) = 0;
    }
    if (rk & 0x01)
        /* divide by 100*/
    {
        num /= 100;
    }
    return num;
}

static void getBOF(int *fd , int* Data, int *err)
{
    /* return Data a vector [BIFF  Version DataType Identifier Year HistoryFlags LowestXlsVersion]
     * works for BIFF2 to BIFF8 records */
    int BIFF;
    short Version;
    short DataType;
    short Identifier = 0;
    short Year = 0;
    int HistoryFlags = 0;
    int LowestXlsVersion = 0;

    unsigned short Opcode;
    unsigned short Len;
    int one = 1;

    C2F(mgetnc) (fd, (void*)&Opcode, &one, typ_ushort, err);
    if (*err > 0)
    {
        return;
    }
    C2F(mgetnc) (fd, (void*)&Len, &one, typ_ushort, err);
    if (*err > 0)
    {
        return;
    }

    switch (Opcode)
    {
        case 2057:     /*Begin of file, BOF for BIFF5 BIFF7 BIFF8 BIFF8X*/
            C2F(mgetnc) (fd, (void*)&Version, &one, typ_short, err);
            if (*err > 0)
            {
                return;
            }
            C2F(mgetnc) (fd, (void*)&DataType, &one, typ_short, err);
            if (*err > 0)
            {
                return;
            }
            C2F(mgetnc) (fd, (void*)&Identifier, &one, typ_short, err);
            if (*err > 0)
            {
                return;
            }
            C2F(mgetnc) (fd, (void*)&Year, &one, typ_short, err);
            if (*err > 0)
            {
                return;
            }
            if (Len == 16)
            {
                C2F(mgetnc) (fd, (void*)&HistoryFlags, &one, typ_int, err);
                if (*err > 0)
                {
                    return;
                }
                C2F(mgetnc) (fd, (void*)&LowestXlsVersion, &one, typ_int, err);
                if (*err > 0)
                {
                    return;
                }
                BIFF = 8;
                if (Version != 1536)
                {
                    return;
                }
            }
            else
            {
                BIFF = 7;
            }
            break;
        case 1033 : /*Interpr�tation du BIFF4  0409 H*/
            C2F(mgetnc) (fd, (void*)&Version, &one, typ_short, err);
            if (*err > 0)
            {
                return;
            }
            C2F(mgetnc) (fd, (void*)&DataType, &one, typ_short, err);
            if (*err > 0)
            {
                return;
            }
            BIFF = 4;
            break;
        case 521 : /*Interpr�tation du BIFF3  0209 H*/
            C2F(mgetnc) (fd, (void*)&Version, &one, typ_short, err);
            if (*err > 0)
            {
                return;
            }
            C2F(mgetnc) (fd, (void*)&DataType, &one, typ_short, err);
            if (*err > 0)
            {
                return;
            }
            BIFF = 3;
            break;
        case 9 : /*Interpr�tation du BIFF2  0009 H*/
            C2F(mgetnc) (fd, (void*)&Version, &one, typ_short, err);
            if (*err > 0)
            {
                return;
            }
            C2F(mgetnc) (fd, (void*)&DataType, &one, typ_short, err);
            if (*err > 0)
            {
                return;
            }
            BIFF = 2;
            break;
        default:
            BIFF = -1; /* not a BOF record */
            Version = 0;
            DataType = 0;
    }
    Data[0] = BIFF;
    Data[1] = Version;
    Data[2] = DataType;
    Data[3] = Identifier;
    Data[4] = Year;
    Data[5] = HistoryFlags;
    Data[6] = LowestXlsVersion;

}

static void getSST(int *fd, short Len, int BIFF, int *ns, char ***sst, int *err)
{
    int i = 0, one = 1;
    /* SST data */
    int ntot = 0; /*total number of strings */
    int nm = 0;/*Number of following strings*/
    short count = 0;

    *ns = 0;
    *sst = NULL;

    if (BIFF == 8)
    {
        /*Total number of strings in the workbook*/
        C2F(mgetnc) (fd, (void*)&ntot, &one, typ_int, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        C2F(mgetnc) (fd, (void*)&nm, &one, typ_int, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        *ns = nm;
        count += 8;
        if (nm != 0)
        {
            if ( (*sst = (char **)MALLOC(nm * sizeof(char*))) == NULL)
            {
                goto ErrL;
            }
            for (i = 0; i < nm; i++)
            {
                (*sst)[i] = NULL;
            }
            for (i = 0; i < nm; i++) /* LOOP ON STRINGS */
            {
                *err = i; /*for debug*/
                getString(fd, &count, &Len, 1, &((*sst)[i]), err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                /*printf("i=%d, %s\n",i,(*sst)[i]);*/
            }
        }
    }
    return;
ErrL:
    if (*sst != NULL)
    {
        for (i = 0; i < nm; i++)
            if ( (*sst)[i] != NULL )
            {
                FREE((*sst)[i]);
            }
        FREE(*sst);
    }

    if (*err == 0)
    {
        *err = 3;    /* malloc problem */
    }
    else
    {
        *err = 4;    /* read problem */
    }
}

static void getString(int *fd, short *PosInRecord, short *RecordLen, int flag, char **str, int *err)
{
    short ln = 0;
    short Opcode = 0;/* to store tag information */
    int BytesToBeRead = 0, one = 1, strindex = 0;
    char OptionFlag = 0;
    int sz = 0; /* for extended string data */
    short rt = 0;/* for rich string data */
    int UTFEncoding = 0, extendedString = 0, richString = 0;
    int j = 0, l1 = 0;

    *str = (char *)NULL;
    *err = 0;
    ln = 0;

    /*check for continue tag */
    if (flag && (*PosInRecord == *RecordLen)) /* data limit encountered */
    {
        /*check for continue tag */
        /*lecture de l'Opcode et de la RecordLen du tag*/
        C2F(mgetnc) (fd, &Opcode, &one, typ_ushort, err);
        if ((*err > 0) || (Opcode != 60))
        {
            goto ErrL;
        }
        C2F(mgetnc) (fd, RecordLen, &one, typ_ushort, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        *PosInRecord = 0;
    }

    /* get the number of characters included in the string (number of bytes or number of couple of bytes) */
    if (flag)   /* getString called by getSST */
    {
        C2F(mgetnc) (fd, (void*)&ln, &one, typ_short, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        *PosInRecord += 2;
    }
    else   /* getString called by getBoundsheets */
    {
        C2F(mgetnc) (fd, (void*)&ln, &one, typ_char, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        *PosInRecord += 1;
    }

    /*get the encoding options */
    C2F(mgetnc) (fd, (void*)&OptionFlag, &one, typ_char, err);
    if (*err > 0)
    {
        goto ErrL;
    }
    *PosInRecord += 1;

    UTFEncoding = (OptionFlag & 0x01) == 1;
    extendedString = (OptionFlag & 0x04) != 0;
    richString = (OptionFlag & 0x08) != 0;

    if (richString)   /*richString*/
    {
        C2F(mgetnc) (fd, (void*)&rt, &one, typ_short, err);
        *PosInRecord += 2;
        if (*err > 0)
        {
            goto ErrL;
        }
    }

    if (extendedString)  /* extendedString */
    {
        C2F(mgetnc) (fd, (void*)&sz, &one, typ_int, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        *PosInRecord += 4;
    }

    /* number of bytes to be read */
    BytesToBeRead = (UTFEncoding) ? ln * 2 : ln;


    if ((*str = (char*) MALLOC((BytesToBeRead + 1) * sizeof(char))) == NULL)
    {
        goto ErrL;
    }
    /* read the bytes */

    if (!flag || (*PosInRecord + BytesToBeRead <= *RecordLen))
    {
        /* all bytes are in the same record */
        C2F(mgetnc) (fd, (void*)*str, &BytesToBeRead, typ_char, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        *PosInRecord += (short)BytesToBeRead;
    }
    else  /* char stream contains at least one "continue" */
    {
        int bytesRead = *RecordLen - *PosInRecord; /* number of bytes before continue */
        strindex = 0; /*current position in str*/
        /* read bytes before the "continue"  */
        /* according to documentation  bytesRead should be strictly positive */
        C2F(mgetnc) (fd, (void*)(*str + strindex), &bytesRead, typ_char, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        strindex += bytesRead;
        *PosInRecord += (short)bytesRead;
        while (BytesToBeRead - bytesRead > 0)
        {
            /*"continue" tag assumed, verify */
            C2F(mgetnc) (fd, &Opcode, &one, typ_ushort, err);
            if ((*err > 0) || (Opcode != 60))
            {
                goto ErrL;
            }
            C2F(mgetnc) (fd, RecordLen, &one, typ_ushort, err);
            if (*err > 0)
            {
                goto ErrL;
            }
            *PosInRecord = 0;
            /* encoding option may change !!!! */
            C2F(mgetnc) (fd, (void*)&OptionFlag, &one, typ_char, err);
            if (*err > 0)
            {
                goto ErrL;
            }
            *PosInRecord += 1;

            if ((!UTFEncoding && (OptionFlag == 0)) || (UTFEncoding && (OptionFlag != 0)))
            {
                /*string encoding does not change */
                l1 = Min(BytesToBeRead - bytesRead, *RecordLen - *PosInRecord);
                C2F(mgetnc) (fd, (void*)(*str + strindex), &l1, typ_char, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                bytesRead += l1;
                strindex += l1;
                *PosInRecord += (short)l1;
            }
            else if (UTFEncoding && (OptionFlag  == 0))
            {
                /* character  encoding changes from twobytes to a single byte*/
                /* may this happen ???? */
                l1 = Min(BytesToBeRead - bytesRead, *RecordLen - *PosInRecord);
                for (j = 0; j < l1; j++)
                {
                    C2F(mgetnc) (fd, (void*)(*str + strindex), &one, typ_char, err);
                    if (*err > 0)
                    {
                        goto ErrL;
                    }
                    (*str)[strindex + 1] = '\0';
                    strindex += 2;
                    *PosInRecord += 2;
                    UTFEncoding = 0;
                }
            }
            else
            {
                /* character encoding changes from a single byte to two bytes */
                /* first, convert read characters to two bytes*/
                char *str1 = *str;
                strindex = 0;
                str = (char**) MALLOC((2 * BytesToBeRead + 1) * sizeof(char*));
                if (str == NULL)
                {
                    goto ErrL;
                }
                for (j = 0; j < bytesRead; j++)
                {
                    (*str)[strindex] = str1[j];
                    (*str)[strindex + 1] = '\0';
                    strindex += 2;
                }
                FREE(str1);
                BytesToBeRead = BytesToBeRead * 2;
                bytesRead = bytesRead * 2;
                /* read following two bytes characters */
                l1 = Min((BytesToBeRead - bytesRead) * 2, *RecordLen - *PosInRecord);
                C2F(mgetnc) (fd, (void*)(*str + strindex), &l1, typ_char, err);
                if (*err > 0)
                {
                    goto ErrL;
                }
                bytesRead += l1;
                strindex += l1;
                *PosInRecord += (short)l1;
                UTFEncoding = 1;
            }

        }

    } /*all character read */

    /* For extended strings, skip over the extended string data*/
    /* may continuation records appear here? */
    l1 = 4 * rt;
    if (richString)
    {
        long long lll1 = (long long)l1;
        *err = mseek(*fd, lll1, SEEK_CUR);
        *PosInRecord += (short)l1;
    }
    if (extendedString)
    {
        long long llsz = (long long)sz;
        *err = mseek(*fd, llsz, SEEK_CUR);
        *PosInRecord += (short)sz;
    }

    /* add string terminaison */
    if (UTFEncoding)
    {
        /* Scilab currently do not support unicode, so we remove the second byte*/
        strindex = 0;
        for (j = 0; j < BytesToBeRead; j += 2)
        {
            (*str)[strindex] = (*str)[j];
            strindex++;
        }
        BytesToBeRead = BytesToBeRead / 2;
    }
    (*str)[BytesToBeRead] = '\0';


    return;
ErrL:
    if (*err == 0)
    {
        FREE(*str);
        *err = 3; /* malloc problem */
    }
    else
    {
        *err = 4;    /* read problem */
    }
}

static void getBoundsheets(int * fd, char ***Sheetnames, int** Abspos, int *nsheets, long long *cur_pos, int *err)
{
    /* the global workbook contains a sequence of boudsheets this procedure reads all
    * the sequence and returns a vector o sheetnames, a vector of absolute sheet positions*/
    int abspos; /* Absolute stream position of BoF*/
    char visibility, sheettype; /*Visiblity , Sheet type*/
    long long pos;
    unsigned short Opcode;
    unsigned short Len;
    int one = 1;
    int ns, i;

    *Sheetnames = (char **)NULL;
    *Abspos = (int *)NULL;
    *err = 0;

    /* memorize the first boundsheet beginning */
    pos = *cur_pos;
    /* Count number of boundsheets */
    ns = 0;
    while (1)
    {
        *err = mseek(*fd, *cur_pos, SEEK_SET);
        if (*err > 0)
        {
            goto ErrL;
        }
        C2F(mgetnc) (fd, &Opcode, &one, typ_ushort, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        C2F(mgetnc) (fd, &Len, &one, typ_ushort, err);
        if (*err > 0)
        {
            goto ErrL;
        }
        if (Opcode == 133)
        {
            C2F(mgetnc) (fd, (void*)&abspos, &one, typ_int, err);
            if (*err > 0)
            {
                goto ErrL;
            }
            C2F(mgetnc) (fd, (void*)&visibility, &one, typ_char, err);
            if (*err > 0)
            {
                goto ErrL;
            }
            C2F(mgetnc) (fd, (void*)&sheettype, &one, typ_char, err);
            if (sheettype == 0) /* worksheet */
            {
                ns++;
            }
            *cur_pos = *cur_pos + 4 + Len;
        }
        else
        {
            break;
        }

    }

    *nsheets = ns;
    /*alloc the Sheetnames ans Abspos arrays */
    if ( (*Sheetnames = (char **)MALLOC(ns * sizeof(char*))) == NULL)
    {
        goto ErrL;
    }
    if ( (*Abspos = (int *)MALLOC(ns * sizeof(int))) == NULL)
    {
        goto ErrL;
    }

    /* rescan boundsheet sequence to get the data */
    *cur_pos = pos;
    i = -1;
    while (1)
    {
        *err = mseek(*fd, *cur_pos, SEEK_SET);
        if (*err > 0)
        {
            goto ErrL;
        }
        C2F(mgetnc) (fd, &Opcode, &one, typ_ushort, err);
        C2F(mgetnc) (fd, &Len, &one, typ_ushort, err);
        if (Opcode == 133)
        {
            C2F(mgetnc) (fd, (void*)&abspos, &one, typ_int, err);
            if (*err > 0)
            {
                goto ErrL;
            }
            C2F(mgetnc) (fd, (void*)&visibility, &one, typ_char, err);
            if (*err > 0)
            {
                goto ErrL;
            }
            C2F(mgetnc) (fd, (void*)&sheettype, &one, typ_char, err);
            if (sheettype == 0) /* worksheet */
            {
                short count = 0;
                i++;
                (*Abspos)[i] = abspos;
                getString(fd, &count, (short *) &Len, 0, &((*Sheetnames)[i]), err);
                if (*err > 0)
                {
                    goto ErrL;
                }
            }
            *cur_pos = *cur_pos + 4 + Len;
        }
        else
        {
            break;
        }

    }
    return;
ErrL:
    if (*Sheetnames != NULL)
    {
        for (i = 0; i < ns; i++)
            if ( (*Sheetnames)[i] != NULL )
            {
                FREE((*Sheetnames)[i]);
            }
        FREE(*Sheetnames);
    }
    FREE(*Abspos);
    if (*err == 0)
    {
        *err = 3;    /* malloc problem */
    }
    else
    {
        *err = 4;    /* read problem */
    }
}

/**
 ** Bruno : Defined but not used... so what !!!!!!!!!
 **
 static int get_oleheader(int *fd)
 {
 unsigned char MAGIC[8] = { 0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1 };
 unsigned char header[512];
 int c,ierr;

 C2F(mgetnc) (fd, (void *)header,(c=512,&c), typ_uchar, &ierr);
 if (ierr !=0)  return 1;
 if (memcmp (header, MAGIC, sizeof (MAGIC)) != 0) return 1;
 return 0;
 }
**/
