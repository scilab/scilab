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
#include "symbol.hxx"

#define __MAX_CLONE_NUMBER__	1000000

using symbol::Symbol;
using symbol::symbols_t;

static int symbol_test()
{

  Symbol foo("foo");
  Symbol foo2("foo");
  Symbol foo3("Foo");
  Symbol *foo4 = new Symbol("Foo");
  Symbol bar("bar");
  Symbol scicos("scicos");
  symbols_t clone;
  int i = 0;
  symbols_t::iterator it;

  for (i = 0 ; i < __MAX_CLONE_NUMBER__ ; ++i)
    {
      clone.push_front(new Symbol("clone"));
    }
  for (it = clone.begin() ; it != clone.end() ; ++it)
    {
      assert((*it)->name_get() == "clone");
    }

  assert(foo.name_get() == "foo");
  assert(foo2.name_get() == "foo");
  assert(foo3.name_get() == "Foo");
  assert(foo4->name_get() == "Foo");
  assert(bar.name_get() == "bar");
  assert(scicos.name_get() == "scicos");

  assert(foo == foo2);
  assert(foo != foo3);
  assert(*foo4 == foo3);

  /* foo, Foo, bar, scicos, clone */
  assert(Symbol::map_size() == 5);

  std::cout << "Symbol Overview :" << std::endl;
  std::cout << "-----------------" <<  std::endl;
  std::cout << "Size : " << Symbol::map_size() << std::endl;
  std::cout << "-----------------" <<  std::endl;
  std::cout << foo << std::endl;
  std::cout << foo2 << std::endl;
  std::cout << foo3 << std::endl;
  std::cout << *foo4 << std::endl;
  std::cout << bar << std::endl;
  std::cout << scicos << std::endl;
  std::cout << **(clone.begin()) << std::endl;
  std::cout << "----------------" << std::endl;
  return 0;
}

int main(void) {
  symbol_test();
  return 0;
}
