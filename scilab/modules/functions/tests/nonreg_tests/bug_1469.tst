// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1469 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1469
//
// <-- Short Description -->
//    Comments modify line numbering in functions
//
//    What you don't believe?
//
//    Try this:
//
//    1st test, no comment in line 1:
//
//    -->function foo1()
//    -->  function foo3
//    -->  endfunction
//    -->  disp("line a");
//    -->  disp("line b");
//    -->  disp("line c");
//    -->endfunction
//
//    -->setbpt("foo1",[3,4])
//
//    -->foo1()
//    Stop after row     3 in function foo1 :
// ...

//test of line numbering in  functions, handling initial comments

function foo,endfunction
L=macr2lst(foo);
R1=L(4)(1)=='15'&size(L)==6
clear foo;

function foo,,endfunction
L=macr2lst(foo);
R2=L(4)(1)=='15'&size(L)==6
clear foo;

function foo(),endfunction
L=macr2lst(foo);
R3=L(4)(1)=='15'&size(L)==6
clear foo;

function foo(),a=1,endfunction
L=macr2lst(foo);
R4=L(4)(1)=='6'&size(L)==8
clear foo;

function foo
endfunction
L=macr2lst(foo);
R5=L(4)(1)=='15'&size(L)==6
clear foo;


function foo,
endfunction
L=macr2lst(foo);
R6=L(4)(1)=='15'&size(L)==6
clear foo;


function foo//ZZZZ
endfunction
L=macr2lst(foo);
R7=L(4)(1)=='31'&L(5)(1)=='15'&size(L)==7
clear foo;


function foo,//ZZZZ
endfunction
L=macr2lst(foo);
R8=L(4)(1)=='31'&L(5)(1)=='15'&size(L)==7
clear foo;


function foo
//ZZZZ
endfunction
L=macr2lst(foo);
R9=L(4)(1)=='15'&L(5)(1)=='31'&size(L)==8
clear foo;


function foo,//ZZZZ
  a=1
endfunction
L=macr2lst(foo);
R10=L(4)(1)=='31'&L(5)(1)=='15'&L(6)(1)=='6'&size(L)==10
clear foo;


function loc=foo,[l,w]=where(),loc=l(1),endfunction
R11= and(foo()==1)
clear foo;


function loc=foo,//ZZZZ
  [l,w]=where(),loc=l(1)
endfunction
R12= and(foo()==2)
clear foo;

function loc=foo
//ZZZZ
  [l,w]=where(),loc=l(1)
endfunction
R13= and(foo()==3)
clear foo;


function loc=foo1()
  [l,w]=where(),loc=l(1)
  function loc=foo3(loc)
    [l,w]=where(),loc=[loc,l(1:2)']
  endfunction
  [l,w]=where(),loc=[loc,l(1)]
  loc=foo3(loc)
endfunction
R14= and(foo1()==[2,6,2,7])
clear foo1;

function loc=foo1()//ZZZZZZZ
  [l,w]=where(),loc=l(1)
  function loc=foo3(loc)
    [l,w]=where(),loc=[loc,l(1:2)']
  endfunction
  [l,w]=where(),loc=[loc,l(1)]
  loc=foo3(loc)
endfunction
L=macr2lst(foo1);
R15=and(foo1()==[2,6,2,7])&(L(4)(1)=='31')&(L(5)=='15')
clear foo1;


function loc=foo1()
  //ZZZZZZZ
  [l,w]=where(),loc=l(1)
  function loc=foo3(loc)
    [l,w]=where(),loc=[loc,l(1:2)']
  endfunction
  [l,w]=where(),loc=[loc,l(1)]
  loc=foo3(loc)
endfunction
L=macr2lst(foo1);
R16=and(foo1()==[3,7,2,8])&(L(4)=='15')&(L(5)(1)=='31')
clear foo1;


//pb execution des macros non compilee
deff('y=foo(a,b,c)','y=a+b+c','n')
R17=foo('aaa','bbb','cc')=='aaabbbcc'
clear foo

deff('y=foo(a,b,c)',['','y=a+b+c'],'n')
R18=foo('aaa','bbb','cc')=='aaabbbcc'

comp(foo)
R19=foo('aaa','bbb','cc')=='aaabbbcc'
clear foo

//function definition nested in a or loop!
z=0;R20=%t
for k=1:2
 function y=foo()
   y=k
 endfunction
 L=macr2lst(foo);
 R20=R20&(L(4)=='15')&(L(5)(1)=='2');
 z=z+foo();
end

R20=R20&z==3



//test avec getf
t=['function foo,//ZZZZ'
'  a=1'
'endfunction'];
mputl(t,TMPDIR+'/foo.sci');
getf(TMPDIR+'/foo.sci')
L=macr2lst(foo); //getf ignores declaration line comments
R21=L(4)(1)=='15'&L(5)(1)=='6'&size(L)==9
clear foo;


t=['function foo'
'//ZZZZ'
'  a=1'
'endfunction'];
mputl(t,TMPDIR+'/foo.sci');
getf(TMPDIR+'/foo.sci')
L=macr2lst(foo); //getf ignores declaration line comments
R22=L(4)(1)=='15'&L(5)(1)=='31'&L(6)(1)=='15'&size(L)==11
clear foo;

R=[R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15,R16,R17,R18,R19,R20,R21,R22]

if or(~R) then pause,end
