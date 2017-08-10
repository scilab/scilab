/*  Scicos
*
*  Copyright (C) INRIA - Alan LAYEC
*  Copyright (C) 2013 - Scilab Enterprises - Clement DAVID
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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
#include <cstring>

#include "internal.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "double.hxx"
#include "string.hxx"
#include "int.hxx"

extern "C"
{
#include "scicos_block4.h"
#include "charEncoding.h"
}

#include "extractblklist.hxx"

/*--------------------------------------------------------------------------*/

template <typename T>
bool sci2var(T* p, void** dest)
{
    const int size = p->getSize();
    typename T::type* srcR = p->get();

    if (p->isComplex())
    {
        typename T::type* srcI = p->getImg();
        *dest = (typename T::type*)MALLOC(sizeof(typename T::type) * size * 2);
        if (*dest == nullptr)
        {
            return false;
        }

        typename T::type* destR = (typename T::type*)*dest;
        typename T::type* destI = destR + size;
        for (int i = 0; i < size; ++i)
        {
            destR[i] = srcR[i];
            destI[i] = srcI[i];
        }
    }
    else
    {
        *dest = (typename T::type*)MALLOC(sizeof(typename T::type) * size);
        if (*dest == nullptr)
        {
            return false;
        }

        typename T::type* destR = (typename T::type*)*dest;
        for (int i = 0; i < size; ++i)
        {
            destR[i] = srcR[i];
        }
    }

    return true;
}

static bool sci2var(types::InternalType* p, void** dest)
{
    *dest = nullptr;
    switch (p->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            return sci2var(p->getAs<types::Double>(), dest);
        }
        case types::InternalType::ScilabInt8:
        {
            return sci2var(p->getAs<types::Int8>(), dest);
        }
        case types::InternalType::ScilabInt16:
        {
            return sci2var(p->getAs<types::Int16>(), dest);
        }
        case types::InternalType::ScilabInt32:
        {
            return sci2var(p->getAs<types::Int32>(), dest);
        }
        case types::InternalType::ScilabInt64:
        {
            return sci2var(p->getAs<types::Int64>(), dest);
        }
        case types::InternalType::ScilabUInt8:
        {
            return sci2var(p->getAs<types::UInt8>(), dest);
        }
        case types::InternalType::ScilabUInt16:
        {
            return sci2var(p->getAs<types::UInt16>(), dest);
        }
        case types::InternalType::ScilabUInt32:
        {
            return sci2var(p->getAs<types::UInt32>(), dest);
        }
        case types::InternalType::ScilabUInt64:
        {
            return sci2var(p->getAs<types::UInt64>(), dest);
        }
        default:
            return false;
    }

    return false;
}

static bool getString(types::InternalType* p, char** dest)
{
    *dest = nullptr;

    if (p == nullptr)
    {
        return false;
    }

    if (p->isString())
    {
        types::String* s = p->getAs<types::String>();
        if (s->isScalar())
        {
            *dest = wide_string_to_UTF8(s->get()[0]);
            return true;
        }
    }
    return false;
}

static bool getDoubleArray(types::InternalType* p, double** dest, const int size)
{
    *dest = nullptr;

    if (p == nullptr)
    {
        return false;
    }

    if (p->isDouble())
    {
        types::Double* d = p->getAs<types::Double>();
        if (d->getSize() == size)
        {
            if (size == 0)
            {
                return true;
            }

            *dest = (double*)MALLOC(sizeof(double) * size);
            if (*dest == nullptr)
            {
                return false;
            }
            memcpy(*dest, d->get(), sizeof(double) * size);
            return true;
        }
    }

    return false;
}

static bool getDoubleArrayAsInt(types::InternalType* p, int** dest, const int size)
{
    *dest = nullptr;

    if (p == nullptr)
    {
        return false;
    }

    if (p->isDouble())
    {
        types::Double* d = p->getAs<types::Double>();
        if (d->getSize() == size)
        {
            if (size == 0)
            {
                return true;
            }

            const double* const dbl = d->get();
            *dest = (int*)MALLOC(sizeof(int) * size);
            if (*dest == nullptr)
            {
                return false;
            }

            for (int i = 0; i < size; ++i)
            {
                (*dest)[i] = static_cast<int>(dbl[i]);
            }
            return true;
        }
    }

    return false;
}

static bool getDoubleAsInt(types::InternalType* p, int* dest)
{
    if (p == nullptr)
    {
        return false;
    }

    if (p->isDouble())
    {
        types::Double* d = p->getAs<types::Double>();
        if (d->isScalar())
        {
            *dest = static_cast<int>(d->get()[0]);
            return true;
        }
    }
    return false;
}

static bool checkType(const int type, types::InternalType* p)
{
    if (p == nullptr)
    {
        return false;
    }

    switch (type)
    {
        case 10:
            if (p->isDouble())
            {
                return true;
            }
        case 11:
            if (p->isDouble() && p->getAs<types::Double>()->isComplex())
            {
                return true;
            }
        case 81:
            if (p->isInt8())
            {
                return true;
            }
        case 82:
            if (p->isInt16())
            {
                return true;
            }
        case 84:
            if (p->isInt32())
            {
                return true;
            }
        case 811:
            if (p->isUInt8())
            {
                return true;
            }
        case 812:
            if (p->isUInt16())
            {
                return true;
            }
        case 814:
            if (p->isUInt32())
            {
                return true;
            }
    }

    return false;
}

bool extractblklist(types::TList* t, scicos_block* const Block)
{
    types::InternalType* pIT = nullptr;

    /* 2 - nevprt */
    if (getDoubleAsInt(t->getField(L"nevprt"), &Block->nevprt) == false)
    {
        return false;
    }

    /* 3 - funpt */
    //function ptr hide in double*
    pIT = t->getField(L"funpt");
    if (pIT->isDouble())
    {
        types::Double* d = pIT->getAs<types::Double>();
        Block->funpt = (voidg)(long long)d->get()[0];
    }

    /* 4 - type */
    if (getDoubleAsInt(t->getField(L"type"), &Block->type) == false)
    {
        return false;
    }

    /* 5 - scsptr */
    //function ptr hide in double*
    pIT = t->getField(L"scsptr");
    if (pIT->isDouble())
    {
        types::Double* d = pIT->getAs<types::Double>();
        Block->scsptr = (void*)(long long)d->get()[0];
    }

    /* 6 - nz */
    if (getDoubleAsInt(t->getField(L"nz"), &Block->nz) == false)
    {
        return false;
    }

    /* 7 - z */
    if (getDoubleArray(t->getField(L"z"), &Block->z, Block->nz) == false)
    {
        return false;
    }

    /* 8 - noz */
    if (getDoubleAsInt(t->getField(L"noz"), &Block->noz) == false)
    {
        return false;
    }

    /* 9 - ozsz */
    if (getDoubleArrayAsInt(t->getField(L"ozsz"), &Block->ozsz, Block->noz * 2) == false)
    {
        return false;
    }

    /* 10 - oztyp */
    if (getDoubleArrayAsInt(t->getField(L"oztyp"), &Block->ozsz, Block->noz) == false)
    {
        return false;
    }

    /* 11 - oz */
    pIT = t->getField(L"oz");
    if (pIT->isList())
    {
        types::List* ozptr = pIT->getAs<types::List>();
        if (ozptr->getSize() != Block->noz)
        {
            return false;
        }

        Block->ozptr = (void**)MALLOC(sizeof(void*) * Block->noz);
        if (Block->ozptr == nullptr)
        {
            return false;
        }

        for (int i = 0; i < Block->noz; ++i)
        {
            pIT = ozptr->get(i);
            if (checkType(Block->oztyp[i], pIT) == false)
            {
                return false;
            }

            if (sci2var(pIT, &Block->ozptr[i]) == false)
            {
                return false;
            }
        }
    }

    /* 12 - nx */
    if (getDoubleAsInt(t->getField(L"nx"), &Block->nx) == false)
    {
        return false;
    }

    /* 13 - x */
    if (getDoubleArray(t->getField(L"x"), &Block->x, Block->nx) == false)
    {
        return false;
    }

    /* 14 - xd */
    if (getDoubleArray(t->getField(L"xd"), &Block->xd, Block->nx) == false)
    {
        return false;
    }

    /* 15 - res */
    if (getDoubleArray(t->getField(L"res"), &Block->res, Block->nx) == false)
    {
        return false;
    }

    /* 16 - nin */
    if (getDoubleAsInt(t->getField(L"nin"), &Block->nin) == false)
    {
        return false;
    }

    /* 17 - insz */
    if (getDoubleArrayAsInt(t->getField(L"insz"), &Block->insz, Block->nin * 3) == false)
    {
        return false;
    }

    /* 18 - inptr */
    pIT = t->getField(L"inptr");
    if (pIT->isList())
    {
        types::List* inptr = pIT->getAs<types::List>();
        if (inptr->getSize() != Block->nin)
        {
            return false;
        }

        Block->inptr = (void**)MALLOC(sizeof(void*) * Block->nin);
        if (Block->inptr == nullptr)
        {
            return false;
        }

        for (int i = 0; i < Block->nin; ++i)
        {
            pIT = inptr->get(i);
            if (checkType(Block->insz[2 * Block->nin + i], pIT) == false)
            {
                return false;
            }

            if (sci2var(pIT, &Block->inptr[i]) == false)
            {
                return false;
            }
        }
    }

    /* 19 - nout */
    if (getDoubleAsInt(t->getField(L"nout"), &Block->nout) == false)
    {
        return false;
    }

    /* 20 - outsz */
    if (getDoubleArrayAsInt(t->getField(L"outsz"), &Block->outsz, Block->nout * 3) == false)
    {
        return false;
    }

    /* 21 - outptr */
    pIT = t->getField(L"outptr");
    if (pIT->isList())
    {
        types::List* outptr = pIT->getAs<types::List>();
        if (outptr->getSize() != Block->nout)
        {
            return false;
        }

        Block->outptr = (void**)MALLOC(sizeof(void*) * Block->nout);
        if (Block->outptr == nullptr)
        {
            return false;
        }

        for (int i = 0; i < Block->nout; ++i)
        {
            pIT = outptr->get(i);
            if (checkType(Block->outsz[2 * Block->nout + i], pIT) == false)
            {
                return false;
            }

            if (sci2var(pIT, &Block->outptr[i]) == false)
            {
                return false;
            }
        }
    }

    /* 22 - nevout */
    if (getDoubleAsInt(t->getField(L"nevout"), &Block->nevout) == false)
    {
        return false;
    }

    /* 23 - evout */
    if (getDoubleArray(t->getField(L"evout"), &Block->evout, Block->nevout) == false)
    {
        return false;
    }

    /* 24 - nrpar */
    if (getDoubleAsInt(t->getField(L"nrpar"), &Block->nrpar) == false)
    {
        return false;
    }

    /* 25 - rpar */
    if (getDoubleArray(t->getField(L"rpar"), &Block->rpar, Block->nrpar) == false)
    {
        return false;
    }

    /* 26 - nipar */
    if (getDoubleAsInt(t->getField(L"nipar"), &Block->nipar) == false)
    {
        return false;
    }

    /* 27 - ipar */
    if (getDoubleArrayAsInt(t->getField(L"ipar"), &Block->ipar, Block->nipar) == false)
    {
        return false;
    }

    /* 28 - nopar */
    if (getDoubleAsInt(t->getField(L"nopar"), &Block->nopar) == false)
    {
        return false;
    }

    /* 29 - oparsz */
    if (getDoubleArrayAsInt(t->getField(L"oparsz"), &Block->oparsz, 2 * Block->nopar) == false)
    {
        return false;
    }

    /* 30 - opartyp */
    if (getDoubleArrayAsInt(t->getField(L"opartyp"), &Block->opartyp, Block->nopar) == false)
    {
        return false;
    }

    /* 31 - opar */
    pIT = t->getField(L"opar");
    if (pIT->isList())
    {
        types::List* opar = pIT->getAs<types::List>();
        if (opar->getSize() != Block->nopar)
        {
            return false;
        }

        Block->oparptr = (void**)MALLOC(sizeof(void*) * Block->nopar);
        if (Block->inptr == nullptr)
        {
            return false;
        }

        for (int i = 0; i < Block->nopar; ++i)
        {
            pIT = opar->get(i);
            if (checkType(Block->opartyp[i], pIT) == false)
            {
                return false;
            }

            if (sci2var(pIT, &Block->oparptr[i]) == false)
            {
                return false;
            }
        }
    }

    /* 32 - ng */
    if (getDoubleAsInt(t->getField(L"ng"), &Block->ng) == false)
    {
        return false;
    }

    /* 33 - g */
    if (getDoubleArray(t->getField(L"g"), &Block->g, Block->ng) == false)
    {
        return false;
    }

    /* 34 - ztyp */
    if (getDoubleAsInt(t->getField(L"ztyp"), &Block->ztyp) == false)
    {
        return false;
    }

    /* 35 - jroot */
    if (getDoubleArrayAsInt(t->getField(L"jroot"), &Block->jroot, Block->ng) == false)
    {
        return false;
    }

    /* 36 - label */
    if (getString(t->getField(L"label"), &Block->label) == false)
    {
        return false;
    }

    /* 37 - work*/
    pIT = t->getField(L"work");
    if (pIT->isDouble())
    {
        types::Double* d = pIT->getAs<types::Double>();
        Block->work = (void**)(long long)d->get()[0];
    }

    /* 38 - nmode*/
    if (getDoubleAsInt(t->getField(L"nmode"), &Block->nmode) == false)
    {
        return false;
    }

    /* 39 - mode */
    if (getDoubleArrayAsInt(t->getField(L"mode"), &Block->mode, Block->nmode) == false)
    {
        return false;
    }

    /* 40 - xprop */
    if (getDoubleArrayAsInt(t->getField(L"xprop"), &Block->xprop, Block->nx) == false)
    {
        return false;
    }

    return true;
}
/*--------------------------------------------------------------------------*/
