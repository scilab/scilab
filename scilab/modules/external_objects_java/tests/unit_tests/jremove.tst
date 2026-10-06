// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// =============================================================================

jimport java.lang.Runtime
jimport java.lang.String
runtime = Runtime.getRuntime();

// construct a big java.lang.String object
LENGTH =  2**18;
function str = alloc()
    str = jnewInstance(String, int8(grand(1,LENGTH, "unf", 32, 124)));
endfunction

function F()
    jimport java.lang.String
    str = alloc()

    jremove(str)
    jremove(String)
endfunction

function G()
    jimport java.lang.String
    str = alloc()

    //  let the scope cleanup do the job
    //    jremove(str)
    //    jremove(String)
endfunction

// Heap used after each release, displayed by tests/benchmarks/jremove.tst
color=1;
N=100;
for i=1:N
    F();
    times(color,i) = (runtime.totalMemory() - runtime.freeMemory());
end
color = color+1;
for i=1:N
    G();
    times(color,i) = (runtime.totalMemory() - runtime.freeMemory());
end
color = color+1;
for i=1:N
    execstr("str"+string(i)+" = alloc();");
    clear("str"+string(i));
    times(color,i) = (runtime.totalMemory() - runtime.freeMemory());
end
color = color+1;
for i=1:N
    execstr("str"+string(i)+" = alloc();");
    jremove("str"+string(i));
    clear("str"+string(i));
    times(color,i) = (runtime.totalMemory() - runtime.freeMemory());
end
color = color+1;
for i=1:N
    l = list(alloc());
    clear("l");
    times(color,i) = (runtime.totalMemory() - runtime.freeMemory());
end
