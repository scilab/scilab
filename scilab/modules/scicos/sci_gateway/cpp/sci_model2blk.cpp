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
#include "list.hxx"
#include "mlist.hxx"
#include "context.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "machine.h" /* F2C */
#include "sci_malloc.h"
#include "sciblk4.h"
#include "sciblk2.h"
#include "MlistGetFieldNumber.h"
#include "scicos.h"
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"

    extern void F2C(sciblk)();
}

#include "createblklist.hxx"

/*--------------------------------------------------------------------------*/
//extern int *listentry(int *header, int i);
//extern int C2F(funnum) (char *fname);
//extern int C2F(namstr)();
/*--------------------------------------------------------------------------*/
//extern int ntabsim;
//extern OpTab tabsim[];
/*--------------------------------------------------------------------------*/
/* model2blk Build a scicos_block structure from
* a scicos model
*
* [Block] = model2blk(objs.model)
*
* rhs 1 : a scicos model Tlist
*        - 1  : model(1)     : !model      sim     in      in2     intyp    out    out2  outtyp
*                               evtin      evtout  state   dstate  odstate  rpar   ipar  opar
*                               blocktype  firing  dep_ut  label   nzcross  nmode  equations  !
*        - 2  : model.sim       :
*        - 3  : model.in        :
*        - 4  : model.in2       :
*        - 5  : model.intyp     :
*        - 6  : model.out       :
*        - 7  : model.out2      :
*        - 8  : model.outtyp    :
*        - 9  : model.evtin     :
*        - 10 : model.evtout    :
*        - 11 : model.state     :
*        - 12 : model.dsate     :
*        - 13 : model.odsate    :
*        - 14 : model.rpar      :
*        - 15 : model.ipar      :
*        - 16 : model.opar      :
*        - 17 : model.blocktype :
*        - 18 : model.firing    :
*        - 19 : model.dep_ut    :
*        - 20 : model.label     :
*        - 21 : model.nzcross   :
*        - 22 : model.nmode      :
*        - 23 : model.equations :
*
* lhs 1 : a scicos block Tlist
*
* initial rev 12/11/07, Alan
* 05/07/08, Alan : fix for xprop
*                  add extra comments
*                  check in/out size and type
*
*/
static const std::string name = "model2blk";

static void freeSubBlock(void* _ptr)
{
    if (_ptr)
    {
        FREE(_ptr);
    }
}

static void freeBlock(scicos_block* block)
{
    for (int i = 0; i < block->nin; ++i)
    {
        freeSubBlock(block->inptr[i]);
    }

    freeSubBlock(block->inptr);
    freeSubBlock(block->insz);

    for (int i = 0; i < block->nout; ++i)
    {
        freeSubBlock(block->outptr[i]);
    }
    freeSubBlock(block->outptr);
    freeSubBlock(block->outsz);

    freeSubBlock(block->evout);
    freeSubBlock(block->x);
    freeSubBlock(block->xd);
    freeSubBlock(block->xprop);
    freeSubBlock(block->res);
    freeSubBlock(block->z);
    freeSubBlock(block->ozsz);
    freeSubBlock(block->oztyp);

    for (int i = 0; i < block->noz; ++i)
    {
        freeSubBlock(block->ozptr[i]);
    }
    freeSubBlock(block->ozptr);

    freeSubBlock(block->rpar);
    freeSubBlock(block->ipar);
    freeSubBlock(block->oparsz);
    freeSubBlock(block->opartyp);

    for (int i = 0; i < block->nopar; ++i)
    {
        freeSubBlock(block->oparptr[i]);
    }
    freeSubBlock(block->oparptr);
    freeSubBlock(block->g);
    freeSubBlock(block->label);
    freeSubBlock(block->jroot);
    freeSubBlock(block->mode);
    freeSubBlock(block->uid);
    freeSubBlock(block->work);
}

template<typename T>
bool alloc_and_set(T* t, void** v)
{
    const int size = t->getSize();
    typename T::type* src = t->get();
    *v = MALLOC(sizeof(typename T::type) * size);
    if (*v == nullptr)
    {
        return false;
    }

    for (int i = 0; i < size; ++i)
    {
        ((typename T::type*)(*v))[i] = src[i];
    }

    return true;
}

types::Function::ReturnValue sci_model2blk(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::InternalType* pIT = nullptr;
    types::InternalType* il_sim = nullptr;

    /* misc */
    types::Double* d = nullptr;
    types::List * l = nullptr;
    types::String * s = nullptr;

    scicos_block Block;
    memset(&Block, 0, sizeof(scicos_block));

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument: %d expected.\n"), name.data(), 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), name.data(), 1);
        return types::Function::Error;
    }

    pIT = in[0];
    /* check for a tlist */
    if (pIT->isMList() == false)
    {
        Scierror(888, _("%s : First argument must be a Typed list.\n"), name.data());
        return types::Function::Error;
    }

    types::MList* m = pIT->getAs<types::MList>();

    /* check for a type "scicos model" */
    if (wcscmp(m->getTypeStr().data(), L"model"))
    {
        Scierror(888, _("%s : First argument must be a Typed list.\n"), name.data());
        return types::Function::Error;
    }

    char* c = wide_string_to_UTF8(m->getTypeStr().data());
    std::string blockname(c);
    FREE(c);

    /* 2 : model.sim  */
    pIT = m->getField(L"sim");
    if (pIT->isList())
    {
        l = pIT->getAs<types::List>();
        il_sim = l->get(0)->getAs<types::Double>();
        d = l->get(1)->getAs<types::Double>();
        Block.type = static_cast<int>(d->get()[0]);
    }
    else
    {
        Block.type = 0;
        il_sim = pIT;
    }

    bool isMacro = false;
    /* check if typfsim is a scilab function */
    if (il_sim->isMacro() || il_sim->isMacroFile())
    {
        isMacro = true;
    }
    else if (il_sim->isString())
    {
        types::String* funStr = il_sim->getAs<types::String>();
        wchar_t* w_str = funStr->get(0);
        char* c_str = wide_string_to_UTF8(w_str);
        void* f = funnum2(c_str); // Search associated function number of function name
        // Block is defined by a C or Fortran function
        if (f != nullptr)
        {
            // C interface from "tabsim" defined in blocks.h
            Block.funpt = (voidg)f;
        }
        // Block is defined by a predefined scilab function
        else
        {
            ConfigVariable::EntryPointStr* pEP = ConfigVariable::getEntryPoint(w_str);
            if (pEP)
            {
                //linked functions
                Block.funpt = static_cast<voidg>(pEP->functionPtr);
            }
            else
            {
                types::InternalType* pMacro = symbol::Context::getInstance()->get(symbol::Symbol(w_str));
                if (pMacro && pMacro->isCallable())
                {
                    //macros
                    il_sim = pMacro;
                    isMacro = true;
                }
                else
                {
                    Scierror(888, _("%s : unknown block : %s\n"), name.data(), blockname.data());
                    return types::Function::Error;
                }
            }
        }
    }

    if (isMacro)
    {
        switch (Block.type)
        {
            case 0:
                Block.funpt = F2C(sciblk);
                break;
            case 1:
                Scierror(888, _("%s : type 1 function not allowed for scilab blocks\n"), name.data());
                return types::Function::Error;
            case 2:
                Scierror(888, _("%s : type 2 function not allowed for scilab blocks\n"), name.data());
                return types::Function::Error;
            case 3:
                Block.funpt = (voidg)sciblk2;
                Block.type = 2;
                break;
            case 5:
                Block.funpt = (voidg)sciblk4;
                Block.type = 4;
                break;
            case 99: /* debugging block */
                Block.funpt = (voidg)sciblk4;
                Block.type = 4;
                break;
            case 10005:
                Block.funpt = (voidg)sciblk4;
                Block.type = 10004;
                break;
            default:
                Scierror(888, _("%s : Undefined Function type\n"), name.data());
                return types::Function::Error;
        }
        Block.scsptr = static_cast<void*>(il_sim);
    }

    /* check input ports */
    /* 3 : model.in  */
    pIT = m->getField(L"in");
    d = pIT->getAs<types::Double>();
    const int sizeIn = d->getSize();
    Block.nin = d->getSize();
    Block.insz = nullptr;
    Block.inptr = nullptr;
    if (Block.nin > 0)
    {
        const double* const vIn = d->get();
        for (int i = 0; i < sizeIn; ++i)
        {
            /* check value of in */
            if (vIn[i] <= 0)
            {
                Scierror(888, _("%s : Undetermined Size. in(%d)=%d. Please adjust your model.\n"), \
                         name.data(), i + 1, static_cast<int>(vIn[i]));
                return types::Function::Error;
            }
        }

        /* alloc insz */
        if ((Block.insz = (int *)MALLOC(Block.nin * 3 * sizeof(int))) == nullptr)
        {
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }
        /* alloc inptr */
        if ((Block.inptr = (void **)MALLOC(Block.nin * sizeof(void *))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        /* 4 : model.in2  */
        pIT = m->getField(L"in2");
        d = pIT->getAs<types::Double>();
        const int sizeIn2 = d->getSize();
        const double* const vIn2 = d->get();
        for (int i = 0; i < sizeIn2; ++i)
        {
            /* check value of in2 */
            if (vIn2[i] <= 0)
            {
                Scierror(888, _("%s : Undetermined Size. in2(%d)=%d. Please adjust your model.\n"), \
                         name.data(), i + 1, static_cast<int>(vIn2[i]));
                freeBlock(&Block);
                return types::Function::Error;
            }
        }

        /* 5 : model.intyp  */
        pIT = m->getField(L"intyp");
        d = pIT->getAs<types::Double>();
        const int sizeIntyp = d->getSize();
        const double* const vIntype = d->get();
        for (int i = 0; i < sizeIntyp; ++i)
        {
            /* check value of intyp */
            if (vIntype[i] <= 0)
            {
                Scierror(888, _("%s : Undetermined Size. intyp(%d)=%d. Please adjust your model.\n"), \
                         name.data(), i + 1, static_cast<int>(vIntype[i]));
                freeBlock(&Block);
                return types::Function::Error;
            }
        }

        if (sizeIn == sizeIn2 && sizeIn == sizeIntyp)
        {
            for (int i = 0; i < sizeIn; ++i)
            {
                Block.insz[i] = static_cast<int>(vIn[i]);
                Block.insz[i + sizeIn] = static_cast<int>(vIn2[i]);
                int val = 0;
                switch (static_cast<int>(vIntype[i]))
                {
                    case 1:
                    default:
                        val = 10;
                        break;
                    case 2:
                        val = 11;
                        break;
                    case 3:
                        val = 84;
                        break;
                    case 4:
                        val = 82;
                        break;
                    case 5:
                        val = 81;
                        break;
                    case 6:
                        val = 814;
                        break;
                    case 7:
                        val = 812;
                        break;
                    case 8:
                        val = 811;
                        break;
                }

                Block.insz[i + sizeIn * 2] = val;
            }
        }
        else
        {
            for (int i = 0; i < Block.nin; i++)
            {
                Block.insz[i] = static_cast<int>(vIn[i]);
                Block.insz[i + sizeIn] = 1;
                Block.insz[i + sizeIn * 2] = 10;
            }
        }

        for (int i = 0; i < sizeIn; i++)
        {
            int size = Block.insz[i] * Block.insz[Block.nin + i];
            switch (Block.insz[2 * sizeIn + i])
            {
                case 10: //double
                    size *= sizeof(double);
                    break;
                case 11: //complex
                    size *= 2 * sizeof(double);
                    break;
                case 84: //int32
                    size *= sizeof(SCSINT32_COP);
                    break;
                case 82: //int16
                    size *= sizeof(short);
                    break;
                case 81: //int8
                    size *= sizeof(char);
                    break;
                case 814: //uint32
                    size *= sizeof(SCSUINT32_COP);
                    break;
                case 812: //uint16
                    size *= sizeof(unsigned short);
                    break;
                case 811: //uint8
                    size *= sizeof(unsigned char);
                    break;
                default:
                    Scierror(888, _("%s : Unknown Data type\n"), name.data());
                    return types::Function::Error;
            }

            Block.inptr[i] = MALLOC(size);
            if (Block.inptr == nullptr)
            {
                freeBlock(&Block);
                Scierror(888, _("%s : Allocation error.\n"), name.data());
                return types::Function::Error;
            }

            memset(Block.inptr[i], 0x00, size);
        }
    }

    /* check output ports */
    /* 6 : model.out  */
    pIT = m->getField(L"out");
    d = pIT->getAs<types::Double>();
    const int sizeOut = d->getSize();
    Block.nout = d->getSize();
    Block.outsz  = nullptr;
    Block.outptr = nullptr;
    if (sizeOut > 0)
    {
        const double* const vOut = d->get();
        for (int i = 0; i < sizeOut; ++i)
        {
            /* check value of out */
            if (vOut[i] <= 0)
            {
                Scierror(888, _("%s : Undetermined Size. out(%d)=%d. Please adjust your model.\n"), \
                         name.data(), i + 1, static_cast<int>(vOut[i]));
                freeBlock(&Block);
                return types::Function::Error;
            }
        }

        /* alloc outsz */
        if ((Block.outsz = (int *)MALLOC(sizeOut * 3 * sizeof(int))) == nullptr)
        {
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            freeBlock(&Block);
            return types::Function::Error;
        }

        /* alloc outptr */
        if ((Block.outptr = (void **)MALLOC(sizeOut * sizeof(void *))) == nullptr)
        {
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            freeBlock(&Block);
            return types::Function::Error;
        }

        /* 7 : model.out2  */
        pIT = m->getField(L"out2");
        d = pIT->getAs<types::Double>();
        const int sizeOut2 = d->getSize();
        const double* const vOut2 = d->get();
        for (int i = 0; i < sizeOut2; ++i)
        {
            /* check value of in2 */
            if (vOut2[i] <= 0)
            {
                Scierror(888, _("%s : Undetermined Size. out2(%d)=%d. Please adjust your model.\n"), \
                         name.data(), i + 1, static_cast<int>(vOut2[i]));
                freeBlock(&Block);
                return types::Function::Error;
            }
        }

        /* 5 : model.intyp  */
        pIT = m->getField(L"intyp");
        d = pIT->getAs<types::Double>();
        const int sizeOuttyp = d->getSize();
        const double* const vOuttype = d->get();
        for (int i = 0; i < sizeOuttyp; ++i)
        {
            /* check value of intyp */
            if (vOuttype[i] <= 0)
            {
                Scierror(888, _("%s : Undetermined Size. outtyp(%d)=%d. Please adjust your model.\n"), \
                         name.data(), i + 1, static_cast<int>(vOuttype[i]));
                freeBlock(&Block);
                return types::Function::Error;
            }
        }

        if (sizeOut == sizeOut2 && sizeOut == sizeOuttyp)
        {
            for (int i = 0; i < sizeOut; ++i)
            {
                Block.outsz[i] = static_cast<int>(vOut[i]);
                Block.outsz[i + sizeOut] = static_cast<int>(vOut2[i]);
                int val = 0;
                switch (static_cast<int>(vOuttype[i]))
                {
                    case 1:
                    default:
                        val = 10;
                        break;
                    case 2:
                        val = 11;
                        break;
                    case 3:
                        val = 84;
                        break;
                    case 4:
                        val = 82;
                        break;
                    case 5:
                        val = 81;
                        break;
                    case 6:
                        val = 814;
                        break;
                    case 7:
                        val = 812;
                        break;
                    case 8:
                        val = 811;
                        break;
                }

                Block.outsz[i + sizeOut * 2] = val;
            }
        }
        else
        {
            for (int i = 0; i < sizeOut; i++)
            {
                Block.outsz[i] = static_cast<int>(vOut[i]);
                Block.outsz[i + sizeOut] = 1;
                Block.outsz[i + sizeOut * 2] = 10;
            }
        }

        for (int i = 0; i < sizeOut; i++)
        {
            int size = Block.outsz[i] * Block.outsz[sizeOut + i];
            switch (Block.outsz[2 * sizeOut + i])
            {
                case 10: //double
                    size *= sizeof(double);
                    break;
                case 11: //complex
                    size *= 2 * sizeof(double);
                    break;
                case 84: //int32
                    size *= sizeof(SCSINT32_COP);
                    break;
                case 82: //int16
                    size *= sizeof(short);
                    break;
                case 81: //int8
                    size *= sizeof(char);
                    break;
                case 814: //uint32
                    size *= sizeof(SCSUINT32_COP);
                    break;
                case 812: //uint16
                    size *= sizeof(unsigned short);
                    break;
                case 811: //uint8
                    size *= sizeof(unsigned char);
                    break;
                default:
                    Scierror(888, _("%s : Unknown Data type\n"), name.data());
                    return types::Function::Error;
            }

            Block.outptr[i] = MALLOC(size);
            if (Block.outptr[i] == nullptr)
            {
                freeBlock(&Block);
                Scierror(888, _("%s : Allocation error.\n"), name.data());
                return types::Function::Error;
            }

            memset(Block.outptr[i], 0x00, size);
        }
    }

    /* event input port */
    /* 9 : model.evtin  */

    /* event output port  */
    /* 10 : model.evtout  */
    pIT = m->getField(L"evtout");
    d = pIT->getAs<types::Double>();
    Block.nevout = d->getSize();
    if (Block.nevout > 0)
    {
        if ((Block.evout = (double *)MALLOC(Block.nevout * sizeof(double))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        pIT = m->getField(L"firing");
        types::Double* firing = pIT->getAs<types::Double>();
        if (Block.nevout == firing->getSize())
        {
            const double* const f = firing->get();
            for (int j = 0; j < Block.nevout; j++)
            {
                Block.evout[j] = f[j];
            }
        }
        else
        {
            for (int j = 0; j < Block.nevout; j++)
            {
                Block.evout[j] = -1;
            }
        }
    }

    /* continuous state  */
    /* 11 : model.state  */
    m->getField(L"state");
    d = pIT->getAs<types::Double>();
    Block.nx        = d->getSize();
    Block.x         = nullptr;
    Block.xprop     = nullptr;
    Block.xd        = nullptr;
    Block.res       = nullptr;
    if (Block.nx > 0)
    {
        /* x */
        if ((Block.x = (double *) MALLOC(Block.nx * sizeof(double))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        const double* const x = d->get();
        for (int j = 0; j < Block.nx; j++)
        {
            Block.x[j] = x[j];
        }

        /* xd */
        if ((Block.xd = (double *) MALLOC(Block.nx * sizeof(double))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        for (int j = 0; j < Block.nx; j++)
        {
            Block.xd[j] = 0;
        }

        /* xprop */
        if ((Block.xprop = (int *) MALLOC(Block.nx * sizeof(int))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        for (int j = 0; j < Block.nx; j++)
        {
            Block.xprop[j] = 1;
        }

        /* res */
        /*if (blktyp>10000) {*/
        if ((Block.res = (double *)MALLOC(Block.nx * sizeof(double))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        for (int j = 0; j < Block.nx; j++)
        {
            Block.res[j] = 0;
        }
        /*}*/
    }

    /* discrete state  */
    /* 12 : model.dstate  */
    m->getField(L"dstate");
    d = pIT->getAs<types::Double>();
    Block.nz = d->getSize();
    if (Block.nz > 0)
    {
        if ((Block.z = (double *)MALLOC(Block.nz * sizeof(double))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        const double* const z = d->get();
        for (int j = 0; j < Block.nz; j++)
        {
            Block.z[j] = z[j];
        }
    }

    /* discrete object state  */
    /* 13 : model.odstate  */
    pIT = m->getField(L"odstate");
    l = pIT->getAs<types::List>();
    Block.noz    = l->getSize();
    Block.ozsz   = nullptr;
    Block.oztyp  = nullptr;
    Block.ozptr  = nullptr;
    if (Block.noz > 0)
    {
        if ((Block.ozsz = (int *)MALLOC(2 * Block.noz * sizeof(int))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        if ((Block.oztyp = (int *)MALLOC(Block.noz * sizeof(int))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        if ((Block.ozptr = (void **)MALLOC(Block.noz * sizeof(void *))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }


        for (int i = 0; i < Block.noz; ++i)
        {
            pIT = l->get(i);

            if (pIT->isGenericType())
            {
                types::GenericType* pGT = pIT->getAs<types::GenericType>();
                Block.ozsz[i] = pGT->getRows();
                Block.ozsz[Block.noz + i] = pGT->getCols();
            }

            if (pIT->isDouble())
            {
                d = pIT->getAs<types::Double>();
                Block.ozsz[i] = d->getRows();
                Block.ozsz[Block.noz + i] = d->getCols();
                const int size = d->getSize();
                const double* const r = d->get();
                if (d->isComplex() == false)
                {
                    Block.oztyp[i] = 10;
                    if ((Block.ozptr[i] = (double *)MALLOC(size * sizeof(double))) == nullptr)
                    {
                        freeBlock(&Block);
                        Scierror(888, _("%s : Allocation error.\n"), name.data());
                        return types::Function::Error;
                    }

                    for (int j = 0; j < size; ++j)
                    {
                        ((double*)Block.ozptr[i])[j] = r[j];
                    }
                }
                else
                {
                    Block.oztyp[i] = 11;
                    if ((Block.ozptr[i] = (double *)MALLOC(2 * size * sizeof(double))) == nullptr)
                    {
                        freeBlock(&Block);
                        Scierror(888, _("%s : Allocation error.\n"), name.data());
                        return types::Function::Error;
                    }

                    const double* const im = d->getImg();
                    for (int j = 0; j < size; ++j)
                    {
                        ((double*)Block.ozptr[i])[j] = r[j];
                        ((double*)Block.ozptr[i])[size + j] = im[j];
                    }
                }
            }
            else if (pIT->isInt())
            {
                bool ret = false;
                switch (pIT->getType())
                {
                    case types::InternalType::ScilabInt8:
                        Block.oztyp[i] = 81;
                        ret = alloc_and_set(pIT->getAs<types::Int8>(), &Block.ozptr[i]);
                        break;
                    case types::InternalType::ScilabInt16:
                        Block.oztyp[i] = 82;
                        ret = alloc_and_set(pIT->getAs<types::Int16>(), &Block.ozptr[i]);
                        break;
                    case types::InternalType::ScilabInt32:
                        Block.oztyp[i] = 84;
                        ret = alloc_and_set(pIT->getAs<types::Int32>(), &Block.ozptr[i]);
                        break;
                    case types::InternalType::ScilabUInt8:
                        Block.oztyp[i] = 811;
                        ret = alloc_and_set(pIT->getAs<types::UInt8>(), &Block.ozptr[i]);
                        break;
                    case types::InternalType::ScilabUInt16:
                        Block.oztyp[i] = 812;
                        ret = alloc_and_set(pIT->getAs<types::UInt16>(), &Block.ozptr[i]);
                        break;
                    case types::InternalType::ScilabUInt32:
                        Block.oztyp[i] = 814;
                        ret = alloc_and_set(pIT->getAs<types::UInt32>(), &Block.ozptr[i]);
                        break;
                    default :
                        ret = false;
                        break;
                }

                if (ret == false)
                {
                    freeBlock(&Block);
                    Scierror(888, _("%s : Allocation error.\n"), name.data());
                    return types::Function::Error;
                }
            }
        }
    }

    /* real parameters */
    /* 14 : model.rpar  */
    pIT = m->getField(L"rpar");
    d = pIT->getAs<types::Double>();
    Block.nrpar = d->getSize();
    Block.rpar = nullptr;
    if (Block.nrpar > 0)
    {
        if ((Block.rpar = (double *)MALLOC(Block.nrpar * sizeof(double))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        const double* const r = d->get();
        for (int j = 0; j < Block.nrpar; j++)
        {
            Block.rpar[j] = r[j];
        }
    }

    /* integer parameters */
    /* 15 : model.ipar  */
    pIT = m->getField(L"ipar");
    d = pIT->getAs<types::Double>();
    Block.nipar = d->getSize();
    Block.ipar = nullptr;
    if (Block.nipar > 0)
    {
        if ((Block.ipar = (int *)MALLOC(Block.nipar * sizeof(int))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        const double* const r = d->get();
        for (int j = 0; j < Block.nipar; j++)
        {
            Block.ipar[j] = static_cast<int>(r[j]);
        }
    }

    /* object parameters */
    /* 16 : model.opar  */
    pIT = m->getField(L"opar");
    l = pIT->getAs<types::List>();
    Block.nopar = l->getSize();
    Block.oparsz = nullptr;
    Block.opartyp = nullptr;
    Block.oparptr = nullptr;

    if (Block.nopar > 0)
    {
        if ((Block.oparsz = (int *)MALLOC(2 * Block.nopar * sizeof(int))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        if ((Block.opartyp = (int *)MALLOC(Block.nopar * sizeof(int))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        if ((Block.oparptr = (void **)MALLOC(Block.nopar * sizeof(void *))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        for (int i = 0; i < Block.nopar; ++i)
        {
            pIT = l->get(i);
            if (pIT->isDouble())
            {
                d = pIT->getAs<types::Double>();
                const int size = d->getSize();
                Block.oparsz[i] = d->getRows();
                Block.oparsz[Block.nopar + i] = d->getCols();
                const double* const r = d->get();
                if (d->isComplex() == false)
                {
                    Block.opartyp[i] = 10;
                    if ((Block.oparptr[i] = (double *)MALLOC(size * sizeof(double))) == nullptr)
                    {
                        freeBlock(&Block);
                        Scierror(888, _("%s : Allocation error.\n"), name.data());
                        return types::Function::Error;
                    }

                    for (int j = 0; j < size; ++j)
                    {
                        ((double*)Block.oparptr[i])[j] = r[j];
                    }
                }
                else
                {
                    Block.opartyp[i] = 11;
                    if ((Block.oparptr[i] = (double *)MALLOC(2 * size * sizeof(double))) == nullptr)
                    {
                        freeBlock(&Block);
                        Scierror(888, _("%s : Allocation error.\n"), name.data());
                        return types::Function::Error;
                    }

                    const double* const im = d->getImg();
                    for (int j = 0; j < size; ++j)
                    {
                        ((double*)Block.oparptr[i])[j] = r[j];
                        ((double*)Block.oparptr[i])[size + j] = im[j];
                    }
                }
            }
            else if (pIT->isInt())
            {
                bool ret = false;
                switch (pIT->getType())
                {
                    case types::InternalType::ScilabInt8:
                    {
                        Block.opartyp[i] = 81;
                        types::Int8* pI8 = pIT->getAs<types::Int8>();
                        ret = alloc_and_set(pI8, &Block.oparptr[i]);
                        Block.oparsz[i] = pI8->getRows();
                        Block.oparsz[Block.nopar + i] = pI8->getCols();
                        break;
                    }
                    case types::InternalType::ScilabInt16:
                    {
                        Block.opartyp[i] = 82;
                        types::Int16* pI16 = pIT->getAs<types::Int16>();
                        ret = alloc_and_set(pI16, &Block.oparptr[i]);
                        Block.oparsz[i] = pI16->getRows();
                        Block.oparsz[Block.nopar + i] = pI16->getCols();
                        break;
                    }
                    case types::InternalType::ScilabInt32:
                    {
                        Block.opartyp[i] = 84;
                        types::Int32* pI32 = pIT->getAs<types::Int32>();
                        ret = alloc_and_set(pI32, &Block.oparptr[i]);
                        Block.oparsz[i] = pI32->getRows();
                        Block.oparsz[Block.nopar + i] = pI32->getCols();
                        break;
                    }
                    case types::InternalType::ScilabUInt8:
                    {
                        Block.opartyp[i] = 811;
                        types::UInt8* pUI8 = pIT->getAs<types::UInt8>();
                        ret = alloc_and_set(pUI8, &Block.oparptr[i]);
                        Block.oparsz[i] = pUI8->getRows();
                        Block.oparsz[Block.nopar + i] = pUI8->getCols();
                        break;
                    }
                    case types::InternalType::ScilabUInt16:
                    {
                        Block.opartyp[i] = 812;
                        types::UInt16* pUI16 = pIT->getAs<types::UInt16>();
                        ret = alloc_and_set(pUI16, &Block.oparptr[i]);
                        Block.oparsz[i] = pUI16->getRows();
                        Block.oparsz[Block.nopar + i] = pUI16->getCols();
                        break;
                    }
                    case types::InternalType::ScilabUInt32:
                    {
                        Block.opartyp[i] = 814;
                        types::UInt32* pUI32 = pIT->getAs<types::UInt32>();
                        ret = alloc_and_set(pUI32, &Block.oparptr[i]);
                        Block.oparsz[i] = pUI32->getRows();
                        Block.oparsz[Block.nopar + i] = pUI32->getCols();
                        break;
                    }
                    default :
                        ret = false;
                        break;
                }

                if (ret == false)
                {
                    freeBlock(&Block);
                    Scierror(888, _("%s : Allocation error.\n"), name.data());
                    return types::Function::Error;
                }
            }
        }
    }

    /* labels */
    /* 20 : model.label  */
    pIT = m->getField(L"label");
    s = pIT->getAs<types::String>();
    Block.label = wide_string_to_UTF8(s->get()[0]);

    /* zero crossing */
    /* 21 : model.nzcross  */
    pIT = m->getField(L"nzcross");
    d = pIT->getAs<types::Double>();
    Block.ng = static_cast<int>(d->get()[0]);
    Block.g = nullptr;
    Block.jroot = nullptr;
    if (Block.ng > 0)
    {
        if ((Block.g = (double *)MALLOC(Block.ng * sizeof(double))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        for (int j = 0; j < Block.ng; j++)
        {
            Block.g[j] = 0;
        }

        if ((Block.jroot = (int *)MALLOC(Block.ng * sizeof(int))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        for (int j = 0; j < Block.ng; j++)
        {
            Block.jroot[j] = 0;
        }
    }

    /* mode */
    /* 22 : model.nmode  */
    pIT = m->getField(L"nmode");
    d = pIT->getAs<types::Double>();
    Block.nmode = static_cast<int>(d->get()[0]);
    if (Block.nmode > 0)
    {
        if ((Block.mode = (int *)MALLOC(Block.nmode * sizeof(int))) == nullptr)
        {
            freeBlock(&Block);
            Scierror(888, _("%s : Allocation error.\n"), name.data());
            return types::Function::Error;
        }

        for (int j = 0; j < Block.nmode; j++)
        {
            Block.mode[j] = 0;
        }
    }

    /* uids */
    /* 23 : model.uid  */
    pIT = m->getField(L"uid");
    s = pIT->getAs<types::String>();
    Block.uid = wide_string_to_UTF8(s->get()[0]);

    if ((Block.work = (void **)MALLOC(sizeof(void *))) == nullptr)
    {
        freeBlock(&Block);
        Scierror(888, _("%s : Allocation error.\n"), name.data());
        return types::Function::Error;
    }

    *Block.work = nullptr;

    out.push_back(createblklist(&Block, -1, Block.type));

    freeBlock(&Block);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

