// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2008-2011 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//Checks syntactical aspects  related to functions
funcprot(0);
//
//               Various syntaxes
//               ----------------
function y=foo(x)
    y=x^2
endfunction
if foo(2)<>4  then pause,end
//
function y=foo(a,b)
    y=a+b
endfunction
if foo(2,3)<>5  then pause,end
//
function [y,z]=foo(a)
    y=a^2
    z=a^3
endfunction
[y,z]=foo(2);if y<>4|z<>8 then pause,end
//
function y=foo()
    y=2
endfunction
if foo()<>2  then pause,end
//
function y=foo
    y=2
endfunction
if foo()<>2  then pause,end
//
function []=foo
    y=resume(2)
endfunction
foo();if y<>2  then pause,end
//
function foo
    y=resume(3)
endfunction
foo();if y<>3  then pause,end
//
function result=foo(a,b)
    result = [(a+b) (a-b) (a*a + b*b)];
endfunction
result=foo(32,64); if or(result <> [96. -32. 5120.]) then pause, end

//
//               Various line splits
//               -------------------
function y=foo(x),y=x^2
endfunction
if foo(2)<>4  then pause,end
//
function y=foo(x),y=3*x,endfunction
if foo(2)<>6  then pause,end
//
function y=foo(x)
    z=x^2
    function y=foo1(x)
        y=x+1
    endfunction
    y=foo1(z)
endfunction
if foo(2)<>5  then pause,end
//
//               Combined with deff
//               -------------------
function y=foo(x)
    z=x^2
    deff("y=foo1(x)","y=x+1")
    y=foo1(z)
endfunction
if foo(2)<>5  then pause,end
//
deff("y=foo(x)",[
"z=x^2"
"function y=foo1(x)"
"y=x+1"
"endfunction"
"y=foo1(z)"])
if foo(2)<>5  then pause,end

function y=foo(x),y=x+1
endfunction;if foo(2)<>3  then pause,end
//
//               Combined with exec
//               -------------------
function foo
    z=9;
    function y=foo1(x)
        y=x+1;
    endfunction
    y=foo1(z);
endfunction
exec(foo); if y<>10  then pause,end
//

//               Combined with control instructions
//               ----------------------------------
if %t then
    function  y=foo(x)
        y=sin(x)
    endfunction
else
    function  y=foo(x)
        y=1
    endfunction
end
if foo(1)<>sin(1)  then pause,end
//
if %t then
    function  y=foo(x)
        if x==0 then
            y=1
        else
            y=sin(x)/x
        end
    endfunction
else
    function  y=foo(x)
        y=1
    endfunction
end
if foo(0)<>1   then pause,end
if foo(2)<>sin(2)/2   then pause,end
//
z=0;
for k=1:2
    function y=foo()
        y=k
    endfunction
    z=z+foo();
end
if z<>3 then pause,end

//
z=0;for k=1:2
    function y=foo(),y=k,endfunction
    z=z+foo();
end
if z<>3 then pause,end
//
z=0;for k=1:2, function y=foo(),y=k,endfunction
    z=z+foo();
end
if z<>3 then pause,end


z=0;
for k=1:2
    function y=foo(k)//qsdsdf
        y=k^2
    endfunction
    z=z+foo();
end
if z<>5 then pause,end

z=0;
for k=1:2
    function y=foo(k)//qsdsdf
        y=k^2 //a comment
    endfunction
    z=z+foo();
end
if z<>5 then pause,end

z=0;
for k=1:2
    function y=foo(k), y=k^2, endfunction
    z=z+foo();
end
if z<>5 then pause,end


z=0;
for k=1:2
    function y=foo(k), y=k^2, endfunction// a comment
    z=z+foo();
end
if z<>5 then pause,end

//bug 1024 non regression test
O=[];
for n= 1:10;
    Fx=rand(1,100);
    Fy=1:100;
    function [f,g,ind]=cout(x,ind)
        f1=(1-exp(-x*Fx) - Fy);
        f= (1/2)*f1*f1';
        g= x*(Fx.*exp(-x*Fx))*f1';
    endfunction
    [fopt,lmopt]=optim(cout,0.4);
    O=[O fopt];
end
if size(O,"*")<>10 then pause,end

// test line count in compiled macros
//
function a=foo(),a=1,endfunction
L=macr2tree(foo);
if L(6)<>1 then pause,end

clear foo
function a=foo()
    a=1,
endfunction
L=macr2tree(foo);
if L(6)<>3 then pause,end

clear foo
function a=foo()//xxcxcx
    a=1,
endfunction
L=macr2tree(foo);
if L(6)<>3 then pause,end

clear foo
function a=foo(),
    a=1,
endfunction
L=macr2tree(foo);
if L(6)<>3 then pause,end

clear foo
function a=foo(),a=1//xxcxcx
    b=2
endfunction
L=macr2tree(foo);
if L(6)<>3 then pause,end


clear foo
function a=foo()
    a=...
    sin...
    (...
    1...
    ),
endfunction
//continuation lines are replaced by a sequence of empty lines followed by the logical line
//so the function above is  the same as
function a=foo1()




    a=sin(1),
endfunction

L=macr2tree(foo);
t=L(6)<>7;
if foo<>foo1 then pause,end


clear foo foo1
function a=foo(),a=...
    sin...
    (...
    1 ...
    ),
endfunction
//continuation lines are replaced by a sequence of empty lines followed by the logical line
//foo should be equal to foo1
function a=foo1(),a=sin(1),
endfunction

L=macr2tree(foo);
if L(6)<>6  then pause,end
if foo<>foo1 then pause,end



