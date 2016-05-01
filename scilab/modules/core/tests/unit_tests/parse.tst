// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

funcprot(0);

//syntax with no explicit lhs
1
-1
1+1
1==2
[1 2 3]
ans(1)
[sin(3) 2 3]
sin(3)
ans
ans==1
[
]

[ans
]

"xxxxx"
"xx=xxx"

disp xxx
//syntax with simple lhs
a=1
a=[1 2 3]
a=[1 2 3
4 5 6]
a(2)=44

clear c;c(5)=9

[m,k]=max(1:3)
[d1f d2f d3f d4f] = (1,2,3,4)

//if 10<>3 then x=disp('abcd'),end
if 10<>3 then disp("abcd"),end
//if 10<>3  x=disp('abcd'),end//matlab syntax
if 10<>3  disp("abcd"),end//matlab syntax


if (1==1) then "ok",end

for k=1:2,disp(k),end
function foo(k),disp(k),endfunction;
for k=1:2,foo(k),end

for k=1:2,(1+1),end

for k=1:2,[m,k]=max(1:k),end
for k=1:2,[1,2],end


for k=1:3 k,end //matlab syntax

//nouvelles syntaxes

[a(1),b(2)]=(3,4)

[a(1),x,b(2)]=(3,4,5)
[a(1),b(1,2),x]=(3,4,5)

I1=list(2,3);I2=list(1,2);
clear x y;[x,y(I2(:))]=(11,12)
clear x y;[x(I1(:)),y]=(11,12)
clear x y;[x(I1(:)),y(I2(:))]=(11,12)


str_l=list();str_l(1)=1;str_l(1)

l=list(1,2,3);l(3)=null()

function []=foo()
    [noeuds,triang]=resume(1,2)
endfunction
foo();disp([noeuds,triang])


function [a]=foo(x)
    a=33
    [noeuds,triang]=resume(45,sin(2))
endfunction
foo(%t)

function [a]=foo(x)
    a=33
    if x,  return,end
endfunction
foo(%t)

foo();disp([noeuds,triang])


deff("t1=foo()","t1(2)= 1");foo()

deff("t1=foo()","t1=list();t1(1)= 44;");foo()

deff("[a,b,x]=foo()","[a(1),b(1,2),x]=(3,4,5);");[a,b,x]=foo()



function [a,b,x]=foo(),u=5;[a(1),b(1,2),x]=(3,4,u),a(2)=44,endfunction
[a,b,x]=foo()

function [a,b,x]=foo(),[a,b,x]=(3,4,5),endfunction
[a,b,x]=foo()



function x=foo(),INDX=list(2,3); x(INDX(:))=11; endfunction
foo()

I1=list(2,3);I2=list(1,2);
function foo(),[x(I1(:)),y(I2(:))]=(11,12);disp(x,y), endfunction
foo()


function l=foo(),l=list(1,2,3);l(3)=null();endfunction
foo()

funcprot(1);

