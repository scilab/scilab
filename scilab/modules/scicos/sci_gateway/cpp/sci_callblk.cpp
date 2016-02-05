/*  Scicos
*
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*  Copyright (C) INRIA -
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
#include <string>
#include <cwchar>
#include <cstring>

#include "gw_scicos.hxx"

#include "internal.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "int.hxx"
#include "mlist.hxx"
#include "context.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "scicos.h"
#include "scicos_block4.h"
#include "Scierror.h"
#include "localization.h"
}

#include "extractblklist.hxx"
#include "createblklist.hxx"

/*--------------------------------------------------------------------------*/
static const std::string name = "callblk";
/*--------------------------------------------------------------------------*/
static void clearSubBlock(void* sub)
{
    if (sub)
    {
        FREE(sub);
    }
}

static void clearBlock(scicos_block* Block)
{
    clearSubBlock(Block->z);
    clearSubBlock(Block->ozsz);
    clearSubBlock(Block->oztyp);
    for (int j = 0; j < Block->noz; j++)
    {
        clearSubBlock(Block->ozptr[j]);
    }
    clearSubBlock(Block->ozptr);
    clearSubBlock(Block->x);
    clearSubBlock(Block->xd);
    clearSubBlock(Block->xprop);
    clearSubBlock(Block->res);
    clearSubBlock(Block->insz);
    for (int j = 0; j < Block->nin; j++)
    {
        clearSubBlock(Block->inptr[j]);
    }
    clearSubBlock(Block->inptr);
    clearSubBlock(Block->outsz);
    for (int j = 0; j < Block->nout; j++)
    {
        clearSubBlock(Block->outptr[j]);
    }
    clearSubBlock(Block->outptr);
    clearSubBlock(Block->evout);
    clearSubBlock(Block->rpar);
    clearSubBlock(Block->ipar);
    clearSubBlock(Block->oparsz);
    clearSubBlock(Block->opartyp);
    for (int j = 0; j < Block->nopar; j++)
    {
        clearSubBlock(Block->oparptr[j]);
    }
    clearSubBlock(Block->oparptr);
    clearSubBlock(Block->g);
    clearSubBlock(Block->jroot);
    if (Block->label)
    {
        clearSubBlock(Block->label);
    }
    clearSubBlock(Block->mode);
    if (Block->uid)
    {
        clearSubBlock(Block->uid);
    }
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_callblk(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::InternalType* pIT = nullptr;
    types::TList* tl = nullptr;
    types::String* s = nullptr;
    types::Double* d = nullptr;

    /* length of the scilab list scicos struct */
    const int nblklst = 41;

    scicos_block Block;
    memset(&Block, 0, sizeof(scicos_block));

    if (in.size() != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument: %d expected.\n"), name.data(), 3);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), name.data(), 1);
        return types::Function::Error;
    }

    pIT = in[0];

    if (pIT->isTList() == false)
    {
        Scierror(888, _("%s : First argument must be a scicos_block typed list.\n"), name.data());
        return types::Function::Error;
    }

    tl = pIT->getAs<types::TList>();
    pIT = tl->get(0);

    if (pIT->isString() == false)
    {
        Scierror(888, _("%s : First argument must be a valid scicos_block typed list.\n"), name.data());
        return types::Function::Error;
    }

    s = pIT->getAs<types::String>();
    if (s->getSize() != nblklst)
    {
        Scierror(888, _("%s : First argument must be a valid scicos_block typed list.\n"), name.data());
        return types::Function::Error;
    }

    if (wcscmp(s->get()[0], L"scicos_block"))
    {
        Scierror(888, _("%s : First argument must be a valid scicos_block typed list.\n"), name.data());
        return types::Function::Error;
    }

    /* convert scilab scicos struct to a C scicos struct */
    if (extractblklist(tl, &Block) == false)
    {
        clearBlock(&Block);
        return types::Function::Error;
    }

    d = in[1]->getAs< types::Double>();
    if (d->isDouble() == false || d->isScalar() == false)
    {
        Scierror(888, _("%s : Second argument must be scalar.\n"), name.data());
        return types::Function::Error;
    }

    scicos_flag flag = static_cast<scicos_flag>(static_cast<int>(d->get()[0]));

    d = in[2]->getAs< types::Double>();
    if (d->isDouble() == false || d->isScalar() == false)
    {
        Scierror(888, _("%s : Third argument must be scalar.\n"), name.data());
        return types::Function::Error;
    }

    double t = d->get()[0];

    /* call block */
    callf(&t, &Block, &flag);


    out.push_back(createblklist(&Block, -1, Block.type));
    clearBlock(&Block);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
