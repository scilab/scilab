/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#include "types.hxx"
#include "double.hxx"
#include "opexp.hxx"
#include <functional>

#include "dynlib_ast.h"

EXTERN_AST void initOperationArray();
EXTERN_AST bool checkSameSize(types::GenericType*, types::GenericType*);
EXTERN_AST std::wstring errorSameSize(types::GenericType*, types::GenericType*, std::wstring op);
EXTERN_AST std::wstring errorMultiplySize(types::GenericType*, types::GenericType*);

inline auto makeStrides = [](const std::vector<int>& dims)
{
    std::vector<int> strides(dims.size(), 1);
    int s = 1;
    for (size_t k = 0; k < dims.size(); ++k)
    {
        strides[k] = s;
        s *= dims[k];
    }
    return strides;
};

//check if sizes allow expansion : for each dimension, sizes must be equal or one of them must be 1
inline bool canExpandSize(types::GenericType* l, types::GenericType* r)
{
    int iDimsL = l->getDims();
    int iDimsR = r->getDims();
    int* piDimsL = l->getDimsArray();
    int* piDimsR = r->getDimsArray();

    int iDims = std::max(iDimsL, iDimsR);
    for (int i = 0; i < iDims; ++i)
    {
        int iL = i < iDimsL ? piDimsL[i] : 1;
        int iR = i < iDimsR ? piDimsR[i] : 1;
        if (iL != iR && iL != 1 && iR != 1)
        {
            return false;
        }
    }

    return true;
}

struct ExpandPlan
{
    std::vector<int> dimsO;
    std::vector<int> dimsL;
    std::vector<int> dimsR;
    std::vector<int> stridesL;
    std::vector<int> stridesR;
    int size = 0;
};

template<class T, class U, class O>
inline ExpandPlan makeExpandPlan(T* l, U* r, O*& o, const std::wstring& op)
{
    std::vector<int> dimsL(l->getDimsArray(), l->getDimsArray() + l->getDims());
    std::vector<int> dimsR(r->getDimsArray(), r->getDimsArray() + r->getDims());

    while (dimsL.size() < dimsR.size())
        dimsL.push_back(1);
    while (dimsR.size() < dimsL.size())
        dimsR.push_back(1);

    ExpandPlan plan;
    plan.dimsL = dimsL;
    plan.dimsR = dimsR;
    plan.size = 1;
    for (size_t i = 0; i < dimsL.size(); ++i)
    {
        if (dimsL[i] != dimsR[i] && dimsL[i] != 1 && dimsR[i] != 1)
        {
            throw ast::InternalError(errorSameSize(l, r, op));
        }

        plan.dimsO.push_back(std::max(dimsL[i], dimsR[i]));
        plan.size *= plan.dimsO.back();
    }
    plan.stridesL = makeStrides(dimsL);
    plan.stridesR = makeStrides(dimsR);

    o = new O(plan.dimsO.size(), plan.dimsO.data());
    if (l->isDouble() && r->isDouble() && o->isDouble())
    {
        if (l->template getAs<types::Double>()->isComplex() || r->template getAs<types::Double>()->isComplex())
        {
            o->template getAs<types::Double>()->setComplex(true);
        }
    }

    return plan;
}

template<class Fn>
inline void expandApply(const ExpandPlan& plan, Fn&& fn)
{
    std::vector<int> idx(plan.dimsO.size(), 0);
    int iL = 0, iR = 0;
    for (int iO = 0; iO < plan.size; ++iO)
    {
        fn(iL, iR, iO);

        for (size_t k = 0; k < plan.dimsO.size(); ++k)
        {
            ++idx[k];
            if (idx[k] < plan.dimsO[k])
            {
                if (plan.dimsL[k] != 1)
                    iL += plan.stridesL[k];
                if (plan.dimsR[k] != 1)
                    iR += plan.stridesR[k];
                break;
            }
            idx[k] = 0;
            if (plan.dimsL[k] != 1)
                iL -= plan.stridesL[k] * (plan.dimsL[k] - 1);
            if (plan.dimsR[k] != 1)
                iR -= plan.stridesR[k] * (plan.dimsR[k] - 1);
        }
    }
}
