/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>
#include <assert.h>
#include "export_symbol.h"
#include "symbol.hxx"

#define __MAX_CLONE_NUMBER__	2

using symbol::Symbol;
using symbol::symbols_t;

static int symbol_test()
{

    Symbol foo(L"foo");
    Symbol foo2(L"foo");
    Symbol foo3(L"Foo");
    Symbol *foo4 = new Symbol(L"Foo");
    Symbol bar(L"bar");
    Symbol scicos(L"scicos");
    symbols_t clone;
    int i = 0;
    symbols_t::iterator it;

    for (i = 0 ; i < __MAX_CLONE_NUMBER__ ; ++i)
    {
        clone.push_front(new Symbol(L"clone"));
    }
    for (it = clone.begin() ; it != clone.end() ; ++it)
    {
        assert((*it)->name_get() == L"clone");
    }

    assert(foo.name_get() == L"foo");
    assert(foo2.name_get() == L"foo");
    assert(foo3.name_get() == L"Foo");
    assert(foo4->name_get() == L"Foo");
    assert(bar.name_get() == L"bar");
    assert(scicos.name_get() == L"scicos");

    assert(foo == foo2);
    assert(foo != foo3);
    assert(*foo4 == foo3);

    /* foo, Foo, bar, scicos, clone */
    assert(Symbol::map_size() == 5);


    std::wcout << L"Symbol Overview :" << std::endl;
    std::wcout << L"-----------------" <<  std::endl;
    std::wcout << L"Size : " << Symbol::map_size() << std::endl;
    std::wcout << L"-----------------" <<  std::endl;
    std::wcout << foo << std::endl;
    std::wcout << foo2 << std::endl;
    std::wcout << foo3 << std::endl;
    std::wcout << *foo4 << std::endl;
    std::wcout << bar << std::endl;
    std::wcout << scicos << std::endl;
    std::wcout << **(clone.begin()) << std::endl;
    std::wcout << L"----------------" << std::endl;
    return 0;
}

int test()
{
    Symbol foo(L"foo");
    Symbol bar(L"foo");
    std::wcout << &foo << std::endl;
    std::wcout << &bar << std::endl;
}

int main(void)
{
    symbol_test();
    //    test();
    return 0;
}
