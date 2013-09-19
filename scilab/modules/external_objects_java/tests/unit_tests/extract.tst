// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


jimport java.util.HashMap;
h = HashMap.new();
h.put("Coucou", "Foo");
h.put("Hello", "Bar");
a=rand(10,10);
h.put("MyMatrix", a);
dense=[%F, %F, %T, %F, %F
%T, %F, %F, %F, %F
%F, %F, %F, %F, %F
%F, %F, %F, %F, %T];
h.put("MyBool", dense);

assert_checkequal(h.get("Coucou"), "Foo");
assert_checkequal(h.Coucou, "Foo");
assert_checkequal(h("Coucou"),"Foo");

assert_checkequal(h.get("Hello"), "Bar");
assert_checkequal(h.Hello, "Bar");
assert_checkequal(h("Hello"),"Bar");

assert_checkequal(h.get("MyMatrix"), a);
assert_checkequal(h.MyMatrix, a);
assert_checkequal(h("MyMatrix"),a);

assert_checkequal(h.get("MyBool"), dense);
assert_checkequal(h.MyBool, dense);
assert_checkequal(h("MyBool"),dense);
