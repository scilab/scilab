// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//for version after 3.0 the comments are no more handled by a preprocessor,
//but by the parser itself

a=1//test
if a<>1 then pause,end
a=2;//test
if a<>2 then pause,end
a="1//test";
if a<>"1/"+"/test" then pause,end
a=33;//'sqdf'qsdf'
if a<>33 then pause,end

//in matrix concatenation

a=[//test
1 //test1
2 //test2
];//endtest
if or(a<>[1;2])  then pause,end
a=[//test
];
if a<>[]  then pause,end
a=[//test
//test1
//test2
];
if a<>[]  then pause,end

//in multi-line control instruction

if %t //test
    a=3; //test1
else //test2 end
    a=4//test3 end
end//endtest
if a<>3 then pause,end

if %f //test
    a=3; //test1
else //test2 end
    a=4//test3 end
end//endtest
if a<>4 then pause,end

a=0;
for k=1:10 //loop begin
    if a> 10 then //
        break // go out the loop
    else //111111
        a=a-1; //sdfdfd
    end //xxxxx
    a=a+k; //hhhhh
end //loop end
if a<>15 then pause,end

for i=1
    a=1;
end //


a=0;k=0
while %t //loop begin
    k=k+1; //ttttt
    if a> 10 then //
        break // go out the loop
    else //111111
        a=a-1; //sdfdfd
    end //xxxxx
    a=a+k; //hhhhh
end //loop end
if a<>15 then pause,end

c=1;
select c //test
case 1 then//test then
    a=8; //test8
case 2 then //test end
    a=-1 //test minus 1
else //else
    a=0  //test zeor
end //end
if a<>8 then pause,end

c=2;
select c //test
case 1 then//test then
    a=8; //test8
case 2 then //test end
    a=-1 //test minus 1
else //else
    a=0  //test zeor
end //end
if a<>-1 then pause,end

// In execstr
execstr("a=1//test")
if a<>1 then pause,end
execstr("a=2;//test")
if a<>2 then pause,end
execstr("a=''1//test'';")
if a<>"1/"+"/test" then pause,end
execstr("a=33;//''sqdf''qsdf''")
if a<>33 then pause,end

// In execstr with matrix concatenation

t=["a=[//test"
"1 //test1"
"2 //test2"
"];//endtest"];
execstr(t)
if or(a<>[1;2])  then pause,end
t=["a=[//test"
"];"];
execstr(t)

if a<>[]  then pause,end
t=["a=[//test"
"//test1"
"//test2"
"];"];
execstr(t)

if a<>[]  then pause,end

// In execstr with control instructions

t=["if %t //test"
"  a=3; //test1"
"else //test2 end"
"  a=4//test3 end"
"end//endtest"];execstr(t)
if a<>3 then pause,end

t=["if %f //test"
" a=3; //test1"
"else //test2 end"
" a=4//test3 end"
"end//endtest"];execstr(t)
if a<>4 then pause,end

// in deff (without compilation)
funcprot(0);
deff("a=foo","a=1//test");a=foo();
if a<>1 then pause,end
deff("a=foo","a=''1//test'';");a=foo();
if a<>"1/"+"/test" then pause,end
deff("a=foo","a=33;//''sqdf''qsdf''");a=foo();
if a<>33 then pause,end

t=["if %t //test"
"  a=3; //test1"
"else //test2 end"
"  a=4//test3 end"
"end//endtest"];deff("a=foo",t);a=foo();
if a<>3 then pause,end

// in deff (with compilation)
deff("a=foo","a=1//test");a=foo();
if a<>1 then pause,end
deff("a=foo","a=''1//test'';");a=foo();
if a<>"1/"+"/test" then pause,end
deff("a=foo","a=33;//''sqdf''qsdf''");a=foo();
if a<>33 then pause,end

t=["if %t //test"
"  a=3; //test1"
"else //test2 end"
"  a=4//test3 end"
"end//endtest"];deff("a=foo",t);a=foo();
if a<>3 then pause,end

// in function
function a=foo
    a=1//test
endfunction
a=foo();
if a<>1 then pause,end

function a=foo(..
    )
    a=1//test
endfunction
a=foo();
if a<>1 then pause,end

function a=foo// CALLING SYNTAX
    a=1//test
endfunction
a=foo();
if a<>1 then pause,end

function a=foo// CALLING SYNTAX
    a=0;k=0
    while %t //loop begin
        k=k+1; //ttttt
        if a> 10 then //
            break // go out the loop
        else //111111
            a=a-1; //sdfdfd
        end //xxxxx
        a=a+k; //hhhhh
    end //loop end
endfunction
a=foo();
if a<>15 then pause,end


function a=foo
    a=0;k=0;//initialization
    function a=fii  //internal function
        while %t //loop begin
            k=k+1; //ttttt
            if a> 10 then //
                break // go out the loop
            else //111111
                a=a-1; //sdfdfd
            end //xxxxx
            a=a+k; //hhhhh
        end //loop end
    endfunction
    a=fii()
    //SDSDFD
endfunction
a=foo();
if a<>15 then pause,end

warning("off");
//with  getf
t=["function a=foo"
"a=0;k=0;//initialization"
"  while %t //loop begin"
"    k=k+1; //ttttt"
"    if a> 10 then //"
"	break // go out the loop"
"    else //111111"
"	a=a-1; //sdfdfd"
"    end //xxxxx"
"    a=a+k; //hhhhh"
"  end //loop end"
"endfunction"];

mputl(t,TMPDIR+"/foo.sci");
exec(TMPDIR+"/foo.sci")
a=foo();
if a<>15 then pause,end
exec(TMPDIR+"/foo.sci")
a=foo();
if a<>15 then pause,end

t=["//sdffgfgfgggg"
"//sdsdsddfgfgfghgfhjhui"
"function a=foo//uuuuuuu"
"a=0;k=0;//initialization"
"  while %t //loop begin"
"    k=k+1; //ttttt"
"    if a> 10 then //"
"	break // go out the loop"
"    else //111111"
"	a=a-1; //sdfdfd"
"    end //xxxxx"
"    a=a+k; //hhhhh"
"  end //loop end"
"endfunction"];

mputl(t,TMPDIR+"/foo.sci");
exec(TMPDIR+"/foo.sci")
a=foo();
if a<>15 then pause,end
exec(TMPDIR+"/foo.sci")
a=foo();
if a<>15 then pause,end
warning("on");

//with macr2tree
//first define an utilitary function
function [C,L,lc]=getcommentsintree(T,lc)
    C=[],L=[];
    if argn(2)<2 then lc=0,end
    for k=1:size(T)
        l=T(k)
        if  l(1)(1)=="ifthenelse" then
            [CC,LL,lc]=getcommentsintree(l.then,lc)
            C=[C;CC]
            L=[L;LL]
            [CC,LL,lc]=getcommentsintree(l.elseifs,lc)
            C=[C;CC]
            L=[L;LL]
        [CC,LL,lc]=getcommentsintree(l.else,lc)
            C=[C;CC]
            L=[L;LL]
        elseif  l(1)(1)=="while" then
            [CC,LL,lc]=getcommentsintree(l.statements,lc)
            C=[C;CC]
            L=[L;LL]
        elseif l(1)(1)=="comment" then
            C=[C;l.text]
            L=[L;lc]
        elseif l(1)=="EOL" then
            lc=lc+1
        end
    end
endfunction

function a=foo
    a=0;k=0;//initialization
    while %t
        k=k+1; //ttttt ttttt
        if a> 10 then //f f
            break // go out the loop
        else //111 111
            a=a-1; //sdf dfd
        end //xx xxx
        a=a+k; //hhh hh
    end //loop end
endfunction
T=macr2tree(foo);
[C,L]=getcommentsintree(T.statements)
Cref=["initialization";
"ttttt ttttt";
"f f";
" go out the loop";
"111 111";
"sdf dfd";
"xx xxx";
"hhh hh";
"loop end"];
Lref=[1 3 5:6 8:12]';
if or(C<>Cref) then pause,end
if or(L<>Lref) then pause,end


function a=foo
    a=0;k=0;//initialization
    while %t //loop begin
        k=k+1; //ttttt tttt
        if a> 10 then //ff
            break // go out the loop
        else //111111
            a=a-1; //sdfdfd
        end //xxxxx
        a=a+k; //hhhhh
    end //loop end
endfunction

//T=macr2tree(foo);  bug a cause du commentaire //loop begin


deff("[y]=u(t)","if t==0 then y=0;else y=1,end") //step qsdqsqsd
function y=foo(a),y=a,endfunction//test qsdqsd
foo(2);  //zeros=0
foo("f//");
