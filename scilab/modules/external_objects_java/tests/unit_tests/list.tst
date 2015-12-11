// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

jimport("java.util.LinkedList");

l = LinkedList.new(jvoid);
l.add("hello");
l.add(123);
l.add(%t);
ll = LinkedList.new(jvoid);
ll.add("world")
l.add(ll)

assert_checkequal(l(1), "hello");
assert_checkequal(l(2), 123);
assert_checkequal(l(3), %t);
assert_checkequal(l(4, 1), "world");

l(4, 1) = "scilab";

assert_checkequal(l(4, 1), "scilab");