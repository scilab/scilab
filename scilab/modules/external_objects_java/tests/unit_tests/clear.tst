// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
// <-- NO CHECK REF -->
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================






jimport java.lang.System

klass = jcompile("Test", ["public class Test {"
"static int counter = 0;"
""
"public Test() {"
"    counter++;"
"}"
""
"protected void finalize() throws Throwable {"
"    counter--;"
"}"
""
"public static double getClassCounter() {";
"    return counter;";
"}"
""
"public double getCounter() {";
"    return counter;";
"}"
""
"}"]);

//
// Allocate an object and manipulate it
//

a = klass.new();
b = a;

assert_checktrue(a.equals(b));
assert_checkequal(a.counter, b.counter);

counter = b.counter

clear b
System.gc();
sleep(1,"s"); // to let the GC do the job

assert_checkequal(a.counter, counter);
clear a

//
// allocate some objects and clear all of them through Scilab mechanism
//

l = list();
for i=1:100
    l($+1) = klass.new();
end

System.gc();
sleep(1,"s"); // to let the GC do the job

allocated = klass.new();
assert_checkequal(allocated.counter, 100+1);

clear l
System.gc();
sleep(1,"s"); // to let the GC do the job

assert_checkequal(allocated.counter, 1);

clear allocated
System.gc();
sleep(1,"s"); // to let the GC do the job

assert_checkequal(klass.getClassCounter(), 0);

