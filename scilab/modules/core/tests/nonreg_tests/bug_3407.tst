//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2009 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 3407 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3407
//
// <-- Short Description -->
//Invalid line numbering in compiled functions

fprot=funcprot();
funcprot(0);

function l=myfun()
    a=1
    a=1
    a=[1;..
    1;..
    1;..
    1;..
    1;..
    1;..
    2]
    a=1
    a=1
    [l,w]=where()
endfunction
l=myfun();if l(1)<>13 then pause,end
//If

function l=myfun2(),if  %t then [l,w]=where(),end,endfunction
l=myfun2();if l(1)<>1 then pause,end

function l=myfun2()
    if  %t then [l,w]=where(),end
endfunction
l=myfun2();if l(1)<>2 then pause,end

function l=myfun2()
    if  %t then
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>3 then pause,end

function l=myfun2()
    if  [%t
        %t] then
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>4 then pause,end

function l=myfun2()
    if  [%t,..
        %t] then
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>4 then pause,end


function l=myfun2()
    if  %f then
        l=0,
    else
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>5 then pause,end

function l=myfun2()
    if  %f then
        l=0,
    else
        l=0
    end
    [l,w]=where()
endfunction
l=myfun2();if l(1)<>7 then pause,end

//for
function l=myfun2(),for k=1, [l,w]=where(),end,endfunction
l=myfun2();if l(1)<>1 then pause,end

function l=myfun2(),
    for k=1, [l,w]=where(),end,
endfunction
l=myfun2();if l(1)<>2 then pause,end

function l=myfun2(),
    for k=1,..
        [l,w]=where(),
    end,
endfunction
l=myfun2();if l(1)<>3 then pause,end

function l=myfun2(),
    for k=1,
        [l,w]=where(),
    end,
endfunction
l=myfun2();if l(1)<>3 then pause,end

function l=myfun2(),
    for k=1,
        l=0
    end,
    [l,w]=where(),
endfunction
l=myfun2();if l(1)<>5 then pause,end

function l=myfun2(),
    for k=1,
        l=0
    end,
    x=[1;
    2];
    [l,w]=where(),
endfunction
l=myfun2();if l(1)<>7 then pause,end

function l=myfun2(),
    for k=1,
        l=0
    end,
    x=[1,..
    2];
    [l,w]=where(),
endfunction
l=myfun2();if l(1)<>7 then pause,end

//while
function l=myfun2(),while  %t then [l,w]=where(),break,end,endfunction
l=myfun2();if l(1)<>1 then pause,end

function l=myfun2()
    while  %t then [l,w]=where(),break,end
endfunction
l=myfun2();if l(1)<>2 then pause,end

function l=myfun2()
    while  %t then
        [l,w]=where(),break
    end
endfunction
l=myfun2();if l(1)<>3 then pause,end


function l=myfun2()
    while  %f then
        l=0,
    end
    [l,w]=where()
endfunction
l=myfun2();if l(1)<>5 then pause,end

//Select
function l=myfun2(),select 1,case 1 then [l,w]=where(),end,endfunction
l=myfun2();if l(1)<>1 then pause,end

function l=myfun2()
    select 1,case 1 then [l,w]=where(),end
endfunction
l=myfun2();if l(1)<>2 then pause,end

function l=myfun2()
    select 1,case 1
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>3 then pause,end

function l=myfun2()
    select 1,..
    case 1
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>4 then pause,end

function l=myfun2()
    select 1,
    case 1
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>4 then pause,end

function l=myfun2()
    select 1,
    case 0
        l=0
    case 1
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>6 then pause,end

function l=myfun2()
    select 1,
    case 0
        l=0
    case 2
        l=3
    else
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>8 then pause,end

function l=myfun2()
    select 1,
    case 0
        l=0
    case 2
        l=3
    else
        l=-1
    end
    [l,w]=where()
endfunction
l=myfun2();if l(1)<>10 then pause,end





//try
function l=myfun2(),try, [l,w]=where(),end,endfunction
l=myfun2();if l(1)<>1 then pause,end

function l=myfun2()
    try [l,w]=where(),end
endfunction
l=myfun2();if l(1)<>2 then pause,end

function l=myfun2()
    try
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>3 then pause,end

function l=myfun2()
    try ..
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>3 then pause,end


function l=myfun2()
    try
        error(999)
    catch
        [l,w]=where()
    end
endfunction
l=myfun2();if l(1)<>5 then pause,end

function l=myfun2()
    try
        error(999)
    catch
        l=0
    end
    [l,w]=where()
endfunction
l=myfun2();if l(1)<>7 then pause,end

function l=myfun2()
    try
        l=0
    catch
        l=0
    end
    [l,w]=where()
endfunction
l=myfun2();if l(1)<>7 then pause,end

funcprot(fprot);
