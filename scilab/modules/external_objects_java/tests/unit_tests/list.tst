// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

// Convert a Scilab array into a java.util.List (Collection parameter)
jimport("java.util.ArrayList");
// Array of strings (non-primitive type)
as = ArrayList.new(jvoid);
as.addAll(["hello" "world" "scilab"]);
assert_checkequal(as.size(), int32(3));
assert_checkequal(as.get(0), "hello");
assert_checkequal(as.get(2), "scilab");
// Array of numbers (goes through fromPrimitive)
an = ArrayList.new(jvoid);
an.addAll([10 20 30 40]);
assert_checkequal(an.size(), int32(4));
assert_checkequal(an.get(0), 10);
assert_checkequal(an.get(3), 40);