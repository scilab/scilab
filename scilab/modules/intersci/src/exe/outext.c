/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2000-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <stdlib.h>

#include "intersci-n.h"
#include "outext.h"

/***********************************************
 * Output of external variables
 **********************************************/

/* global variables */

extern int icre ;    /* incremental counter for variable creation */
extern int indent ; /* incremental counter for code indentation */
extern int pass ; /* flag for couting pass on code generation */

static char str[MAXNAM];
static char str1[MAXNAM];
static char str2[MAXNAM];
static char str3[MAXNAM];

void OutExtCOLUMN(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str2, "1");
    strcpy(str1, Forname2Int(variables[var->el[0] - 1], 0));
    OutExtCommon(f, var, insidelist, nel);
}

void OutExtROW(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str1, "1");
    strcpy(str2, Forname2Int(variables[var->el[0] - 1], 0));
    OutExtCommon(f, var, insidelist, nel);
}

void OutExtVECTOR(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str1, "1");
    strcpy(str2, Forname2Int(variables[var->el[0] - 1], 0));
    OutExtCommon(f, var, insidelist, nel);
}

void OutExtMATRIX(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str1, Forname2Int(variables[var->el[0] - 1], 0));
    strcpy(str2, Forname2Int(variables[var->el[1] - 1], 0));
    OutExtCommon(f, var, insidelist, nel);
}

void OutExtSCALAR(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str1, "1");
    strcpy(str2, "1");
    OutExtCommon(f, var, insidelist, nel);
}

void OutExtCommon(FILE *f, VARPTR var, int insidelist, int nel)
{
    if ( insidelist != 0)
    {
        int iout = GetExistOutVar();
        VARPTR vout = variables[iout - 1];
        AddDeclare(DEC_INT, "lrs");
        if ( strncmp(var->fexternal, "cintf", 4) == 0 ||
                strncmp(var->fexternal, "cboolf", 5) == 0 ||
                strncmp(var->fexternal, "cdoublef", 7) == 0 ||
                strncmp(var->fexternal, "cfloatf", 6) == 0 ||
                strncmp(var->fexternal, "ccharf", 5) == 0)
        {
            if (isdigit(str1[0]) != 0)
            {
                if (isdigit(str2[0]) != 0)
                {
                    AddDeclare(DEC_INT, "loc1");
                    AddDeclare(DEC_INT, "loc2");
                    Fprintf(f, indent, "CreateListVarFromPtr(%d,%d,\"%s\",(loc1=%s,&loc1),(loc2=%s,&loc2),&le%d);\n",
                            vout->stack_position, nel,
                            SGetExtForTypeAbrev(var),
                            str1, str2, var->stack_position);
                }
                else
                {
                    AddDeclare(DEC_INT, "loc1");
                    Fprintf(f, indent, "CreateListVarFromPtr(%d,%d,\"%s\",(loc1=%s,&loc1),%s,&le%d);\n",
                            vout->stack_position, nel,
                            SGetExtForTypeAbrev(var),
                            str1, str2, var->stack_position);
                }
            }
            else
            {
                if (isdigit(str2[0]) != 0)
                {
                    AddDeclare(DEC_INT, "loc2");
                    Fprintf(f, indent, "CreateListVarFromPtr(%d,%d,\"%s\",%s,(loc2=%s,&loc2),&le%d);\n",
                            vout->stack_position, nel,
                            SGetExtForTypeAbrev(var),
                            str1, str2, var->stack_position);
                }
                else
                {
                    Fprintf(f, indent, "CreateListVarFromPtr(%d,%d,\"%s\",%s,%s,&le%d);\n",
                            vout->stack_position, nel,
                            SGetExtForTypeAbrev(var),
                            str1, str2, var->stack_position);
                }
            }
            if ( strcmp(var->fexternal, "cintf") == 0 ||
                    strcmp(var->fexternal, "cboolf") == 0 ||
                    strcmp(var->fexternal, "cdoublef") == 0 ||
                    strcmp(var->fexternal, "cfloatf") == 0 ||
                    strcmp(var->fexternal, "ccharf") == 0 )
                Fprintf(f, indent, "if (%s*%s != 0) FreePtr(&le%d);\n",
                        (str1[0] == '&') ? str1 + 1 : str1,
                        (str2[0] == '&') ? str2 + 1 : str2,
                        var->stack_position);
        }
        else /* not a cint* cbool* cdouble* cfloat* cchar* */
        {
            Fprintf(f, indent, "CreateListVar(%d,\"%s\",%s,%s,&lrs);\n", icre,
                    SGetExtForTypeAbrev(var),
                    str1, str2);
        }
    }
    else /* not a list element */
    {
        /** CreateVarFromPtr peut etre utilise ds plusieurs cas **/
        if ( strncmp(var->fexternal, "cintf", 4) == 0 ||
                strncmp(var->fexternal, "cboolf", 5) == 0 ||
                strncmp(var->fexternal, "cdoublef", 7) == 0 ||
                strncmp(var->fexternal, "cfloatf", 6) == 0 ||
                strncmp(var->fexternal, "ccharf", 5) == 0)
        {
            if (isdigit(str1[0]) != 0)
            {
                if (isdigit(str2[0]) != 0)
                {
                    AddDeclare(DEC_INT, "loc1");
                    AddDeclare(DEC_INT, "loc2");
                    Fprintf(f, indent, "CreateVarFromPtr(%d,\"%s\",(loc1=%s,&loc1),(loc2=%s,&loc2),&le%d);\n", icre,
                            SGetExtForTypeAbrev(var),
                            str1, str2, var->stack_position);
                }
                else
                {
                    AddDeclare(DEC_INT, "loc1");
                    Fprintf(f, indent, "CreateVarFromPtr(%d,\"%s\",(loc1=%s,&loc1),%s,&le%d);\n", icre,
                            SGetExtForTypeAbrev(var),
                            str1, str2, var->stack_position);
                }
            }
            else
            {
                if (isdigit(str2[0]) != 0)
                {
                    AddDeclare(DEC_INT, "loc2");
                    Fprintf(f, indent, "CreateVarFromPtr(%d,\"%s\",%s,(loc2=%s,&loc2),&le%d);\n", icre,
                            SGetExtForTypeAbrev(var),
                            str1, str2, var->stack_position);
                }
                else
                {
                    Fprintf(f, indent, "CreateVarFromPtr(%d,\"%s\",%s,%s,&le%d);\n", icre,
                            SGetExtForTypeAbrev(var),
                            str1, str2, var->stack_position);
                }
            }
            if ( strcmp(var->fexternal, "cintf") == 0 ||
                    strcmp(var->fexternal, "cboolf") == 0 ||
                    strcmp(var->fexternal, "cdoublef") == 0 ||
                    strcmp(var->fexternal, "cfloatf") == 0 ||
                    strcmp(var->fexternal, "ccharf") == 0 )
                Fprintf(f, indent, "if (%s*%s != 0) FreePtr(&le%d);\n",
                        (str1[0] == '&') ? str1 + 1 : str1,
                        (str2[0] == '&') ? str2 + 1 : str2,
                        var->stack_position);
        }
        else /* not a cint* cbool* cdouble* cfloat* cchar* */
        {
            Fprintf(f, indent, "CreateVar(%d,\"%s\",%s,%s,&lrs);\n", icre,
                    SGetExtForTypeAbrev(var),
                    str1, str2);
        }
        Fprintf(f, indent, "LhsVar(%d)=%d;\n", var->out_position, icre);
        icre++;
    }
    if ( strncmp(var->fexternal, "cintf", 4) == 0 ||
            strncmp(var->fexternal, "cboolf", 5) == 0 ||
            strncmp(var->fexternal, "cdoublef", 7) == 0 ||
            strncmp(var->fexternal, "cfloatf", 6) == 0 ||
            strncmp(var->fexternal, "ccharf", 5) == 0)
    {
    }
    else /* not a cint* cbool* cdouble* cfloat* cchar* */
    {
        Fprintf(f, indent, "----outext3 %s\n", var->fexternal);
        AddDeclare(DEC_INT, "lrs");
        AddDeclare(DEC_INT, "mn");
        Fprintf(f, indent, "mn=%s*%s;\n", str1 + 1, str2 + 1);
        Fprintf(f, indent, "C2F(%s)(&mn,&le%d,%s(lrs));\n", var->fexternal,
                var->stack_position, SGetExtForTypeStack(var));
    }
}


void OutExtSTRING(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str1, Forname2Int(variables[var->el[0] - 1], 0));
    strcpy(str2, "1");
    if ( strcmp(var->fexternal, "ccharf") == 0 || strcmp(var->fexternal, "cchar") == 0 )
    {
        if ( insidelist != 0)
        {
            int iout = GetExistOutVar();
            VARPTR vout = variables[iout - 1];
            AddDeclare(DEC_INT, "un=1");
            strcpy(str2, "&un");
            Fprintf(f, indent, "CreateListVarFromPtr(%d,%d,\"c\",%s,%s,&le%d);\n",
                    vout->stack_position, nel, str1, str2, var->stack_position);
        }
        else
        {
            AddDeclare(DEC_INT, "un=1");
            strcpy(str2, "&un");
            Fprintf(f, indent, "CreateVarFromPtr(%d,\"c\",%s,%s,&le%d);\n", icre,
                    str1, str2, var->stack_position);
            Fprintf(f, indent, "LhsVar(%d)=%d;\n", var->out_position, icre);
            icre++;
        }
        if ( strcmp(var->fexternal, "ccharf") == 0 )
        {
            Fprintf(f, indent, "FreePtr(&le%d);\n", var->stack_position);
        }
    }
    else
    {
        if ( insidelist != 0)
        {
            int iout = GetExistOutVar();
            VARPTR vout = variables[iout - 1];
            AddDeclare(DEC_INT, "lrs");
            /** Le X a l'air faux **/
            AddDeclare(DEC_INT, "m_u=-1");
            AddDeclare(DEC_INT, "un=1");
            strcpy(str2, "&un");
            Fprintf(f, indent, "CreateListVar(%d,%d,\"c\",%s,%s,&lrs,m_u);\n",
                    vout->stack_position, nel, str1, str2);
        }
        else
        {
            /** XXXX : a revoir CreateVarFromPtr peut etre utilise ds plusieurs cas **/
            AddDeclare(DEC_INT, "un=1");
            strcpy(str2, "&un");
            Fprintf(f, indent, "CreateVar(%d,\"c\",%s,%s,&lrs);\n", icre,
                    str1, str2);
            Fprintf(f, indent, "LhsVar(%d)=%d;\n", var->out_position, icre);
            icre++;
        }
        AddDeclare(DEC_INT, "lrs");
        AddDeclare(DEC_INT, "mn");
        Fprintf(f, indent, "mn=%s*%s;\n", str1 + 1, str2 + 1);
        Fprintf(f, indent, "C2F(%s)(&mn,&le%d,cstk(lrs));\n", var->fexternal,
                var->stack_position);
    }
}


void OutExtSPARSE(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str1, Forname2Int(variables[var->el[0] - 1], 0));
    strcpy(str2, Forname2Int(variables[var->el[1] - 1], 0));
    sprintf(str3, "%d", var->stack_position);
    if ( insidelist != 0)
    {
        int iout = GetExistOutVar();
        VARPTR vout = variables[iout - 1];
        Fprintf(f, indent, "CreateListVarFromPtr(%d,\"s\",&(S%s->m),&(S%s->n),S%s);\n",
                vout->stack_position, nel, str3, str3, str3);
    }
    else
    {
        Fprintf(f, indent, "LhsVar(%d)=%d;\n", var->out_position, icre);
        Fprintf(f, indent, "CreateVarFromPtr(%d,\"s\",&(S%s->m),&(S%s->n),S%s);\n",
                icre, str3, str3, str3);
        icre++;
    }
    if ( strcmp(var->fexternal, "csparsef") == 0)
    {
        Fprintf(f, indent, "FreeSparse(S%s);\n", str3);
        /* Fprintf(f,indent,"/ *call %s()* /\n",var->fexternal,str); */
    }
}

/**** OK ****/

void OutExtIMATRIX(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str1, Forname2Int(variables[var->el[0] - 1], 0));
    strcpy(str2, Forname2Int(variables[var->el[1] - 1], 0));
    strcpy(str3, Forname2Int(variables[var->el[2] - 1], 0));
    if ( insidelist != 0)
    {
        int iout = GetExistOutVar();
        VARPTR vout = variables[iout - 1];
        AddDeclare(DEC_INT, "lrs");
        AddDeclare(DEC_INT, "lcs");
        /** Le X a l'air faux **/
        AddDeclare(DEC_INT, "m_u=-1");
        Fprintf(f, indent, "CreateListCVar(%d,%d,\"%s\",%s,%s,%s,&lrs,&lcs,m_u,m_u);\n",
                vout->stack_position, nel,
                SGetExtForTypeAbrev(var),
                str3, str1, str2);
    }
    else
    {
        /** XXXX : a revoir CreateVarFromPtr peut etre utilise ds plusieurs cas **/
        Fprintf(f, indent, "CreateCVar(%d,\"%s\",%s,%s,%s,&lrs,&lcs);\n", icre,
                SGetExtForTypeAbrev(var),
                str3, str1, str2);
        Fprintf(f, indent, "LhsVar(%d)=%d;\n", var->out_position, icre);
        icre++;
    }
    AddDeclare(DEC_INT, "lrs");
    AddDeclare(DEC_INT, "lcrs");
    AddDeclare(DEC_INT, "mnit");
    Fprintf(f, indent, "mnit=%s*%s;\n", str1 + 1, str2 + 1);
    Fprintf(f, indent, "C2F(%s)(&mnit,&ler%d,%s(lrs));\n", var->fexternal,
            var->stack_position, SGetExtForTypeStack(var));
    Fprintf(f, indent++, "if ( %s > 0 ) {\n", str3 + 1);
    Fprintf(f, indent, "C2F(%s)(&mnit,&lec%d,%s(lrs));\n", var->fexternal,
            var->stack_position, SGetExtForTypeStack(var));
    Fprintf(f, --indent, "}\n");
}


void OutExtBMATRIX(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str1, Forname2Int(variables[var->el[0] - 1], 0));
    strcpy(str2, Forname2Int(variables[var->el[1] - 1], 0));
    OutExtCommon(f, var, insidelist, nel);
}


void OutExtBMATRIX1(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str1, Forname2Int(variables[var->el[0] - 1], 0));
    strcpy(str2, Forname2Int(variables[var->el[1] - 1], 0));
    if ( insidelist != 0)
    {
        AddDeclare(DEC_LOGICAL, "listcrebmat");
        Fprintf(f, indent, "if(.not.listcrebmat(fname,top,%d,lw,%s,%s,lrs)) return\n",
                nel, str1, str2);
    }
    else
    {
        AddDeclare(DEC_LOGICAL, "crebmat");
        Fprintf(f, indent, "if(.not.crebmat(fname,top,%s,%s,lrs)) return\n", str1, str2);
    }
    sprintf(str, "lr%d", var->stack_position);
    Fprintf(f, indent, "call %s(%s*%s,istk(%s),istk(lrs))\n", var->fexternal, str1, str2, str);
}

void OutExtPOLYNOM(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str1, Forname2Int(variables[var->el[0] - 1], 0));
    strcpy(str2, variables[var->el[1] - 1]->name);
    Fprintf(f, indent, "err=sadr(ilw+10)+%s-lstk(bot)\n", str1);
    Fprintf(f, indent, "if(err .gt. 0) then\n");
    Fprintf(f, indent, "call error(17)\n");
    Fprintf(f, indent, "return\n");
    Fprintf(f, indent, "endif\n");
    Fprintf(f, indent, "istk(ilw)=1\n");
    Fprintf(f, indent, "istk(ilw+1)=1\n");
    Fprintf(f, indent, "istk(ilw+2)=1\n");
    Fprintf(f, indent, "istk(ilw+3)=0\n");
    Fprintf(f, indent, "call cvstr(4,'%s    ',istk(ilw+4),0)\n", str2);
    /* str2 comes from SCILAB input ??? */
    Fprintf(f, indent, "istk(ilw+8)=1\n");
    Fprintf(f, indent, "istk(ilw+9)=1+%s\n", str1);
    sprintf(str, "lr%d", var->stack_position);
    Fprintf(f, indent, "lw=sadr(ilw+10)\n");
    Fprintf(f, indent, "call %s(%s,stk(%s),stk(lw))\n",
            var->fexternal, str1, str);
    Fprintf(f, indent, "lw=lw+%s\n", str1);
}


void OutExtPOINTER(FILE *f, VARPTR var, int insidelist, int nel)
{
    if ( insidelist != 0)
    {
        printf(" %s in output list : not implemented ;", SGetSciType(var->type));
    }
    else
    {
    }
}

void OutExtSTRINGMAT(FILE *f, VARPTR var, int insidelist, int nel)
{
    strcpy(str1, Forname2Int(variables[var->el[0] - 1], 0));
    strcpy(str2, Forname2Int(variables[var->el[1] - 1], 0));
    sprintf(str, "lr%d", var->stack_position);
    Fprintf(f, indent, "call %s(stk(%s),istk(ilw),%s,%s,lstk(bot)-sadr(ilw),ierr)\n",
            var->fexternal, str, str1, str2);
    Fprintf(f, indent, "if(ierr .gt. 0) then\n");
    Fprintf(f, indent, "buf='not enough memory'\n");
    Fprintf(f, indent, "call error(1000)\n");
    Fprintf(f, indent, "return\n");
    Fprintf(f, indent, "endif\n");
    sprintf(str, "istk(ilw+4+%s*%s)-1", str1, str2);
    Fprintf(f, indent, "lw=sadr(ilw+5+%s*%s+%s)\n", str1, str2, str);
}


void OutExtANY(FILE *f, VARPTR var, int insidelist, int nel)
{
    printf("output variable \"%s\" cannot have type\n",
           var->name);
    printf("  \"WORK\", \"LIST\", \"TLIST\", \"SEQUENCE\" or \"ANY\"\n");
    exit(1);
}

typedef  struct
{
    int type;
    void (*fonc)(FILE *f, VARPTR var, int insidelist, int nel);
} OutExtRhsTab ;




OutExtRhsTab OutExtRHSTAB[] =
{
    {DIMFOREXT, OutExtANY},
    {COLUMN, OutExtCOLUMN},
    {LIST, OutExtANY},
    {TLIST, OutExtANY},
    {MATRIX, OutExtMATRIX},
    {POLYNOM, OutExtPOLYNOM},
    {ROW, OutExtROW},
    {SCALAR, OutExtSCALAR},
    {SEQUENCE, OutExtANY},
    {STRING, OutExtSTRING},
    {WORK, OutExtVECTOR},
    {EMPTY, OutExtANY},
    {ANY, OutExtANY},
    {VECTOR, OutExtVECTOR},
    {STRINGMAT, OutExtSTRINGMAT},
    {SCIMPOINTER, OutExtPOINTER},
    {IMATRIX, OutExtIMATRIX},
    {SCISMPOINTER, OutExtPOINTER},
    {SCILPOINTER, OutExtPOINTER},
    {BMATRIX, OutExtBMATRIX},
    {SCIBPOINTER, OutExtPOINTER},
    {SCIOPOINTER, OutExtPOINTER},
    {SPARSE, OutExtSPARSE}
};


/***********************************************
 * Output of external variable var
 *   if variable is outputed inside a list
 *   insidelist is set to true and nel is the number
 *   of the variable in the list
 * Convert ==> 1 if the variable has an output convertion to be done
 ***********************************************/

void WriteExternalVariableOutput(FILE *f, VARPTR var, int insidelist, int nel)
{
    (*(OutExtRHSTAB[var->type].fonc))(f, var, insidelist, nel);
}


