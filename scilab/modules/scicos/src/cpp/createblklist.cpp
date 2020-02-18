/*  Scicos
*
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*  Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#include "double.hxx"
#include "int.hxx"
#include "internal.hxx"
#include "list.hxx"
#include "string.hxx"
#include "tlist.hxx"

extern "C"
{
#include "import.h"
}

#include "createblklist.hxx"
/*--------------------------------------------------------------------------*/
static types::InternalType* allocsci(void* data, const int rows, const int cols, const int type)
{
    switch (type)
    {
        case SCSREAL_N:
        {
            types::Double* var = new types::Double(rows, cols);
            return var;
        }
        case SCSCOMPLEX_N:
        {
            types::Double* var = new types::Double(rows, cols, true);
            return var;
        }
        case SCSINT8_N:
        {
            types::Int8* var = new types::Int8(rows, cols);
            return var;
        }
        case SCSINT16_N:
        {
            types::Int16* var = new types::Int16(rows, cols);
            return var;
        }
        case SCSINT32_N:
        {
            types::Int32* var = new types::Int32(rows, cols);
            return var;
        }
        case SCSUINT8_N:
        {
            types::UInt8* var = new types::UInt8(rows, cols);
            return var;
        }
        case SCSUINT16_N:
        {
            types::UInt16* var = new types::UInt16(rows, cols);
            return var;
        }
        case SCSUINT32_N:
        {
            types::UInt32* var = new types::UInt32(rows, cols);
            return var;
        }
        default: // case SCSUNKNOW_N: pass the data by pointers
        {
            return (types::InternalType*)data;
        }
    }
}
/*--------------------------------------------------------------------------*/
static types::InternalType* vartosci(types::InternalType* pIT, void* data, const int rows, const int cols, const int type)
{
    int size = rows * cols;
    switch (type)
    {
        case SCSREAL_N:
        {
            if (!pIT->isDouble())
            {
                return pIT;
            }
            types::Double* var = pIT->getAs<types::Double>();
            size = std::min(var->getSize(), size);
            for (int i = 0; i < size; ++i)
            {
                var->get()[i] = ((types::Double::type*)data)[i];
            }
            return var;
        }
        case SCSCOMPLEX_N:
        {
            if (!pIT->isDouble())
            {
                return pIT;
            }
            types::Double* var = pIT->getAs<types::Double>();
            size = std::min(var->getSize(), size);
            for (int i = 0; i < size; ++i)
            {
                var->get()[i] = ((types::Double::type*)data)[i];
                var->getImg()[i] = ((types::Double::type*)data)[size + i];
            }
            return var;
        }
        case SCSINT8_N:
        {
            if (!pIT->isInt8())
            {
                return pIT;
            }
            types::Int8* var = pIT->getAs<types::Int8>();
            size = std::min(var->getSize(), size);
            for (int i = 0; i < size; ++i)
            {
                var->get()[i] = ((types::Int8::type*)data)[i];
            }
            return var;
        }
        case SCSINT16_N:
        {
            if (!pIT->isInt16())
            {
                return pIT;
            }
            types::Int16* var = pIT->getAs<types::Int16>();
            size = std::min(var->getSize(), size);
            for (int i = 0; i < size; ++i)
            {
                var->get()[i] = ((types::Int16::type*)data)[i];
            }
            return var;
        }
        case SCSINT32_N:
        {
            if (!pIT->isInt32())
            {
                return pIT;
            }
            types::Int32* var = pIT->getAs<types::Int32>();
            size = std::min(var->getSize(), size);
            for (int i = 0; i < size; ++i)
            {
                var->get()[i] = ((types::Int32::type*)data)[i];
            }
            return var;
        }
        case SCSUINT8_N:
        {
            if (!pIT->isUInt8())
            {
                return pIT;
            }
            types::UInt8* var = pIT->getAs<types::UInt8>();
            size = std::min(var->getSize(), size);
            for (int i = 0; i < size; ++i)
            {
                var->get()[i] = ((types::UInt8::type*)data)[i];
            }
            return var;
        }
        case SCSUINT16_N:
        {
            if (!pIT->isUInt16())
            {
                return pIT;
            }
            types::UInt16* var = pIT->getAs<types::UInt16>();
            size = std::min(var->getSize(), size);
            for (int i = 0; i < size; ++i)
            {
                var->get()[i] = ((types::UInt16::type*)data)[i];
            }
            return var;
        }
        case SCSUINT32_N:
        {
            if (!pIT->isUInt32())
            {
                return pIT;
            }
            types::UInt32* var = pIT->getAs<types::UInt32>();
            size = std::min(var->getSize(), size);
            for (int i = 0; i < size; ++i)
            {
                var->get()[i] = ((types::UInt32::type*)data)[i];
            }
            return var;
        }
        default: // case SCSUNKNOW_N: pass the data by pointers
        {
            return (types::InternalType*)data;
        }
    }
}
/*--------------------------------------------------------------------------*/
static types::InternalType* vartosci(void* data, const int rows, const int cols, const int type)
{
    return vartosci(allocsci(data, rows, cols, type), data, rows, cols, type);
}
/*--------------------------------------------------------------------------*/
types::InternalType* createblklist(const scicos_block* const Blocks, const int flag_imp, const int /*funtyp*/)
{
    const int fieldCount = 41;
    int size = 0;

    /* set string of first element of scilab Blocks tlist */
    static const char* str_blklst[] = {"scicos_block", "nevprt", "funpt", "type",
                                       "scsptr", "nz", "z", "noz",
                                       "ozsz", "oztyp", "oz", "nx",
                                       "x", "xd", "res", "nin",
                                       "insz", "inptr", "nout", "outsz",
                                       "outptr", "nevout", "evout", "nrpar",
                                       "rpar", "nipar", "ipar", "nopar",
                                       "oparsz", "opartyp", "opar", "ng",
                                       "g", "ztyp", "jroot", "label",
                                       "work", "nmode", "mode", "xprop",
                                       "uid"
                                      };

    int* xptr = nullptr; /* to retrieve xptr by import and zcptr of scicos_blocks */
    double* x = nullptr; /* ptr for x, xd and g for scicos_blocks                 */
    double* xd = nullptr;
    int* zcptr = nullptr;
    double* g = nullptr;

    if (flag_imp >= 0)
    {
        int nv, mv; /* length of data                             */
        int nblk;   /* to store number of blocks                  */
        //int ng;             /* to store number of zero cross              */
        void* ptr; /* ptr for data comming from import structure */

        /*retrieve nblk by import structure*/
        char Nblk[] = "nblk";
        getscicosvarsfromimport(Nblk, &ptr, &nv, &mv);
        nblk = ((int*)ptr)[0];

        /* retrieve ng by import structure */
        char Ng[] = "ng";
        getscicosvarsfromimport(Ng, &ptr, &nv, &mv);
        //ng = ((int*)ptr)[0];

        /*retrieve xptr by import structure*/
        char Xptr[] = "xptr";
        getscicosvarsfromimport(Xptr, &ptr, &nv, &mv);
        xptr = (int*)ptr;

        /*retrieve zcptr by import structure*/
        char Zcptr[] = "zcptr";
        getscicosvarsfromimport(Zcptr, &ptr, &nv, &mv);
        zcptr = (int*)ptr;

        /*retrieve x and xd by import structure*/
        char X[] = "x";
        getscicosvarsfromimport(X, &ptr, &nv, &mv);
        x = (double*)ptr;
        xd = &x[xptr[nblk] - 1];

        /*retrieve g by import structure*/
        char G[] = "g";
        getscicosvarsfromimport(G, &ptr, &nv, &mv);
        g = (double*)ptr;
    }

    types::TList* m = new types::TList();

    /* 1 - scicos_block */
    types::String* s = new types::String(1, fieldCount);
    for (int i = 0; i < fieldCount; ++i)
    {
        s->set(i, str_blklst[i]);
    }

    m->append(s);

    /* 2 - nevprt */
    m->append(new types::Double(static_cast<double>(Blocks->nevprt)));

    /* 3 - funpt */
    //cast function ptr to double*
    if (sizeof(voidg) >= sizeof(double))
    {
        // store N double values as the function pointer value
        size = sizeof(voidg) / sizeof(double);
    }
    else
    {
        // push at least one double
        size = 1;
    }

    types::Double* funpt = new types::Double(size, 1);
    double* d = funpt->get();
    for (int i = 0; i < size; ++i)
    {
        d[i] = (double)((long long)Blocks->funpt);
    }

    m->append(funpt);

    /* 4 - type */
    m->append(new types::Double(static_cast<double>(Blocks->type)));

    /* 5 - scsptr */
    //cast function ptr to double*
    if (sizeof(types::InternalType*) >= sizeof(double))
    {
        // store N double values as the function pointer value
        size = sizeof(types::InternalType*) / sizeof(double);
    }
    else
    {
        // push at least one double
        size = 1;
    }
    types::Double* scsptr = new types::Double(size, 1);
    d = scsptr->get();
    for (int i = 0; i < size; ++i)
    {
        d[i] = (double)((long long)Blocks->scsptr);
    }

    m->append(scsptr);

    /* 6 - nz */
    m->append(new types::Double(static_cast<double>(Blocks->nz)));

    /* 7 - z */
    m->append(vartosci(Blocks->z, Blocks->nz, 1, SCSREAL_N));

    /* 8 - noz */
    m->append(new types::Double(static_cast<double>(Blocks->noz)));

    /* 9 - ozsz */
    types::Double* ozsz = new types::Double(Blocks->noz, 1);
    d = ozsz->get();
    for (int i = 0; i < Blocks->noz; ++i)
    {
        d[i] = static_cast<double>(Blocks->ozsz[i]);
    }

    m->append(ozsz);

    /* 10 - oztyp */
    types::Double* oztyp = new types::Double(Blocks->noz, 1);
    d = oztyp->get();
    for (int i = 0; i < Blocks->noz; ++i)
    {
        d[i] = static_cast<double>(Blocks->oztyp[i]);
    }

    m->append(oztyp);

    /* 11 - ozptr */
    types::List* ozptr = new types::List();
    for (int k = 0; k < Blocks->noz; k++)
    {
        const int rows = Blocks->ozsz[k];               /* retrieve number of rows */
        const int cols = Blocks->ozsz[Blocks->noz + k]; /* retrieve number of cols */
        const int type = Blocks->oztyp[k];              /* retrieve type */
        ozptr->append(vartosci(Blocks->ozptr[k], rows, cols, type));
    }

    m->append(ozptr);

    /* 12 - nx */
    m->append(new types::Double(static_cast<double>(Blocks->nx)));

    /* 13 - x */
    if (flag_imp >= 0)
    {
        m->append(vartosci(&x[xptr[flag_imp] - 1], Blocks->nx, 1, SCSREAL_N));
    }
    else
    {
        m->append(vartosci(Blocks->x, Blocks->nx, 1, SCSREAL_N));
    }

    /* 14 - xd */
    if (flag_imp >= 0)
    {
        m->append(vartosci(&xd[xptr[flag_imp] - 1], Blocks->nx, 1, SCSREAL_N));
    }
    else
    {
        m->append(vartosci(Blocks->xd, Blocks->nx, 1, SCSREAL_N));
    }

    /* 15 - res */
    m->append(vartosci(Blocks->res, Blocks->nx, 1, SCSREAL_N));

    /* 16 - nin */
    m->append(new types::Double(static_cast<double>(Blocks->nin)));

    /* 17 - insz */
    types::Double* insz = new types::Double(3 * Blocks->nin, 1);
    d = insz->get();
    for (int i = 0; i < 3 * Blocks->nin; ++i)
    {
        d[i] = static_cast<double>(Blocks->insz[i]);
    }

    m->append(insz);

    /* 18 - inptr */
    types::List* inptr = new types::List();
    for (int k = 0; k < Blocks->nin; k++)
    {
        const int rows = Blocks->insz[k];                   /* retrieve number of rows */
        const int cols = Blocks->insz[Blocks->nin + k];     /* retrieve number of cols */
        const int type = Blocks->insz[2 * Blocks->nin + k]; /* retrieve type */
        inptr->append(vartosci(Blocks->inptr[k], rows, cols, type));
    }

    m->append(inptr);

    /* 19 - nout */
    m->append(new types::Double(static_cast<double>(Blocks->nout)));

    /* 20 - outsz */
    types::Double* outsz = new types::Double(3 * Blocks->nout, 1);
    d = outsz->get();
    for (int i = 0; i < 3 * Blocks->nout; ++i)
    {
        d[i] = static_cast<double>(Blocks->outsz[i]);
    }

    m->append(outsz);
    /* 21 - outptr */
    types::List* outptr = new types::List();
    for (int k = 0; k < Blocks->nout; k++)
    {
        const int rows = Blocks->outsz[k];                    /* retrieve number of rows */
        const int cols = Blocks->outsz[Blocks->nout + k];     /* retrieve number of cols */
        const int type = Blocks->outsz[2 * Blocks->nout + k]; /* retrieve type */
        outptr->append(vartosci(Blocks->outptr[k], rows, cols, type));
    }

    m->append(outptr);

    /* 22 - nevout */
    m->append(new types::Double(static_cast<double>(Blocks->nevout)));

    /* 23 - evout */
    m->append(vartosci(Blocks->evout, Blocks->nevout, 1, SCSREAL_N));

    /* 24 - nrpar */
    m->append(new types::Double(static_cast<double>(Blocks->nrpar)));

    /* 25 - rpar */
    m->append(vartosci(Blocks->rpar, Blocks->nrpar, 1, SCSREAL_N));

    /* 26 - nipar */
    m->append(new types::Double(static_cast<double>(Blocks->nipar)));

    /* 27 - ipar */
    types::Double* ipar = new types::Double(Blocks->nipar, 1);
    d = ipar->get();
    for (int i = 0; i < Blocks->nipar; ++i)
    {
        d[i] = static_cast<double>(Blocks->ipar[i]);
    }

    m->append(ipar);

    /* 28 - nopar */
    m->append(new types::Double(static_cast<double>(Blocks->nopar)));

    /* 29 - oparsz */
    types::Double* oparsz = new types::Double(Blocks->nopar, 1);
    d = oparsz->get();
    for (int i = 0; i < Blocks->nopar; ++i)
    {
        d[i] = static_cast<double>(Blocks->oparsz[i]);
    }

    m->append(oparsz);

    /* 30 - opartyp */
    types::Double* opartyp = new types::Double(Blocks->nopar, 1);
    d = opartyp->get();
    for (int i = 0; i < Blocks->nopar; ++i)
    {
        d[i] = static_cast<double>(Blocks->opartyp[i]);
    }

    m->append(opartyp);

    /* 31 - opar */
    types::List* opar = new types::List();
    for (int k = 0; k < Blocks->nopar; k++)
    {
        const int rows = Blocks->oparsz[k];                 /* retrieve number of rows */
        const int cols = Blocks->oparsz[Blocks->nopar + k]; /* retrieve number of cols */
        const int type = Blocks->opartyp[k];                /* retrieve type */
        opar->append(vartosci(Blocks->oparptr[k], rows, cols, type));
    }

    m->append(opar);

    /* 32 - ng */
    m->append(new types::Double(static_cast<double>(Blocks->ng)));

    /* 33 - g */
    if (flag_imp >= 0)
    {
        m->append(vartosci(&g[zcptr[flag_imp] - 1], Blocks->ng, 1, SCSREAL_N));
    }
    else
    {
        m->append(vartosci(Blocks->g, Blocks->ng, 1, SCSREAL_N));
    }

    /* 34 - ztyp */
    m->append(new types::Double(static_cast<double>(Blocks->ztyp)));

    /* 35 - jroot */
    types::Double* jroot = new types::Double(Blocks->ng, 1);
    d = jroot->get();
    for (int i = 0; i < Blocks->ng; ++i)
    {
        d[i] = static_cast<double>(Blocks->jroot[i]);
    }

    m->append(jroot);

    /* 36 - label */
    m->append(new types::String(Blocks->label));

    /* 37 - work*/
    //store address as double
    m->append(new types::Double((double)((long long)Blocks->work)));

    /* 38 - nmode*/
    m->append(new types::Double(static_cast<double>(Blocks->nmode)));

    /* 39 - mode */
    types::Double* mode = new types::Double(Blocks->nmode, 1);
    d = mode->get();
    for (int i = 0; i < Blocks->nmode; ++i)
    {
        d[i] = static_cast<double>(Blocks->mode[i]);
    }

    m->append(mode);

    /* 40 - xprop */
    types::Double* xprop = new types::Double(Blocks->nx, 1);
    d = xprop->get();
    for (int i = 0; i < Blocks->nx; ++i)
    {
        d[i] = static_cast<double>(Blocks->xprop[i]);
    }

    m->append(xprop);

    /* 41 - uid */
    if (Blocks->uid)
    {
        m->append(new types::String(Blocks->uid));
    }
    else
    {
        m->append(new types::String(L""));
    }

    return m;
}
/*--------------------------------------------------------------------------*/
types::InternalType* refreshblklist(types::InternalType* pIT, const scicos_block* const Blocks, const int flag_imp, const int /*funtyp*/)
{
    if (!pIT->isTList())
    {
        return pIT;
    }
    types::TList* m = pIT->getAs<types::TList>();

    // ensure that `m` contains the needed elements; if not discard the sync
    if (m->getSize() != 41)
    {
        return m;
    }

    int* xptr = nullptr; /* to retrieve xptr by import and zcptr of scicos_blocks */
    double* x = nullptr; /* ptr for x, xd and g for scicos_blocks                 */
    double* xd = nullptr;
    int* zcptr = nullptr;
    double* g = nullptr;

    if (flag_imp >= 0)
    {
        int nv, mv; /* length of data                             */
        int nblk;   /* to store number of blocks                  */
        //int ng;             /* to store number of zero cross              */
        void* ptr; /* ptr for data comming from import structure */

        /*retrieve nblk by import structure*/
        char Nblk[] = "nblk";
        getscicosvarsfromimport(Nblk, &ptr, &nv, &mv);
        nblk = ((int*)ptr)[0];

        /* retrieve ng by import structure */
        char Ng[] = "ng";
        getscicosvarsfromimport(Ng, &ptr, &nv, &mv);
        //ng = ((int*)ptr)[0];

        /*retrieve xptr by import structure*/
        char Xptr[] = "xptr";
        getscicosvarsfromimport(Xptr, &ptr, &nv, &mv);
        xptr = (int*)ptr;

        /*retrieve zcptr by import structure*/
        char Zcptr[] = "zcptr";
        getscicosvarsfromimport(Zcptr, &ptr, &nv, &mv);
        zcptr = (int*)ptr;

        /*retrieve x and xd by import structure*/
        char X[] = "x";
        getscicosvarsfromimport(X, &ptr, &nv, &mv);
        x = (double*)ptr;
        xd = &x[xptr[nblk] - 1];

        /*retrieve g by import structure*/
        char G[] = "g";
        getscicosvarsfromimport(G, &ptr, &nv, &mv);
        g = (double*)ptr;
    }

    /* 7 - z */
    m->set(6, vartosci(m->get(6), Blocks->z, Blocks->nz, 1, SCSREAL_N));

    /* 11 - ozptr */
    if (m->get(10)->isList())
    {
        types::List* ozptr = m->get(10)->getAs<types::List>();
        for (int k = 0; k < Blocks->noz; k++)
        {
            const int rows = Blocks->ozsz[k];               /* retrieve number of rows */
            const int cols = Blocks->ozsz[Blocks->noz + k]; /* retrieve number of cols */
            const int type = Blocks->oztyp[k];              /* retrieve type */
            ozptr->set(k, vartosci(ozptr->get(k), Blocks->ozptr[k], rows, cols, type));
        }
        m->set(10, ozptr);
    }

    /* 13 - x */
    if (flag_imp >= 0)
    {
        m->set(12, vartosci(m->get(12), &x[xptr[flag_imp] - 1], Blocks->nx, 1, SCSREAL_N));
    }
    else
    {
        m->set(12, vartosci(m->get(12), Blocks->x, Blocks->nx, 1, SCSREAL_N));
    }

    /* 14 - xd */
    if (flag_imp >= 0)
    {
        m->set(13, vartosci(m->get(13), &xd[xptr[flag_imp] - 1], Blocks->nx, 1, SCSREAL_N));
    }
    else
    {
        m->set(13, vartosci(m->get(13), Blocks->xd, Blocks->nx, 1, SCSREAL_N));
    }

    /* 15 - res */
    m->set(14, vartosci(m->get(14), Blocks->res, Blocks->nx, 1, SCSREAL_N));

    /* 18 - inptr */
    if (m->get(17)->isList())
    {
        types::List* inptr = m->get(17)->getAs<types::List>();
        for (int k = 0; k < Blocks->nin; k++)
        {
            const int rows = Blocks->insz[k];                   /* retrieve number of rows */
            const int cols = Blocks->insz[Blocks->nin + k];     /* retrieve number of cols */
            const int type = Blocks->insz[2 * Blocks->nin + k]; /* retrieve type */
            inptr->set(k, vartosci(inptr->get(k), Blocks->inptr[k], rows, cols, type));
        }
        m->set(17, inptr);
    }

    /* 21 - outptr */
    if (m->get(20)->isList())
    {
        types::List* outptr = m->get(20)->getAs<types::List>();
        for (int k = 0; k < Blocks->nout; k++)
        {
            const int rows = Blocks->outsz[k];                    /* retrieve number of rows */
            const int cols = Blocks->outsz[Blocks->nout + k];     /* retrieve number of cols */
            const int type = Blocks->outsz[2 * Blocks->nout + k]; /* retrieve type */
            outptr->set(k, vartosci(outptr->get(k), Blocks->outptr[k], rows, cols, type));
        }
        m->set(20, outptr);
    }

    /* 23 - evout */
    m->set(22, vartosci(m->get(22), Blocks->evout, Blocks->nevout, 1, SCSREAL_N));

    /* 33 - g */
    if (flag_imp >= 0)
    {
        m->set(32, vartosci(m->get(32), &g[zcptr[flag_imp] - 1], Blocks->ng, 1, SCSREAL_N));
    }
    else
    {
        m->set(32, vartosci(m->get(32), Blocks->g, Blocks->ng, 1, SCSREAL_N));
    }

    /* 35 - jroot */
    m->set(34, vartosci(m->get(34), Blocks->jroot, Blocks->ng, 1, SCSREAL_N));

    /* 39 - mode */
    m->set(38, vartosci(m->get(38), Blocks->mode, Blocks->nmode, 1, SCSREAL_N));

    /* 40 - xprop */
    m->set(39, vartosci(m->get(39), Blocks->xprop, Blocks->nx, 1, SCSREAL_N));

    return m;
}
/*--------------------------------------------------------------------------*/
