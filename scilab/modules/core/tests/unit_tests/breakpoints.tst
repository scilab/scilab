// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

mode(-1)
//
// (setbpt delbpt).
deff('[]=lig()',[
'shlig(2),';
'if 1>2 then,';
'  shlig(4),';
',';
'  shlig(6)     ,'; // break-point
',';
'end,shlig(8)   ,'; // break-point
'shlig(9),';
'for k=1:3,';
'  k;shlig(11)  ,'; // break-point 
'end,shlig(12)  ,'; // break-point
',';
'for l=1:10,';
'  if l==5 then break,end,';
'  l,';
'end,shlig(17)  ,'; // break-point
'shlig(18),';
'select 2,';
'case 1 then,';
',';
'case 2 then,';
'  shlig(23)   ,'; // break-point
'case 3 then,';
'1,';
'else,';
'2,';
'end,shlig(28) ,'; // break-point
'shlig(29),';
'select 0,';
'case 1 then,';
'1,';
'case 2 then,';
'2,';
'case 3 then,';
'3,';
'else,';
'  shlig(38)  ,'; // break-point
'end,shlig(39),'; // break-point
'shlig(40)    ,'; // break-point
'i=0,';
'while i<3,';
'i=i+1,shlig(43) ,'; // break-point 
'end,shlig(44) ,'; //break-point
'i=0;,';
'while i<10,';
'i=i+1,if i==2 then break,end,shlig(47),';
'end,shlig(48) ,' //break-point
],'n')
deff('[]=shlig(n)','write(%io(2),''ligne ''+string(n))')

//definition des points d'arrets
setbpt('lig',6)
setbpt('lig',8)
setbpt('lig',11)
setbpt('lig',12)
setbpt('lig',17)
setbpt('lig',23)
setbpt('lig',28)
setbpt('lig',38)
setbpt('lig',39)
setbpt('lig',40)
setbpt('lig',43)
setbpt('lig',44)
setbpt('lig',48)
write(%io(2),[
'user should check the line number'])
write(%io(2),[' ';'MACRO NOT COMPILED';'-----------------';' '])
lig()
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume

comp(lig)
write(%io(2),[' ';'COMPILED MACRO';'-----------------';' '])
lig()
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
delbpt('lig')
clear lig shlig

// Elementary setbpt tests with scalar line number argument
// Order of the commands is important to guarantee
// the full coverage of the tests
delbpt(),dispbpt()   // should display no output
setbpt("foo1",0)    // should spit error 9997
setbpt("foo1",-3)   // should spit error 9997
setbpt("foo1",3.2)  // should spit error 9997
setbpt("foo1"),dispbpt()   // set at line 1 (default)
setbpt("foo1",5),dispbpt()  // add another bpt to a macro that already has bpts, and that is the last one
setbpt("foo2",5),setbpt("foo2",8),dispbpt()  // add bpts to a new macro (will always be placed at the end)
setbpt("foo1",9),dispbpt()  // add another bpt to a macro that already has bpts, and that is NOT the last one
setbpt("foo1",5),dispbpt()  // check duplicates removal - line 5 should appear only once in foo1
setbpt("foo2",5),dispbpt()  // check duplicates removal - line 5 should appear only once in foo2

// More complicated setbpt tests with vector line number argument
// Order of the commands is important to guarantee
// the full coverage of the tests
delbpt(),dispbpt()   // should display no output
setbpt("foo1",[1,0,5])    // should spit error 9997
setbpt("foo1",[-3,2,7])   // should spit error 9997
setbpt("foo1",[1,3.2])  // should spit error 9997
setbpt("foo1",[8,9]),dispbpt()    // add 2 bpts at once to a new macro
setbpt("foo1",5),setbpt("foo1",[2,3]),dispbpt()  // add further bpts to a macro that already has bpts, and that is the last one
setbpt("foo2",[5,8]),setbpt("foo2",7),dispbpt()  // add bpts to a new macro (will always be placed at the end)
setbpt("foo1",[10,15,20]),dispbpt()  // add bpts to a macro that already has bpts, and that is NOT the last one
setbpt("foo1",[50,50,50,60,50,80,60]),dispbpt()  // check duplicates removal in vector input, not the last macro - lines 50 and 60 should appear only once in foo1
setbpt("foo1",[9,5,8,100,3,2]),dispbpt()  // check duplicates removal wrt existing bpts, not the last macro - all lines should appear only once in foo1
setbpt("foo2",[50,50,50,60,50,80,60]),dispbpt()  // check duplicates removal in vector input, last macro - lines 50 and 60 should appear only once in foo1
setbpt("foo2",[9,5,8,100,7,2]),dispbpt()  // check duplicates removal wrt existing bpts, last macro - all lines should appear only once in foo2


// Elementary delbpt tests with scalar line number argument
// Order of the commands is important to guarantee
// the full coverage of the tests
delbpt(),dispbpt()   // should display no output
delbpt("foo1",0)    // should spit error 9997
delbpt("foo1",-3)   // should spit error 9997
delbpt("foo1",3.2)  // should spit error 9997
delbpt("foo1"),dispbpt()   // should display no output
setbpt("foo1",[5,8,11]),delbpt("foo1"),dispbpt()   // should display no output
setbpt("foo1",[5,8,11]),setbpt("foo2",[55,88,111]),delbpt("foo1"),dispbpt()   // should display the 3 bpts for foo2 only
setbpt("foo1",[5,8,11]),setbpt("foo2",[55,88,111]),delbpt("foo2"),dispbpt()   // should display the 3 bpts for foo1 only
delbpt()
setbpt("foo1",5),delbpt("foo1",5),dispbpt()   // should display no output
setbpt("foo1",[5,6,8]),delbpt("foo1",5),dispbpt()    // lines 6 and 8 should appear only
setbpt("foo1",[5,10,11]),delbpt("foo1",10),dispbpt()    // lines 5, 6, 8 and 11 should appear only
delbpt("foo1",11),dispbpt()    // lines 5, 6 and 8 should appear only
setbpt("foo2",[20,21,22,23]),delbpt("foo1",6),dispbpt()    // lines 5 and 8 for foo1, and 20 to 23 for foo2
delbpt("foo2",21),dispbpt()    // lines 5, and 8 for foo1, and 20, 22, 23 for foo2

// More complicated delbpt tests with vector line number argument
// Order of the commands is important to guarantee
// the full coverage of the tests
delbpt(),dispbpt()   // should display no output
delbpt("foo1",[1,0,5])    // should spit error 9997
delbpt("foo1",[-3,2,7])   // should spit error 9997
delbpt("foo1",[1,3.2])  // should spit error 9997
delbpt("foo1",[8,9]),dispbpt()   // should display no output
setbpt("foo1",[5,6,8,9]),delbpt("foo1",[8,5]),dispbpt()    // lines 6 and 9 should appear only
setbpt("foo1",[5,6,8,9]),delbpt("foo1",[8,5,8,8,6,5]),dispbpt()    // check duplicates removal in vector input - line 9 should appear only
setbpt("foo1",[5,6,8,9]),setbpt("foo2",[10,12,14,16,18]),delbpt("foo1",[8,5,8,10]),dispbpt()    // not the last macro, check duplicates removal in vector input - lines 6 and 9 should appear for foo1, plus all the 5 bpts for foo2
setbpt("foo1",[5,6,8,9]),setbpt("foo2",[10,12,14,16,18]),delbpt("foo1",[8,5,6,9]),dispbpt()    // remove all, not the last macro - should display no bpt for foo1, but all the 5 bpts for foo2
setbpt("foo1",[5,6,8,9]),setbpt("foo2",[10,12,14,16,18]),delbpt("foo2",[14,18,10,20]),dispbpt()    // last macro - should display all the 4 bpts for foo1, but only lines 12 and 16 for foo2
setbpt("foo1",[5,6,8,9]),setbpt("foo2",[10,12,14,16,18]),delbpt("foo2",[14,18,10,12,16]),dispbpt()    // remove all, last macro - should display all the 4 bpts for foo1, but no bpt for foo2

// first non regression test for bug 1894
delbpt()
function foo1()
a=1;
a=1;
a=1;
a=1;
a=1;
endfunction
function foo2()
a=2;
a=2;
a=2;
a=2;
a=2;
endfunction
setbpt("foo2",1)
foo2()
delbpt("foo2",1)
dispbpt
setbpt("foo1",[1:5]);setbpt("foo2",[1:5]);
dispbpt
// the following resume(0) must spit:
// Stop after row     2 in function foo2 :
// and not Stop after row     2 in function foo1 :
resume(0)
abort

// second non regression test for bug 1894
delbpt()
function foo1
endfunction
function foo2
  function d=foo3()
    disp("  Entering");
    d=1
    disp("  Leaving");
  endfunction
  disp("Before");
  b=foo3();
  disp("After");
endfunction
setbpt("foo2",[1:10]);
foo2()
delbpt("foo2",[1:10])
setbpt("foo1",[1:2]);setbpt("foo2",[1:10]);setbpt("foo3",[1:5]);
// the following resume must spit:
// Stop after row     6 in function foo2 :
// and not Stop after row     1 in function foo3 :
resume


// non regression test for bug 2474
for i=1;delbpt();end  // should not say:
//                  !--error 115
//Stack problem detected within a loop
// a primitive function has been called with wrong number
// of lhs arguments. No lhs test made for this function;
// please report this bug
//                  !

