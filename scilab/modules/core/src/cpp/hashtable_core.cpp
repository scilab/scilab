/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*----------------------------------------------------------------------------*/
extern "C" {
#include "hashtable_core.h"
#include "stack-def.h"
#include "MALLOC.h"
#include "LoadFunctionsTab.h"
#include "getfunctionslist.h"
#include "existfunction.h"
#include "addinter.h"

    unsigned int hashtable_core_maxFilled = 0;
    extern int C2F(cvname)(int *,char *,int const*, unsigned long int);
#ifdef _MSC_VER
    //#define strdup _strdup
#include "strdup_windows.h"
#endif
}
/*----------------------------------------------------------------------------*/
#include <iterator>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstring> // for std::memset
#include "partition.hxx" // for scilab::core::partition_point_n
#include "unrolled_algorithms.hxx" //for eq_n less_n copy_n
/*----------------------------------------------------------------------------*/
#define MAXLENGHTFUNCTIONNAME  nlgh + 8 /* 24 in fact in scilab +1 for '\0' round to nearest multiple of 8*/
/*----------------------------------------------------------------------------*/
using namespace scilab::core;
/*----------------------------------------------------------------------------*/
namespace {
    struct entry
    {
        entry()
        {
            std::memset(key, 0, sizeof(key)+sizeof(data)+1); // no alignement issues, init ints to 0 and namefunction to ""
        }
        entry(int d, char const* name): data(d)
        {
            int const zero(0);
            if(name)
            {
                strncpy(namefunction, name, MAXLENGHTFUNCTIONNAME);
            }
            else
            {
                namefunction[0]= '\0'; // "" name
            }
            C2F(cvname)(key, namefunction, &zero, static_cast<unsigned long>(strlen(namefunction)));
        }
        int key[nsiz];
        int data;
        char namefunction[MAXLENGHTFUNCTIONNAME];
    };

    /*
    * functions entries are stored in a sorted vector
    */
    typedef std::vector<entry> table_t;

    static  table_t table;

    /*
    * small functors used to search in the vector
    * (could be removed with boost:: or C++0x lambdas )
    *
    */
    struct key_less_than : std::unary_function<entry const&, bool>
    {
        key_less_than(int const* const k): key(k) {}
        bool operator()(entry const& e) const { return less_n<nsiz>(key, e.key);}
        int const* const key;
    };

    struct not_less_than_key : std::unary_function<entry const&, bool>
    {
        not_less_than_key(int const* const k): key(k) {}
        bool operator()(entry const& e) const { return !less_n<nsiz>(e.key, key);}
        int const* const key;
    };


    struct equal_data
    {
        equal_data(int const d) : data(d) {}
        bool operator()(entry const& e) const { return e.data == data ; }
        int const data;
    };
}
/*----------------------------------------------------------------------------*/
/*
* preallocate memory for  DEFAULT_ELEMENTFUNCTIONLIST entries in table
*/
/*----------------------------------------------------------------------------*/
BOOL create_hashtable_scilab_functions(void)
{
    try
    {
        table_t tmp(0);
        tmp.reserve( DEFAULT_ELEMENTFUNCTIONLIST );
        table.swap(tmp);
    }
    catch( std::bad_alloc& e)
    {
        e.what();
        return FALSE;
    }
    return TRUE;
}
/*----------------------------------------------------------------------------*/
/*
* free memory for table
*/
/*----------------------------------------------------------------------------*/
void destroy_hashtable_scilab_functions()
{
    table_t tmp(0); // swap trick needed because resize() does *not* release memory
    table.swap(tmp);
    unLockLoadFunctionsTab();
}
/*----------------------------------------------------------------------------*/
static BOOL doFindFunction(int *key,char *name, int *scilab_funptr)
{
    int tmp[nsiz];
    int const* keyToSearch= key;
    if (name)
    {
        /* faster than a strcmp */
        int job = 0; /* convert name to id */
        C2F(cvname)(tmp, name, &job, static_cast<unsigned long>(std::strlen(name)));
        keyToSearch = tmp;
    }
    // find lower bound : first elt not <
    table_t::const_iterator i= partition_point_n(table.begin(), table.size(), not_less_than_key(keyToSearch));
    if(i != table.end() && eq_n<nsiz>(keyToSearch, i->key))
    {
        *scilab_funptr = i->data;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*----------------------------------------------------------------------------*/
// linear search according to data field in entries
static BOOL doBackSearchFunction(int *key, int *scilab_funptr)
{
    table_t::const_iterator i= std::find_if(table.begin(), table.end()
        , equal_data(*scilab_funptr));
    if( i != table.end())
    {
        copy_n<nsiz>(i->key, key);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*----------------------------------------------------------------------------*/
static BOOL doEnterFunction(int *key,char *name, int *scilab_funptr)
{
    if(table.size() < MAXELEMENTFUNCTIONLIST)
    {
        entry tmp(*scilab_funptr, name);
        // insert before upper bound : first elt such that key<elt
        table.insert(partition_point_n(table.begin(), table.size()
            , key_less_than(tmp.key)), tmp);
        if(table.size() > hashtable_core_maxFilled)
        {
            hashtable_core_maxFilled = (unsigned int)table.size();
        }
        return TRUE;
    }
    return FALSE;
}
/*----------------------------------------------------------------------------*/
static BOOL doDeleteFunction(int *key, int *scilab_funptr)
{
    // search entry with given scilab_funptr starting from the lower bound
    // according to given key
    table_t::iterator i(std::find_if(partition_point_n(table.begin(), table.size()
        , not_less_than_key(key))
        , table.end(), equal_data(*scilab_funptr)));

    if( i != table.end() )
    {
        // entry found -> erase it
        table.erase(i);
        return TRUE;
    }
    else
    {
        // not found
        return FALSE;
    }
}
/*----------------------------------------------------------------------------*/
BOOL action_hashtable_scilab_functions(int *key,char *name, int *scilab_funptr
                                       , SCI_HFUNCTIONS_ACTION action)
{
    switch(action)
    {
    case SCI_HFUNCTIONS_FIND:
        return doFindFunction(key, name, scilab_funptr);

    case SCI_HFUNCTIONS_BACKSEARCH:
        return doBackSearchFunction(key, scilab_funptr);

    case SCI_HFUNCTIONS_ENTER:
        return doEnterFunction(key, name, scilab_funptr);

    case SCI_HFUNCTIONS_DELETE:
        return doDeleteFunction(key, scilab_funptr);
    }
    return FALSE;
}
/*----------------------------------------------------------------------------*/
// small functors, could be removed with boost or c++0X lambdas
struct has_namefunction : std::unary_function<entry const&, bool> {
    bool operator()(entry const& e) const { return e.namefunction[0] != '\0' ;} // <=> strlen(e.namefunction) > 0
};

struct copy_name : std::unary_function<entry const&, char**> {
    copy_name(char** n): names(n) { }
    char** operator()(entry const& e)
    {
        if(e.namefunction[0] != '\0')
        {
            *names = strdup(e.namefunction);
            ++names;
        }
        return names;
    }

    char** names;
};
/*----------------------------------------------------------------------------*/
char **GetFunctionsList(int *sizeList)
{
    *sizeList= (int)std::count_if(table.begin(), table.end(), has_namefunction());
    char **ListFunctions = static_cast<char**>(MALLOC(sizeof(char*)*(*sizeList)));
    if ( ListFunctions )
    {
        std::for_each(table.begin(), table.end(), copy_name(ListFunctions));
    }
    return ListFunctions;
}
/*--------------------------------------------------------------------------*/
struct equal_name : std::unary_function<entry const&, bool>
{
    equal_name(char const* const n): name(n){}
    bool operator()(entry const& e) const{ return std::strncmp(e.namefunction, name, MAXLENGHTFUNCTIONNAME) == 0 ;}
    char const* const name;
};
/*----------------------------------------------------------------------------*/
BOOL ExistFunction(char *name)
{
    return (std::find_if(table.begin(), table.end(), equal_name(name)) == table.end()) ? FALSE : TRUE ;
}
/*----------------------------------------------------------------------------*/
