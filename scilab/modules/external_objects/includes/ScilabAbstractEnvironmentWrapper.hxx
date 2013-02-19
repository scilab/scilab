/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Igor GRIDCHYN
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCILABABSTRACTENVIRONEMNTWRAPPER_HXX__
#define __SCILABABSTRACTENVIRONEMNTWRAPPER_HXX__

#include "ScilabAbstractMemoryAllocator.hxx"

namespace org_modules_external_objects
{
class ScilabAbstractEnvironmentWrapper
{

public:

    /**
         * Wrap a double into an external object
     * @param x the value to wrap
         * @return an id corresponding to the object
         */
    virtual int wrap(double * x, const bool isRef) const = 0;

    /**
         * Wrap a row of doubles into an external object
     * @param x the values to wrap
     * @param xSize the number of elements
         * @return an id corresponding to the object
         */
    virtual int wrap(double * x, int xSize, const bool isRef) const = 0;

    /**
         * Wrap a matrix of doubles into an external object
     * @param x the values to wrap
     * @param xSize the number of rows
     * @param xSizeCol the number of cols
         * @return an id corresponding to the object
         */
    virtual int wrap(double * x, int xSize, int xSizeCol, const bool isRef) const = 0;

    virtual int wrap(double * re, double * im, const bool isRef) const = 0;

    virtual int wrap(double * re, double * im, int xSize, const bool isRef) const = 0;

    virtual int wrap(double * re, double * im, int xSize, int xSizeCol, const bool isRef) const = 0;

    virtual int wrap(char * x, const bool isRef) const = 0;

    virtual int wrap(char * x, int xSize, const bool isRef) const = 0;

    virtual int wrap(char * x, int xSize, int xSizeCol, const bool isRef) const = 0;


    virtual int wrap(unsigned char * x, const bool isRef) const = 0;

    virtual int wrap(unsigned char * x, int xSize, const bool isRef) const = 0;

    virtual int wrap(unsigned char * x, int xSize, int xSizeCol, const bool isRef) const = 0;


    virtual int wrap(short * x, const bool isRef) const = 0;

    virtual int wrap(short * x, int xSize, const bool isRef) const = 0;

    virtual int wrap(short * x, int xSize, int xSizeCol, const bool isRef) const = 0;


    virtual int wrap(unsigned short * x, const bool isRef) const = 0;

    virtual int wrap(unsigned short * x, int xSize, const bool isRef) const = 0;

    virtual int wrap(unsigned short * x, int xSize, int xSizeCol, const bool isRef) const = 0;


    virtual int wrap(int * x, const bool isRef) const = 0;

    virtual int wrap(int * x, int xSize, const bool isRef) const = 0;

    virtual int wrap(int * x, int xSize, int xSizeCol, const bool isRef) const = 0;


    virtual int wrap(unsigned int * x, const bool isRef) const = 0;

    virtual int wrap(unsigned int * x, int xSize, const bool isRef) const = 0;

    virtual int wrap(unsigned int * x, int xSize, int xSizeCol, const bool isRef) const = 0;

#ifdef __SCILAB_INT64__

    virtual int wrap(long long * x, const bool isRef) const = 0;

    virtual int wrap(long long * x, int xSize, const bool isRef) const = 0;

    virtual int wrap(long long * x, int xSize, int xSizeCol, const bool isRef) const = 0;

    virtual int wrap(unsigned long long * x, const bool isRef) const = 0;

    virtual int wrap(unsigned long long * x, int xSize, const bool isRef) const = 0;

    virtual int wrap(unsigned long long * x, int xSize, int xSizeCol, const bool isRef) const = 0;

#endif

    virtual int wrap(char ** x, const bool isRef) const = 0;

    virtual int wrap(char ** x, int xSize, const bool isRef) const = 0;

    virtual int wrap(char ** x, int xSize, int xSizeCol, const bool isRef) const = 0;


    virtual int wrap(float * x, const bool isRef) const = 0;

    virtual int wrap(float * x, int xSize, const bool isRef) const = 0;

    virtual int wrap(float * x, int xSize, int xSizeCol, const bool isRef) const = 0;


    virtual int wrapBool(int * x, const bool isRef) const = 0;

    virtual int wrapBool(int * x, int xSize, const bool isRef) const = 0;

    virtual int wrapBool(int * x, int xSize, int xSizeCol, const bool isRef) const = 0;

    /**
     * Unwrap the external object with the given id into a double
     * @param id the value to wrap
     * @param pos the position in the Scilab stack where to put the result
         */
    virtual void unwrapdouble(int id, const ScilabDoubleStackAllocator & allocator) const = 0;

    virtual void unwraprowdouble(int id, const ScilabDoubleStackAllocator & allocator) const = 0;

    virtual void unwrapmatdouble(int id, const ScilabDoubleStackAllocator & allocator) const = 0;

    virtual void unwrapcomplex(int id, const ScilabComplexStackAllocator & allocator) const = 0;

    virtual void unwraprowcomplex(int id, const ScilabComplexStackAllocator & allocator) const = 0;

    virtual void unwrapmatcomplex(int id, const ScilabComplexStackAllocator & allocator) const = 0;

    virtual void unwrapchar(int id, const ScilabCharStackAllocator & allocator) const = 0;

    virtual void unwraprowchar(int id, const ScilabCharStackAllocator & allocator) const = 0;

    virtual void unwrapmatchar(int id, const ScilabCharStackAllocator & allocator) const = 0;

    virtual void unwrapuchar(int id, const ScilabUCharStackAllocator & allocator) const = 0;

    virtual void unwraprowuchar(int id, const ScilabUCharStackAllocator & allocator) const = 0;

    virtual void unwrapmatuchar(int id, const ScilabUCharStackAllocator & allocator) const = 0 ;

    virtual void unwrapshort(int id, const ScilabShortStackAllocator & allocator) const = 0;

    virtual void unwraprowshort(int id, const ScilabShortStackAllocator  & allocator) const = 0;

    virtual void unwrapmatshort(int id, const ScilabShortStackAllocator & allocator) const = 0;

    virtual void unwrapushort(int id, const ScilabUShortStackAllocator & allocator) const = 0;

    virtual void unwraprowushort(int id, const ScilabUShortStackAllocator & allocator) const = 0;

    virtual void unwrapmatushort(int id, const ScilabUShortStackAllocator & allocator) const = 0;

    virtual void unwrapint(int id, const ScilabIntStackAllocator & allocator) const = 0;

    virtual void unwraprowint(int id, const ScilabIntStackAllocator & allocator) const = 0;

    virtual void unwrapmatint(int id, const ScilabIntStackAllocator & allocator) const = 0;

    virtual void unwrapuint(int id, const ScilabUIntStackAllocator & allocator) const = 0;

    virtual void unwraprowuint(int id, const ScilabUIntStackAllocator & allocator) const = 0;

    virtual void unwrapmatuint(int id, const ScilabUIntStackAllocator & allocator) const = 0;

    virtual void unwraplong(int id, const ScilabLongStackAllocator & allocator) const = 0;

    virtual void unwraprowlong(int id, const ScilabLongStackAllocator & allocator) const = 0;

    virtual void unwrapmatlong(int id, const ScilabLongStackAllocator & allocator) const = 0;

    virtual void unwrapulong(int id, const ScilabULongStackAllocator & allocator) const = 0;

    virtual void unwraprowulong(int id, const ScilabULongStackAllocator & allocator) const = 0;

    virtual void unwrapmatulong(int id, const ScilabULongStackAllocator & allocator) const = 0;

    virtual void unwrapboolean(int id, const ScilabBooleanStackAllocator & allocator) const = 0;

    virtual void unwraprowboolean(int id, const ScilabBooleanStackAllocator & allocator) const = 0;

    virtual void unwrapmatboolean(int id, const ScilabBooleanStackAllocator & allocator) const = 0;

    virtual void unwrapstring(int id, const ScilabStringStackAllocator & allocator) const = 0;

    virtual void unwraprowstring(int id, const ScilabStringStackAllocator & allocator) const = 0;

    virtual void unwrapmatstring(int id, const ScilabStringStackAllocator & allocator) const = 0;

    virtual void unwrapfloat(int id, const ScilabFloatStackAllocator & allocator) const = 0;

    virtual void unwraprowfloat(int id, const ScilabFloatStackAllocator & allocator) const = 0;

    virtual void unwrapmatfloat(int id, const ScilabFloatStackAllocator & allocator) const = 0;
};
}

#endif // __SCILABABSTRACTENVIRONEMNTWRAPPER_HXX__
