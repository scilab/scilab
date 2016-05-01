/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
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
extern "C" {

#ifdef _MSC_VER
#include <windows.h>
#endif
#include <stdio.h>
#include "api_scilab.h"
#include "gw_parallel.h"
#include "dynamic_link.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "parameters.h"
#include "Thread_Wrapper.h" /* locks for concurrency access */

#ifdef _MSC_VER
#include "mmapWindows.h"
#else
#include <sys/mman.h>
#ifndef MAP_ANONYMOUS
# define MAP_ANONYMOUS MAP_ANON
#endif
#endif

#include "concurrency.h" /* prototype for concurrency query function */
}

#include <cstdlib>
#include <cstring>
/*#include <sstream> for debug purposes only */
#include "parallel_run.hxx"



#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
#include <limits>
#include <iterator>
/*
*
We can handle k Rhs  et m Lhs. Currently, only real (double) matrix are implemented.
In fact, from a performance point of view, only Random-Acces data types where it is possible to "extract" a column in O(1)
(plus memcpy for macro a args) are making sense. This rules out most Scilab data types except for matrices of double / int.


[R1, ... , Rm] = parallel_run(A1, ... , Ak, f [,Types] [,Dims])

If Args are of différent sizes, the smallest are recycled.

Types : matrix of <=m strings of the names (as in typeof()) of the  m lhs fo 'f'. default to "constant"
Rows : matrix of doubles 1 x <=m or 2 x <=m giving the nb of rows (or rows and columns) of the m lhs of f. default to 1

/!\ due to matrix data layout in Scilab (i.e. Fortran columnswise storage)
, a matrix [a11,a12,a13;a21,a22,a23] contains 3 (ncols) arguments of 2 (nrows) elements;
*/

extern "C"
{
    int sci_parallel_run(char *fname, unsigned long fname_len);
}

namespace
{
int threadConcurrencyLevel;
int processConcurrencyLevel;

/* to distinguish scilab variable 'address' from usual int* */
typedef int* scilabVar_t;

int currentTop;    /* Top is fragile : this var mimics Top to avoid touching the real one */
char* currentFname = 0; /* name of the current Scilab function, to be used un error messages btw, what is the api_scilab equivalent of get_fname()?*/

SciErr err; /* store error status from api_scilab functions */

/* often, we handle nb of dimensions (rows and cols) at once.
* I'd have gone for std::size_t but Scilab uses int :( */
typedef std::pair<int, int> dim_t;

/* A scilab variable description is a typename and a matric dimension, just to add a default constructor on the inherited struct
* character string a static (no dynamic alloc/destruction)
*/
struct scilabDesc_t : std::pair<char const*, dim_t>
{
    scilabDesc_t(char const* name = "constant", dim_t dim = dim_t(1, 1)) /* default variable is a scalar i.e. 1x1 matrix of typename "constant" */
        : std::pair<char const *, dim_t>(name, dim)
    {
    }
    /* for debug purposes only
    std::string toString() const
    {
    std::stringstream buf;
    buf<<std::pair<char const*, dim_t>::first<<" :"<<std::pair<char const*, dim_t>::second.first
    <<" x "<<std::pair<char const*, dim_t>::second.second;
    return buf.str();
    }
    */
};

/* types to be used in unions of function pointers */
typedef void (*functionToCall_t)(char const*const*, char*const*);
typedef void (*wrapperFunction_t)(double const*, double*);
typedef void (*loadedFunction_t)();
typedef void (*simpleFunction_t)(int );

/* ptr to data type */
typedef union
{
    double* doublePtr;
    int* intPtr;
    char* bytePtr;
    char** strArrayPtr;
    void* opaquePtr;
    int sciCFunction;
} unionOfPtrs_t;

/* a functional version of getVarType that can be composed
* @param int* address of the scilab var
* @return the type (0 on error)
*/
int getVarType(scilabVar_t var)
{
    int res(0);
    err = getVarType(pvApiCtx, var, &res);
    return res;
}

/* return the typename (as in typeof() )
* @param int* address of the scilab var
* @return the name
* only implements the current valid types for parallel_run args.
*/
char const* getTypeName(scilabVar_t var)
{
    char const* res;
    switch (getVarType(var))
    {
        case sci_matrix:
        {
            res = "constant";
            break;
        }
        default:
        {
            res = "unimplemented type in getTypeName";
        }
    }
    return res;
}
/* get nb of Rows & Cols of a Scilab var : a fonctional version of getVarDimension that can be composed.
* @param the address of the variable
* @return the dimensions in a std::pair<int,int> of nb of rows, nb of columns. (0,0) on error.
*/
dim_t getRowsCols(scilabVar_t var)
{
    dim_t res(0, 0);
    err = getVarDimension(pvApiCtx, var, &res.first, &res.second);
    return res;
}
/* get nb of Rows of a Scilab var (would not be needed with tr1)
* @param the address of the variable
* @return the nb of rows, 0 on error.
*/
int getRows(scilabVar_t var)
{
    return getRowsCols(var).first;
}
/* get nb of Columns of a Scilab var (would not be needed with tr1)
* @param the address of the variable
* @return the nb of columns, 0 on error.
*/
int getCols(scilabVar_t var)
{
    return getRowsCols(var).second;
}

/* test if the scilab var can be a function (either because it *is* one or because it can be the name of a function : 1x1 string marix)
* @param var the scilab var
* @return bool true iff it can be a function
*/
bool isFunctionOrString(scilabVar_t var)
{
    bool res;
    switch (getVarType(var))
    {
        case sci_u_function :
        case sci_c_function :
        {
            res = true;
            break;
        }
        case sci_strings :
        {
            res = (getRows(var) == 1) && (getCols(var) == 1);
            break;
        }
        default :
        {
            res = false;
        }
    }
    return res;
}
/* get ptr to data of a Scilab variable
* @param var 'address' of the scilab var
* @return ptr to the data
*/
unionOfPtrs_t getData(scilabVar_t var)
{
    unionOfPtrs_t res = {0};
    switch (getVarType(var)) // unhandled data types should be caught during arg validation
    {
        case sci_matrix :
        {
            if (!isVarComplex(pvApiCtx, var))
            {
                int unused;
                err = getMatrixOfDouble(pvApiCtx, var, &unused, &unused, &res.doublePtr);
            }
            else
            {
                /* TODO suggest workaround in tutorial */

                //              std::cerr<<"complex data not yet implemented var @"<<var<<std::endl;
            }
            break;
        }
        case sci_strings :
        {
            //          std::cerr<<"getData() string data not yet implemented"<<std::endl;
            break;
        }
        default :
        {
            //          std::cerr<<"getData() data type"<<getVarType(var)<<" not yet implemented"<<std::endl;
            //          abort();
        }
    }
    return res;
}
/* get size of an element in a scilab matrix data structure.
* @param var 'address' of the scilab var
* @return the size in bytes, 0 on error.
*/
std::size_t getSizeOfElement(scilabVar_t var)
{
    std::size_t res(0);
    switch (getVarType(var))
    {
        case sci_matrix :
        {
            res = sizeof(double);
            break;
        }
        default : /* returns 0 */
        {
            //    std::cerr<<"getSizeOfElt() @"<<var<<":data type not yet implemented"<<std::endl;
        }
    }
    return res;
}

/* get size of a columns in a scilab matrix data structure.
* @param var 'address' of the scilab var
* @return the size in bytes, 0 on error.
*/
std::size_t getSizeOfColumn(scilabVar_t var)
{
    return getSizeOfElement(var) * getRows(var) ;
}

/* get size of the data in a scilab matrix data structure.
* @param var 'address' of the scilab var
* @return the size in bytes, 0 on error.
*/
std::size_t getSizeOfData(scilabVar_t var)
{
    return getSizeOfColumn(var) * getCols(var) ;
}

/* computes a dimension that is either
* a slice (one column), or the concatenation of n matrix
* For n=1, the dimension is untouched.
* @param d dimension to slice or concatenate
* @return the new dimension
*/
dim_t sliceOrConcat(dim_t d, std::size_t n = 1)
{
    switch (n)
    {
        case 0 :
        {
            d.second = 1; /* slice : one column */
            break;
        }
        case 1:
            break;
        default :
        {
            d.first *= d.second;
            d.second = static_cast<int>(n);
        }
    }
    return d;
}

/* Get the description from a scilab variable.
* If n is provided, it instead returns the description of
* either a slice (n==0) or a concatenation (n>1) of the variable.
* @param var scilab variable address
* @param n either slice or concatenation
* @return the description
*/
scilabDesc_t getDesc(scilabVar_t var, std::size_t n = 1)
{
    return scilabDesc_t( getTypeName(var), sliceOrConcat(getRowsCols(var), n));
}

/* allocate a scilab variable according to a provided description.
* If n is provided, it instead allocates
* either a slice (n==0) or a concatenation (n>1) of the variable.
* @param d scilab variable description
* @param n either slice or concatenation
* @return the variable address
* only real matrices are implemented yet.
*/
scilabVar_t allocVar(scilabDesc_t d, std::size_t n = 1)
{
    scilabVar_t res(0);
    if (std::strcmp(d.first, "constant") == 0)
    {
        double* unused;
        dim_t toAlloc(sliceOrConcat(d.second, n));
        err = allocMatrixOfDouble(pvApiCtx, ++currentTop, toAlloc.first, toAlloc.second, &unused);
        ++Nbvars;
        //          std::cerr<<"alloc var :"<<d.toString()<<" @"<<(currentTop)<<" with Nbvars="<<Nbvars<<std::endl;
        err = getVarAddressFromPosition(pvApiCtx, currentTop, &res);
    }
    else /* unhandled type should be caught at arg validation time */
    {
        //          std::cerr<<"allocVar() "<<d.first<<" data type not yet implemented"<<std::endl;
    }
    return res;
}
/* Ensures that the complete expected var is filled with reasonable default values
* when the returned var was smaller than expected
*
* i.e. we just copied the data from a resultVar @ varData
* when we were expecting an expectedVar : we fill the rest.
*
* @param d scilab variable description
* @param n either slice or concatenation
* @return the variable address
* only real matrices are implemented yet.
*/
void fillUndefinedData(void* varData, scilabDesc_t resultVar, scilabDesc_t expectedVar)
{
    if (!std::strcmp(resultVar.first, expectedVar.first))
    {
        if (!strcmp(resultVar.first, "constant"))
        {
            std::size_t const nbFilled(resultVar.second.first * resultVar.second.second);
            std::fill_n(static_cast<double *>(varData) + nbFilled
                        , expectedVar.second.first * expectedVar.second.second - nbFilled
                        , std::numeric_limits<double>::quiet_NaN() );

        }
    }
}
/*
wrapper on a native c function or a scilab macro called on scilab variables.

constructed on :
- scilab variable for the function (external native function name, sci_c_function (buggy) or macro name)scilab matrices of arguments
- expected lhs

upon construction, allocate scilab result variables and computes all necessary meta data.
*/
struct wrapper
{

    typedef std::vector<std::size_t> sizes_container;

    /* wrapper contructor
    * @param args_begin iterator to the first args of parallel_run
    * @param function_it iterator to the function argument of parallel_run
    * @param args_end iterator past the end of parallel_run args
    * @param function_lhs number of lhs (of parallel_run and of the function: it is the same)
    */
    template<typename VarsIt>
    wrapper(VarsIt begin, VarsIt functionIt, VarsIt end, std::size_t functionLhs)
    {
        registerArgs(begin, functionIt);
        n = *std::max_element(argsNb.begin(), argsNb.end());
        getFunction(*functionIt);
        allocCompleteResults(begin, functionIt + 1, end, functionLhs);
    }

    /* the member function performing the call to the function (foreign function of Scilab macro)
    * @ param args array of ptrs to args data
    * @ param res array of ptrs to res data
    */
    void operator()(char const** args, char ** res)
    {
        (*this.*(this->fPtr))(args, res);
    }

    /* It is idiomatic to pass functors by value in C++, but our wrapper is heavy,
    * so we provide a lightweight handle */
    struct handle
    {
        handle(wrapper& r) : w(r)
        {
        }
        /* just forward to the underlying wrapper */
        void operator()(char const** args, char ** res) const
        {
            w(args, res);
        }
        wrapper& w;
    };
    handle getHandle()
    {
        return handle(*this);
    }
    /* @return begin iterator to the array of pointers to arguments data */
    char const*const* argsDataBegin() const
    {
        return &argsData[0].bytePtr;
    }
    /* @return begin iterator to the array of arguments sizes */
    std::size_t const* argsSizesBegin() const
    {
        return &argsSizes[0];
    }
    /* @return begin iterator to the array of arguments number of elements (they are not requires to have the same nb of elements */
    std::size_t const* argsNbBegin() const
    {
        return &argsNb[0];
    }
    /* @return nb of tasks (calls) to perform = max(args_nb_begin(), args_nb_begin()+rhs) */
    std::size_t tasksNb() const
    {
        return n;
    }
    /* @return begin iterator to the array of pointers to result data */
    char * const* resDataBegin()
    {
        return &resData[0].bytePtr;
    }
    /* @return begin iterator to the array of results sizes */
    std::size_t const* resSizesBegin() const
    {
        return &resSizes[0];
    }
    /* @return nb of rhs vars of the function */
    std::size_t nbRhs() const
    {
        return rhsDesc.size();
    }
    /* @return true if the underlying function is a foreign function, false if it is a Scilab macro */
    bool isForeignFunction() const
    {
        return function.toCall != 0 ;
    }

private:
    /* ptr to foreign function (types used where storing, calling or wrapping) */
    union
    {
        functionToCall_t toCall;
        loadedFunction_t toLoad;
        wrapperFunction_t wrapper;
    } function;
    /* register a matrix of arguments to be used as rhs) */
    template<typename ArgIt>
    void registerArgs(ArgIt it, ArgIt end)
    {
        std::transform(it, end, std::back_inserter(argsData), &getData);
        std::transform(it, end, std::back_inserter(argsSizes), &getSizeOfColumn);
        std::transform(it, end, std::back_inserter(argsNb), &getCols);
        std::transform(it, end, std::back_inserter(rhsDesc), std::bind2nd(std::ptr_fun(&getDesc), 0)); /* get a slice as model for function rhs*/
    }

    /* alloc the scilab variables that will hold the complete collection of results
    * @param first_arg_position only used to compute the args positions for error messages
    * @param res_types_begin, res_types_end iterator range on the args describing result, can be empty
    * @param nb_lhs number of lhs    */
    template<typename VarPtrIt>
    void allocCompleteResults(VarPtrIt begin, VarPtrIt resBegin, VarPtrIt resEnd, std::size_t nbLhs)
    {
        lhsDesc.resize(nbLhs);
        if (resBegin != resEnd)
        {
            if (getVarType(*resBegin) == sci_strings)
            {
                //    std::cerr<<"we have a type lhs arg\n";
                ++resBegin;
            }
            if (resBegin != resEnd)
            {
                if (getVarType(*resBegin) == sci_matrix)
                {
                    //    std::cerr<<"we have a dim lhs arg\n";
                    dim_t const tmp(getRowsCols(*resBegin));
                    double const*const data(getData(*resBegin).doublePtr);
                    switch (tmp.second)
                    {

                        case 2:
                        {
                            //            std::cerr<<"we have rows and cols\n";
                            for (int i(0); i < tmp.first && i < tmp.first * tmp.second; ++i)
                            {
                                lhsDesc[i].second.first = static_cast<int>(data[i]);
                                lhsDesc[i].second.second = static_cast<int>(data[i + tmp.first]);
                            }
                            break;
                        }
                        case 1:
                        {
                            //            std::cerr<<"we have rows \n";
                            for (int i(0); i < tmp.first && i < tmp.first * tmp.second; ++i)
                            {
                                lhsDesc[i].second.first = static_cast<int>(data[i]);
                            }
                            break;
                        }
                        default :
                        {
                            Scierror(999, _("%s: Wrong size of input argument #%d: Number of columns are incompatible ")
                                     , currentFname, std::distance(begin, resBegin));
                        }
                    }
                }
            }
        }
        /* we want to have to result var at hand before calling the scilab macro so we must create it now before the args */
        std::transform(lhsDesc.begin(), lhsDesc.end(), std::back_inserter(scilabCollectionsOfLhs), std::bind2nd(std::ptr_fun(&allocVar), n));
        /* we store addr of result data and the size for the parallel wrapper */
        std::transform(scilabCollectionsOfLhs.begin(), scilabCollectionsOfLhs.end(), std::back_inserter(resData), &getData);
        std::transform(scilabCollectionsOfLhs.begin(), scilabCollectionsOfLhs.end(), std::back_inserter(resSizes), &getSizeOfColumn);
    }

    /* extract the function form the scilab variable (i.e.string) reprensenting it.
    * @param v the variable
    * @return nothing useful but GetRhsVar() macro wants to be able to return an int :(
    */
    int getFunction(scilabVar_t var)
    {
        function.toCall = 0;
        switch (getVarType(var))
        {
            case sci_c_function :
            {
                int unused[2];
                GetRhsVar(2, EXTERNAL_DATATYPE, unused, unused + 1, &scilabFunction);
                fPtr = &wrapper::macro<false>;
                break;
            }
            case sci_strings :
            {
                char* funName;
                getAllocatedSingleString(pvApiCtx, var, &funName);
                int found;
                found = SearchInDynLinks(funName, &function.toLoad);
                fPtr = &wrapper::nativeFunction;
                if (found == -1)
                {
                    /* should check amongst defined macros with getmacroslist (cf dans core/src/c/getvariablesname.c) and check that type is sci_XXX */
                    function.toCall = 0;
                    scilabFunctionName = funName;
                    scilabFunctionNameLength = std::strlen(scilabFunctionName);
                    fPtr = &wrapper::macro<true>;
                }
            }
        }
        return 0;
    }

    /* performs the Scilab macro call
    * @param byName bool template parameter tells if the macro is called by name or by ptr (ptr is currently broken).
    * @param args array of ptrs to args data
    * @param res array of ptr to res data
    */
    template<bool byName>
    void macro(char const** args, char ** res)
    {
        /* rhs models from  */
        int saveNbvars = Nbvars, saveTop = currentTop;
        for ( std::vector<scilabDesc_t>::const_iterator it(rhsDesc.begin())
                ; it != rhsDesc.end(); ++it, ++args)
        {
            scilabVar_t scilabArg = allocVar(*it); /* create a var for a slice (col)of the parallel_run Rhs arg */
            memcpy(getData(scilabArg).bytePtr, *args, getSizeOfData(scilabArg));
        }

        int  sciRhs = static_cast<int>(rhsDesc.size());
        int  sciLhs = static_cast<int>(lhsDesc.size());

        std::size_t dummyVars(0); /* alloc safety variable to ensure space on the stack upon return*/
        int sciArgPos = saveTop + 1;
        for ( ; sciRhs + dummyVars < sciLhs + maxSafetyLhs; ++dummyVars, ++Nbvars)
        {
            double* unused;
            err = allocMatrixOfDouble(pvApiCtx, ++currentTop, 0, 0, &unused);
        }
        Nbvars = Rhs + Lhs + sciRhs;
        bool success(byName
                     ? C2F(scistring)(&sciArgPos, scilabFunctionName, &sciLhs, &sciRhs, static_cast<unsigned long>(scilabFunctionNameLength))
                     : C2F(scifunction)(&sciArgPos, &scilabFunction, &sciLhs, &sciRhs)
                    );
        // result r is now on first position on stack
        {
            Nbvars = static_cast<int>(Rhs + Lhs + sciRhs + dummyVars);
            int resPos = Rhs + Lhs + 1; //+1

            for ( std::vector<scilabDesc_t>::iterator it(lhsDesc.begin())
                    ; it != lhsDesc.end(); ++it, ++resPos, ++res)
            {
                scilabVar_t scilabRes;
                if (success)
                {
                    err = getVarAddressFromPosition(pvApiCtx, resPos, &scilabRes);
                }
                scilabDesc_t resDesc;
                if (!success || err.iErr)
                {
                    /* there was an error getting the result variable */
                    resDesc = *it; /* pretend we got the right type */
                    resDesc.second.first = resDesc.second.second = 0; /* but 0 elements */
                }
                else
                {
                    /* copy the returned data */
                    memcpy(*res, getData(scilabRes).bytePtr, getSizeOfData(scilabRes));
                    resDesc = getDesc(scilabRes);
                }
                fillUndefinedData(*res, resDesc, *it);
            }
            Nbvars = saveNbvars;
            currentTop = saveTop;
        }
    }
    void nativeFunction(char const** args, char ** res)
    {
        function.toCall(args, res);
    }

    /* we prealloc as much scilab var more than requested lhs in case the scilab macro call back returns more thant requested.*/
    static unsigned int const maxSafetyLhs = 20;

    std::size_t n; /* nb of calls to perform */
    sizes_container argsSizes, argsNb, resSizes; /* sizes of arguments, nb of elements for each argument, sizes for results */
    std::vector<unionOfPtrs_t> argsData; /* ptrs to args data */
    std::vector<unionOfPtrs_t> resData; /* ptrs to res data */

    /* the member function to call, dispatches to macro of foreign function */
    void(wrapper::*fPtr)(char const** args, char ** res);

    int scilabFunction; /* the scilab function 'ptr' for scifunction */
    char* scilabFunctionName;/* the scilab function name for scistring */
    std::size_t scilabFunctionNameLength;/* the scilab function name length for scistring */

    /* store models of scilab lhs and rhs variables */
    std::vector<scilabDesc_t> lhsDesc, rhsDesc;
    std::vector<scilabVar_t> scilabCollectionsOfLhs;    /* lhs vars of the parallel_run function : collections of the lhs form the function*/

};
/* Checks if the function parallel_run arguments are valid.
* 1 or more matrices of doubles
* 1 matrix of 1 string
* 0 or 1 matrix of strings and/or 1 matrix of doubles with 1 ou 2 columns
* 0 or 1 configuration plist
*
* @retun true is the args are valid */
bool check_args(void)
{
    {
        if (Rhs < 2)
        {
            return false;
        }
        bool before_function(true), at_least_one_arg(false);
        bool ok(true);
        for ( int pos(1); pos <= Rhs && ok; ++pos)
        {
            int* addr;
            err = getVarAddressFromPosition(pvApiCtx, pos, &addr);
            int type;
            err = getVarType( pvApiCtx, addr, &type );
            if (before_function)
            {
                switch (type)
                {
                    case sci_matrix :
                    {
                        /* check not complex "%s: This feature has not been implemented.\n" */
                        ok = !isVarComplex( pvApiCtx, addr);
                        at_least_one_arg = true;
                        break;
                    }
                    case sci_strings :
                    {
                        /* check dim = 1x1 */
                        int rows, cols;
                        err = getMatrixOfString(pvApiCtx, addr, &rows, &cols, 0, 0);
                        ok = (rows == 1) && (cols == 1);
                        }/* no break */
                    case sci_c_function:
                    {
                        before_function = false;
                        break;
                    }
                    default :
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), currentFname, 1);
                        ok = false;
                    }
                }
            }
            else
            {
                switch (type)
                {
                    case sci_strings :
                    {
                        break;
                    }
                    case sci_matrix :
                    {
                        /* check not complex and ncol <=2 */
                        ok = !isVarComplex( pvApiCtx, addr);
                        if (ok)
                        {
                            int rows, cols;
                            double* unused;
                            err = getMatrixOfDouble(pvApiCtx, addr, &rows, &cols, &unused);
                            ok = (cols <= 2);
                            break;
                        }
                    }
                    case sci_mlist :  /* config plist (which is a mlist) */
                    {
                        break;
                    }

                    default :
                    {
                        ok = false;
                    }
                }
            }
        }
        return ok && at_least_one_arg && (!before_function);
    }
}

/* Get configuration options form the config parameter
*
* in :
* @param config_arg_pos position where the config arg can be
*
* in/out:
*
* @param nb_workers int config value for the key "nb_workers"
* @param shared_memory bool config value for the key "shared_memory"
* @param dynamic_scheduling bool config value for the key "dynamic_scheduling"
* @param chunk_size int config value for the key "chunk_size"
* @param prologue char* config value for the key "prologue"
* @param prologue char* config value for the key "epilogue"
*
* @return bool true if there was a configuration argument in position config_arg_pos.
*/
bool getConfigParameters
(int config_arg_pos, int& nb_workers, bool& shared_memory, bool& dynamic_scheduling, int& chunk_size, char const*& prologue, char const*& epilogue)
{
    int log(0);
    int* addr = NULL;
    getVarAddressFromPosition(pvApiCtx, config_arg_pos, &addr);
    bool has_config_arg(checkPList(pvApiCtx, addr) != 0);
    if (has_config_arg)
    {
        int found;
        getIntInPList(pvApiCtx, addr, "nb_workers", &nb_workers, &found, nb_workers, log,  CHECK_NONE);
        int tmp;
        getIntInPList(pvApiCtx, addr, "shared_memory", &tmp, &found, shared_memory ? 1 : 0, log, CHECK_NONE);
        shared_memory = (tmp != 0);
        getIntInPList(pvApiCtx, addr, "dynamic_scheduling", &tmp, &found, dynamic_scheduling ? 1 : 0, log, CHECK_NONE);
        dynamic_scheduling = (tmp != 0);
        getIntInPList(pvApiCtx, addr, "chunk_size", &chunk_size, &found, chunk_size, log, CHECK_NONE);
        getStringInPList(pvApiCtx, addr, "prologue", const_cast<char**>(&prologue), &found, const_cast<char*>(prologue), log, CHECK_NONE);
        getStringInPList(pvApiCtx, addr, "epilogue", const_cast<char**>(&epilogue), &found, const_cast<char*>(epilogue), log, CHECK_NONE);
    }
    return has_config_arg;
}

/*
A simple wrapper just wraps prologues and epilogues, each taking only a scalar argument (the process number).
*/
struct simple_wrapper
{
    /* the constructor
    * @param the macro or foreign function name, empty string allowed: the function then does nothing.
    */
    explicit simple_wrapper(char const* name): fun(name)
    {
    }
    /* the operator : calls the function or macro passing a scalar argument on the stack
    * @param i the scalar to pass on the stack as a real value.
    */
    void operator()(int i)
    {
        if (*fun)
        {
            /* do nothing on empty name */
            union
            {
                loadedFunction_t toLoad;
                simpleFunction_t toCall;
            } function;
            int found = SearchInDynLinks(const_cast<char*>(fun), &function.toLoad);
            if (found != -1)
            {
                function.toCall(i);
            }
            else
            {
                double* tmpPtr;
                err = allocMatrixOfDouble(pvApiCtx, ++Top, 1, 1, &tmpPtr);
                *tmpPtr = static_cast<double>(i);
                ++Nbvars;
                int lhs(0), rhs(1);
                C2F(scistring)(&(Top), const_cast<char*>(fun), &lhs, &rhs, static_cast<unsigned long>(strlen(fun)));
                --Nbvars;
                --Top;
            }
        }
    }
    char const* fun;
};

struct ConcurrencyState
{
    ConcurrencyState()
    {
        lock = static_cast<__threadSignalLock*>(mmap(0, sizeof( __threadSignalLock), PROT_READ | PROT_WRITE, MAP_SHARED |  MAP_ANONYMOUS, -1, 0));
        processConcurrencyLevelPtr = static_cast<int*>(mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED |  MAP_ANONYMOUS, -1, 0));
        __InitSignalLock(lock);
    }

    struct ScopedUpdater
    {
        ScopedUpdater(ConcurrencyState& c, bool threads)
            : lock(c.lock), countPtr( threads
                                      ? & c.threadConcurrencyLevel
                                      : c.processConcurrencyLevelPtr)
        {
            __LockSignal(lock);
            ++(*countPtr);
            __UnLockSignal(lock);
        }
        ~ScopedUpdater()
        {
            __LockSignal(lock);
            --(*countPtr);
            __UnLockSignal(lock);
        }
        __threadSignalLock* lock;
        int* countPtr;
    };
    ScopedUpdater scopedUpdater(bool threads)
    {
        return ScopedUpdater(*this, threads);
    }

    int get() const
    {
        __LockSignal(lock);
        int res((threadConcurrencyLevel ? 1 : 0) | (*processConcurrencyLevelPtr ? 2 : 0));
        __UnLockSignal(lock);
        return res;
    }

    ~ConcurrencyState()/* called by exit() for static variables */
    {
        __UnLockSignal(lock);
    }
    __threadSignalLock* lock;
    int* processConcurrencyLevelPtr; /* must be in shared mem */
    int threadConcurrencyLevel;
};


}

ConcurrencyState concurrencyState;

int parallelConcurrency()
{
    return concurrencyState.get();
}

int parallelForbidden(char const* fname)
{
    Scierror(999, _("%s: This function is forbidden in a concurrent execution context.\n"), fname);
    return 0;
}



/* Calling point from Scilab.
* checking args and contruction a wrapper around function call of a foreign function or a Scilab macro.
* this wrapper (in fact, a handle) is then passed to another wrapper that will parallelize the calls.
* the parallel wrapper is independant of Scilab (thanks to this wrapper) and is implemented in parallel_wrapper.hpp.
*
* Calling parallel_run is
* 1 checking args
* 2 constructing wrapper pre allocating result Scilab vars and abstracting arrays of args and results pointers
* (in parallel_wrapper )
* 3 contructing a parallel_wrapper
* 4 calling the parallel_wrapper according to config options (i.e. nb of workers)
* 4.1 for each call to be made, adjusting the args and res ptr
* 4.2 calling the wrapper
*
*/
int sci_parallel_run(char *fname, unsigned long fname_len)
{
    typedef std::vector<scilabVar_t> varsContainer_t;
    currentFname = fname; //get_fname(fname, fname_len); uses a static buffer :(
    currentTop = Rhs;
#ifdef _MSC_VER
    Nbvars = max(Rhs, Top);
#else
    Nbvars = std::max(Rhs, Top);
#endif
    if ( !check_args())
    {
        Scierror(999, _("%s: Wrong number of input argument(s).\n"), fname); /* need a better error message */
        return 0;
    }


    int nbArgsToHandle(Rhs);
    /* parameters default values */
    int nbWorkers(0), chunkSize(1);
    bool sharedMemory(false), dynamicScheduling(false);
    char const* prologueName("");
    char const* epilogueName("");
    /* If there is a config parameter, use it to update the parameters value */
    if ( getConfigParameters(Rhs, nbWorkers, sharedMemory, dynamicScheduling, chunkSize, prologueName, epilogueName))
    {
        --nbArgsToHandle;
    }
    varsContainer_t stack(nbArgsToHandle);
    for (int i(0); i != nbArgsToHandle; ++i)
    {
        err = getVarAddressFromPosition(pvApiCtx, i + 1, &stack[i]);
    }
    varsContainer_t::iterator functionArg = std::find_if(stack.begin(), stack.end(), &isFunctionOrString);
    wrapper w(stack.begin(), functionArg, stack.end(), Lhs);
    bool const withThreads(w.isForeignFunction() && sharedMemory);

    ConcurrencyState::ScopedUpdater u(concurrencyState.scopedUpdater(withThreads));

    simple_wrapper prologue(prologueName), epilogue(epilogueName);

    make_parallel_wrapper(w.argsDataBegin(), w.argsSizesBegin(), w.argsNbBegin(), w.nbRhs(), w.tasksNb()
                          ,  w.resDataBegin(), w.resSizesBegin()
                          , Lhs, w.getHandle(), prologue, epilogue)(withThreads, nbWorkers, dynamicScheduling, chunkSize);

    for (int i(0); i != Lhs; ++i)
    {
        LhsVar(i + 1) = Rhs + i + 1;
    }

    PutLhsVar(); /* to be moved to gateway */

    return 0;
}
