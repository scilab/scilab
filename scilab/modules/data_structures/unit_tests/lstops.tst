// Copyright INRIA
mode(5)
//-----------------------
//-- extractions tests --
//-----------------------

l=list(1,2,-4);
if l(1)<>1 then pause,end
if l(3)<>-4 then pause,end
[x1,x2]=l([1 3]);
if x1<>l(1)|x2<>l(3) then pause,end
[x2,x1]=l([3 1]);
if x1<>l(1)|x2<>l(3) then pause,end
[x1,x2]=l([1;3]);
if x1<>l(1)|x2<>l(3) then pause,end
[x2,x1]=l([3;1]);
if x1<>l(1)|x2<>l(3) then pause,end
[x1,x2,x3]=l(:);
if x1<>l(1)|x2<>l(2)|x3<>l(3) then pause,end

//with full tlists 
l=tlist(['t','a','b','c'],1,-2,5);
if l(2)<>1 then pause,end
if l(4)<>5 then pause,end
[x1,x2]=l([2 4]);
if x1<>l(2)|x2<>l(4) then pause,end
[x2,x1]=l([4 2]);
if x1<>l(2)|x2<>l(4) then pause,end
if l('a')<>1 then pause,end
if l('c')<>5 then pause,end
[x1,x2]=l(['a' 'c']);
if x1<>l('a')|x2<>l('c') then pause,end
[x2,x1]=l(['c' 'a']);
if x1<>l('a')|x2<>l('c') then pause,end
[x1,x2]=l(['a';'c']);
if x1<>l('a')|x2<>l('c') then pause,end
[x2,x1]=l(['c';'a']);
if x1<>l('a')|x2<>l('c') then pause,end

//with partial tlists (no elements formal names)
l=tlist('t',1,-2,5);
deff('[f1,f2,f3,f4,f5,f6,f7]=%t_e(i,j,f)',[
  'nams=[''a'',''b'',''c''];'
  'for k=1:prod(size(i))';
  '   kf=find(i(k)==nams);';
  '   if kf==[] then error(21),end;'
  '   execstr(''f''+string(k)+''=j(kf+1)'');'
  '   end;'])
if l(2)<>1 then pause,end
if l(4)<>5 then pause,end
[x1,x2]=l([2 4]);
if x1<>l(2)|x2<>l(4) then pause,end
[x2,x1]=l([4 2]);
if x1<>l(2)|x2<>l(4) then pause,end
if l('a')<>1 then pause,end
if l('c')<>5 then pause,end
[x1,x2]=l(['a' 'c']);
if x1<>l('a')|x2<>l('c') then pause,end
[x2,x1]=l(['c' 'a']);
if x1<>l('a')|x2<>l('c') then pause,end
[x1,x2]=l(['a';'c']);
if x1<>l('a')|x2<>l('c') then pause,end
[x2,x1]=l(['c';'a']);
if x1<>l('a')|x2<>l('c') then pause,end

// extraction inside a input argument list
a=list(1,2,3,4);
if or(list(a(1:2))<>list(1,2)) then pause,end
if type(list(a(1:2)))<>15 then pause,end
deff('[a,b]=foo(x,y)','a=x;b=y')
[u,v]=foo(a(1:2));if u<>a(1)|v<>a(2) then pause,end
deff('[a]=foo(x,y)','a=x+y')
if abs(foo(a(1:2)))<>3 then pause,end

//---------------------
//-- insertion tests --
//---------------------
l=list(2,'a',-1);
l(1)=[1 2];
if l<>list([1 2],'a',-1) then pause,end
l(1)=33;
if l<>list(33,'a',-1) then pause,end
l(1)=33;
if l<>list(33,'a',-1) then pause,end
l(0)=1.25;
if l<>list(1.25,33,'a',-1) then pause,end
l(5)=[1 2 3];
if l<>list(1.25,33,'a',-1,[1 2 3]) then pause,end
l(3)=null();
if l<>list(1.25,33,-1,[1 2 3]) then pause,end
l(3)=null();
if l<>list(1.25,33,[1 2 3]) then pause,end
l(4)=null();
if l<>list(1.25,33,[1 2 3]) then pause,end
l(0)=null();
if l<>list(1.25,33,[1 2 3]) then pause,end
l=tlist('t',2,'a',-1);
l(2)=[1 2 3];
if %l_n_l(l,tlist('t',[1 2 3],'a',-1)) then pause,end
l(2)=33;
if %l_n_l(l,tlist('t',33,'a',-1)) then pause,end
l(2)=33;
if %l_n_l(l,tlist('t',33,'a',-1)) then pause,end 
l(4)=[1 2];
if %l_n_l(l,tlist('t',33,'a',[1,2])) then pause,end
l(2)=null();
if %l_n_l(l,tlist('t','a',[1,2])) then pause,end
l(1)=null();
if %l_n_l(l,list('a',[1,2])) then pause,end
