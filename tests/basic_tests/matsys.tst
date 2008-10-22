// test of matsys functions
clear

// debug
if debug()<>0 then pause,end
//debug(4)
//if debug()<>4 then pause,end
debug(0)

//who
a=33;bbb=-1;
vars=who('get');
if or(vars(1:2)<>['bbb';'a']) then pause,end

//lines
ln=lines();lines(30);ln1=lines();
if ln1(2)<>30 then pause,end
lines(45,70);ln1=lines();
if or(ln1<>[70,45]) then pause,end


//argn
deff('[x,y]=foo(a,b)','[lhs,rhs]=argn(0);x=[lhs,rhs];y=-1')
if or(foo(1)<>[1 1]) then pause,end
if or(foo(1,2)<>[1 2]) then pause,end
if or(foo()<>[1 0]) then pause,end
[x,y]=foo(1);if or(x<>[2 1]) then pause,end
[x,y]=foo(1,1);if or(x<>[2 2]) then pause,end
[x,y]=foo();if or(x<>[2 0]) then pause,end
deff('[x,y]=foo(a,b)','lhs=argn(0);x=lhs;y=-1')
if foo(1)<>1 then pause,end
if foo(1,2)<>1 then pause,end
[x,y]=foo(1);if x<>2 then pause,end
//
deff('[x,y]=foo(a,b)','[lhs,rhs]=argn();x=[lhs,rhs];y=-1')
if or(foo(1)<>[1 1]) then pause,end
if or(foo(1,2)<>[1 2]) then pause,end
if or(foo()<>[1 0]) then pause,end
[x,y]=foo(1);if or(x<>[2 1]) then pause,end
[x,y]=foo(1,1);if or(x<>[2 2]) then pause,end
[x,y]=foo();if or(x<>[2 0]) then pause,end
deff('[x,y]=foo(a,b)','lhs=argn();x=lhs;y=-1')
if foo(1)<>1 then pause,end
if foo(1,2)<>1 then pause,end
[x,y]=foo(1);if x<>2 then pause,end

//type
if type(1)<>1 then pause,end
a=%s;if type(a)<>2  then pause,end

//mode (cannot be tested here)
//mode(7);if mode()<>7 then pause,end;mode(2);

//errcatch,errclear,iserror
clear
//errcatch(4,'continue')
//unknown
//if iserror()<>1 then pause,end
//errclear()
//if iserror()<>0 then pause,end
//unknown
//if iserror(4)<>1 then pause,end
//errclear(4)
//if iserror(4)<>0 then pause,end
errcatch(4,'continue','nomessage')
unknown
if iserror()<>1 then pause,end
errclear()
if iserror()<>0 then pause,end
unknown
if iserror(4)<>1 then pause,end
errclear(4)
if iserror(4)<>0 then pause,end
errcatch()

//error
errcatch(4,'continue','nomessage')
error(4)
if iserror()<>1 then pause,end
errclear();errcatch()
errcatch(-1,'continue','nomessage')
error('test erreur');
errclear();
error('test erreur',999);
errclear();
errcatch()

// lasterror
errcatch(4,'continue','nomessage')
unknown
if stripblanks(lasterror(%f))<>'undefined variable : unknown' then pause,end
[m,n]=lasterror();
if stripblanks(m)<>'undefined variable : unknown' then pause,end
if n<>4 then pause,end
if lasterror()<>[] then pause,end
[m,n]=lasterror();
if m<>[]|n<>0 then pause,end



//resume
deff('foo()','[a,b]=resume(''x'',''y'')')
foo();
if a<>'x'|b<>'y' then pause,end

//format
fmt=format();
format('v')
sqrt(3)
format(10)
sqrt(3)
format(12,'v')
sqrt(3)
format('v',15)
sqrt(3)
format('e')
sqrt(3)
format(10)
sqrt(3)
f=format();
if or(f<>[0 10]) then pause,end
format(10,'v');

//exists
a=1
if exists('a')<>1 then pause,end
clear a
if exists('a')<>0 then pause,end

//predef
if %f then
N=predef();
a=1;
predef('all');
errcatch(13,'continue','nomessage')
clear a 
if iserror()<>1 then pause,end
errclear();errcatch()
predef('clear');
errcatch(13,'continue','nomessage')
clear a
if iserror()<>0 then pause,end
errclear();errcatch()
predef(N);
if predef()<>N then pause,end
end

// newfun, clearfun, funptr
fptr=funptr('sin');
if fptr<>623 then pause,end
newfun('mysin',fptr);
if mysin(1)<>sin(1) then pause,end
clearfun('mysin');
errcatch(4,'continue','nomessage')
mysin(1)
if iserror()<>1 then pause,end
errclear();errcatch();

//whereis
if whereis('sci2exp')<>'stringlib' then pause,end
if whereis('aaa')<>[]  then pause,end

//where
deff('[a,b]=foo()','[a,b]=where()')
[a,b]=foo();
if a<>2|b<>'foo' then pause,end

//funcprot
funcprot(0)
deff('foo()','a=1')
funcprot(1)
deff('foo()','a=2')
errcatch(111,'continue','nomessage')
funcprot(2)
deff('foo()','a=3')
if iserror()<>1 then pause,end
errclear();errcatch()
if funcprot()<>2 then pause,end
funcprot(1)

// timer
timer();for i=1:100000,end;if timer()<=0 then pause,end

// comp
deff('[a]=foo()','a=1','n')
comp(foo)
if foo()<>1 then pause,end

// stacksize 

a=stacksize();a=a(1);
stacksize(a+1000)
y=stacksize();if y(1)<>a+1000 then pause,end

//non compiled functions
deff('y=foo(a)','x=a+1000,stacksize(x),y=stacksize();y=y(1)','n')
a=stacksize();a=a(1);
y=foo(a);
if y<>a+1000 then pause,end
a=y;
deff('y=toto()','a=stacksize();a=a(1);y=foo(a)','n')
y=toto();
if y<>a+1000 then pause,end
a=y;
exec(toto)
if y<>a+1000 then pause,end
a=y;

// compiled functions
deff('y=foo(a)','x=a+1000,stacksize(x),y=stacksize();y=y(1)')
y=foo(a);
if y<>a+1000 then pause,end
a=y;
deff('y=toto()','a=stacksize();a=a(1);y=foo(a)')
y=toto();
if y<>a+1000 then pause,end
a=y;
exec(toto)
if y<>a+1000 then pause,end
a=y;

//in a loop
for k=1:3,
  y=toto();
  if y<>a+1000 then pause,end
  a=y;
end


//mtlb_mode
mtlb_mode(%t)
if ~mtlb_mode() then pause,end
if []+1<>[] then pause,end
mtlb_mode(%f)
if mtlb_mode() then pause,end


//sciargs
args=sciargs();
if type(args)<>10 then pause,end

//ieee
ieee(2)
if 1/0<>%inf then pause,end
ieee(1)
if 1/0<>%inf then pause,end
ieee(0)
errcatch(27,'continue','nomessage')
1/0
if iserror()<>1 then pause,end
errclear();errcatch()


//typename
[typs,nams]=typename();
if or(typs<>[1;2;4;5;6;7;8;9;10;11;13;14;15;16;17;128;129;130]) then pause,end

if or(nams<>['s';'p';'b';'sp';'spb';'msp';'i';'h';'c';'m';'mc';
  'f';'l';'tl';'ml';'ptr';'ip';'fptr'])  then pause,end
typename('mytype',1024)
[typs,nams]=typename();
if typs($)<>1024|nams($)<>'mytype'  then pause,end
typename('',1024)

//global, isglobal, intglobal  --> global.tst
