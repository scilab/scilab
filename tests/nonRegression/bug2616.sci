// Scilab Project - Serge Steer
// Copyright INRIA 2007
// Date : June 2007

mode(-1) ;
clear ;
BugNumber=2616;
T=[];
//simple case uncompiled 
s=0;for k=1:3,s=s+1;end
T=[T,s<>3];


s=0;for k=(1:3)+1,s=s+1;end
T=[T,s<>3];

s=0;for k=[(1:3) 1],s=s+1;end
T=[T,s<>4];

function s=foo(),s=0;for k=1:3,s=s+1;end,endfunction
T=[T,foo()<>3];

function s=foo(),s=0;for k=(1:3)+1,s=s+1;end,endfunction
T=[T,foo()<>3];

function s=foo(),s=0;for k=[(1:3) 1],s=s+1;end,endfunction
T=[T,foo()<>4];

//check if implicit  vector is created or not
old=stacksize();
stacksize(180000)

//should evaluate 
ierr=execstr('s=0;for k=1:200000,s=s+1;end','errcatch')
T=[T,ierr<>0|s<>200000];

//should produce a stacksize error
ierr=execstr('s=0;for k=(1:200000)+1,s=s+1;end','errcatch')
T=[T,ierr<>17];

ierr=execstr('s=0;for k=[(1:200000) 1],s=s+1;end','errcatch')
T=[T,ierr<>17];

function s=foo(),s=0;for k=1:200000,s=s+1;end,endfunction
ierr=execstr('s=foo();','errcatch');
T=[T,ierr<>0|s<>200000];


function s=foo(),s=0;for k=1+(1:200000),s=s+1;end,endfunction
ierr=execstr('s=foo();','errcatch');
T=[T,ierr<>17];
stacksize(old(1))

affich_result(and(~T),BugNumber) ;
clear ;

