// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
funcprot(0);
// simple
//
deff("[x,y]=t1(a,b)","x=a+b,y=a-b")
[u,v]=t1(1,2);
if u<> 3 then pause,end
if v<>-1 then pause,end
//
//with arguments
b=2;
if t1(1)<>t1(1,2) then pause,end
t2=t1;
//
[u,v]=t1(1,2);
if u<> 3 then pause,end
if v<>-1 then pause,end
b=2;
if t1(1)<>t1(1,2) then pause,end
//
//resume
//
deff("[x,y]=t3(a,b)","x=a+b,y=a-b,z=resume(a*a)")
[u,v]=t3(1,2);
if u<> 3 then pause,end
if v<>-1 then pause,end
if z<>1 then pause,end
b=2;
clear z
if t3(1)<>t3(1,2) then pause,end
if z<>1 then pause,end
t4=t3;
[u,v]=t3(1,2);
if u<> 3 then pause,end
if v<>-1 then pause,end
if z<>1 then pause,end
clear z
b=2;
if t3(1)<>t3(1,2) then pause,end
if z<>1 then pause,end
//
// macro + clauses
//
text = ["if n>0 then x=1"
"y=0;for k=1:n,y=y+k,end"
"else x=-1"
"if n==4 then y=''yes'' "
"else y=''no'' "
"end"
"end"
"if n==-1 then return,end"
"if n==-2 then tt=resume(n),end"
"select x,"
"case 1 then x=''pos'' ,tt=resume(''ok''),"
"case -1 then x=''neg'' ,"
"end"];
//
deff("[x,y]=t5(n)",text)
[u,v]=t5(5);
if u<>"pos" then pause,end
if v<>15 then pause,end
if tt<>"ok" then pause,end
clear tt
[u,v]=t5(-1);
if u<>-1 then pause,end
if v<>"no" then pause,end
if exists("tt")==1 then pause,end
clear tt v
u=t5(-2);
if u<>-1 then pause,end
if tt<>-2 then pause,end
clear tt
u=t5(-4);
if u<>"neg" then pause,end
if exists("tt")==1 then pause,end
clear u v tt
[u,v]=t5(5);
if u<>"pos" then pause,end
if v<>15 then pause,end
if tt<>"ok" then pause,end
clear tt
[u,v]=t5(-1);
if u<>-1 then pause,end
if v<>"no" then pause,end
if exists("tt")==1 then pause,end
clear tt v
u=t5(-2);
if u<>-1 then pause,end
if tt<>-2 then pause,end
clear tt
u=t5(-4);
if u<>"neg" then pause,end
if exists("tt")==1 then pause,end
//
// complex clauses
//
text = ["if n>0 then x=1"
"y=0;for k=1:b(n),y=y+b(k),end"
"else x=-1"
"if n==4 then y=b(''yes'') "
"else y=b(''no'') "
"end"
"end"
"if n==b(-1) then return,end"
"if n==b(-2) then tt=resume(b(n)),end"
"select x,"
"case 1 then x=b(''pos'') ,tt=resume(b(''ok'')),"
"case b(-1) then x=b(''neg'') ,"
"end"];
deff("[x]=b(a)","x=a,prod([1 1])")
deff("[x,y]=t5(n)",text)
[u,v]=t5(5);
if u<>"pos" then pause,end
if v<>15 then pause,end
if tt<>"ok" then pause,end
clear tt
[u,v]=t5(-1);
if u<>-1 then pause,end
if v<>"no" then pause,end
if exists("tt")==1 then pause,end
clear tt v
u=t5(-2);
if u<>-1 then pause,end
if tt<>-2 then pause,end
clear tt
u=t5(-4);
if u<>"neg" then pause,end
if exists("tt")==1 then pause,end
clear u v tt
[u,v]=t5(5);
if u<>"pos" then pause,end
if v<>15 then pause,end
if tt<>"ok" then pause,end
clear tt
[u,v]=t5(-1);
if u<>-1 then pause,end
if v<>"no" then pause,end
if exists("tt")==1 then pause,end
clear tt v
u=t5(-2);
if u<>-1 then pause,end
if tt<>-2 then pause,end
clear tt
u=t5(-4);
if u<>"neg" then pause,end
if exists("tt")==1 then pause,end
deff("[x,y]=t5(n)",text)
[u,v]=t5(5);
if u<>"pos" then pause,end
if v<>15 then pause,end
if tt<>"ok" then pause,end
clear tt
[u,v]=t5(-1);
if u<>-1 then pause,end
if v<>"no" then pause,end
if exists("tt")==1 then pause,end
clear tt v
u=t5(-2);
if u<>-1 then pause,end
if tt<>-2 then pause,end
clear tt
u=t5(-4);
if u<>"neg" then pause,end
if exists("tt")==1 then pause,end
//
clear u v tt
[u,v]=t5(5);
if u<>"pos" then pause,end
if v<>15 then pause,end
if tt<>"ok" then pause,end
clear tt
[u,v]=t5(-1);
if u<>-1 then pause,end
if v<>"no" then pause,end
if exists("tt")==1 then pause,end
clear tt v
u=t5(-2);
if u<>-1 then pause,end
if tt<>-2 then pause,end
clear tt
u=t5(-4);
if u<>"neg" then pause,end
if exists("tt")==1 then pause,end
//more complex
deff("[x]=b(a)","if a==1 then x=1,else x=a,prod([1 1]),end")
//
deff("[x,y]=t5(n)",text)
[u,v]=t5(5);
if u<>"pos" then pause,end
if v<>15 then pause,end
if tt<>"ok" then pause,end
clear tt
[u,v]=t5(-1);
if u<>-1 then pause,end
if v<>"no" then pause,end
if exists("tt")==1 then pause,end
clear tt v
u=t5(-2);
if u<>-1 then pause,end
if tt<>-2 then pause,end
clear tt
u=t5(-4);
if u<>"neg" then pause,end
if exists("tt")==1 then pause,end
clear u v tt
[u,v]=t5(5);
if u<>"pos" then pause,end
if v<>15 then pause,end
if tt<>"ok" then pause,end
clear tt
[u,v]=t5(-1);
if u<>-1 then pause,end
if v<>"no" then pause,end
if exists("tt")==1 then pause,end
clear tt v
u=t5(-2);
if u<>-1 then pause,end
if tt<>-2 then pause,end
clear tt
u=t5(-4);
if u<>"neg" then pause,end
if exists("tt")==1 then pause,end
//
deff("[x,y]=t5(n)",text)
[u,v]=t5(5);
if u<>"pos" then pause,end
if v<>15 then pause,end
if tt<>"ok" then pause,end
clear tt
[u,v]=t5(-1);
if u<>-1 then pause,end
if v<>"no" then pause,end
if exists("tt")==1 then pause,end
clear tt v
u=t5(-2);
if u<>-1 then pause,end
if tt<>-2 then pause,end
clear tt
u=t5(-4);
if u<>"neg" then pause,end
if exists("tt")==1 then pause,end
//
clear u v tt
[u,v]=t5(5);
if u<>"pos" then pause,end
if v<>15 then pause,end
if tt<>"ok" then pause,end
clear tt
[u,v]=t5(-1);
if u<>-1 then pause,end
if v<>"no" then pause,end
if exists("tt")==1 then pause,end
clear tt v
u=t5(-2);
if u<>-1 then pause,end
if tt<>-2 then pause,end
clear tt
u=t5(-4);
if u<>"neg" then pause,end
if exists("tt")==1 then pause,end
// resume
deff("[]=t6(a)","x=resume(a)")
clear ans x
t6(15)
if x<>15 then pause,end
if exists("ans")==1 then pause,end
//
clear ans x
t6(15)
if x<>15 then pause,end
if exists("ans")==1 then pause,end
//
//
//
deff("[ydot]=simul(t,y,a)","ydot=a")
a=2;
//appel le plus imple
y=ode(0,0,1:2,simul)
if norm(y-[a 2*a]) >1000*%eps then pause,end
y=ode(0,0,1:2,simul)
if norm(y-[a 2*a]) >1000*%eps then pause,end
//appel avec passage d'arguments supplementaires
deff("[ydot]=simul(t,y,a)","ydot=a")
y=ode(0,0,1:2,list(simul,a))
if norm(y-[a 2*a]) >1000*%eps then pause,end
y=ode(0,0,1:2,list(simul,a))
if norm(y-[a 2*a]) >1000*%eps then pause,end
//
//non linear
//
text=["for k=1:n,"
"y=ode(a*(k-1),k-1,k,simul),"
"x=[x,y],"
"end"];
//
deff("[x]=calcul(n)",text)
deff("[ydot]=simul(t,y,a)","ydot=a")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
deff("[ydot]=simul(t,y,a)","ydot=a")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
//
text(2)="y=ode(a*(k-1),k-1,k,list(simul,a)),";
deff("[x]=calcul(n)",text)
deff("[ydot]=simul(t,y,a)","ydot=a")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
deff("[ydot]=simul(t,y,a)","ydot=a")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
//resume in external
deff("[ydot]=simul(t,y,a)","ydot=a,tt=resume([tt,t])")
tt=[]
deff("[x]=calcul(n)",text)
deff("[ydot]=simul(t,y,a)","ydot=a")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
deff("[ydot]=simul(t,y,a)","ydot=a")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
//
//external calls a macro
//
deff("[x]=b(a)","if a==1 then x=a,prod([1 1]),else x=a,prod([1 1]),end")
text=["for k=1:n,"
"y=ode(a*(k-1),k-1,k,simul),"
"x=[x,y],"
"end"];
//
deff("[x]=calcul(n)",text)
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
//
text(2)="y=ode(a*(k-1),k-1,k,list(simul,a)),";
deff("[x]=calcul(n)",text)
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
//
deff("[ydot]=simul(t,y,a)","ydot=b(a),tt=resume([tt,t])")
tt=[]
deff("[x]=calcul(n)",text)
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
text=["for k=1:n,"
"y=ode(a*(k-1),k-1,k,simul),"
"x=[x,y],"
"end"];
//
deff("[x]=calcul(n)",text)
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
//
text(2)="y=ode(a*(k-1),k-1,k,list(simul,a)),";
deff("[x]=calcul(n)",text)
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
//
deff("[ydot]=simul(t,y,a)","ydot=b(a),tt=resume([tt,t])")
tt=[]
deff("[x]=calcul(n)",text)
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
deff("[ydot]=simul(t,y,a)","ydot=b(a)")
x=[];
x=calcul(3);
if norm(x-a*[1 2 3])>1000*%eps then pause,end
//
//macro defining ,compiling and executing a macro
//
text=["deff(''[x]=b(a)'',''if a==1 then x=1,else x=a,prod([1 1]),end''),"
"b,"
"x=b(n),"]
deff("[x]=t8(n)",text')
y=t8(10);
if y<>10 then pause,end
y=t8(10);
if y<>10 then pause,end
//
//
s=poly(0,"s");z=poly(0,"z");
deff("[ok]=cplist(l1,l2)",["ok=1;"
"if size(l1)<>size(l2) then ok=0,return,end"
"for k=1:length(l1),"
"if l1(k)<>l2(k) then ok=0,return,end;"
"end"])
deff("[r]=horner(p,x)",["if type(p)==15|type(p)==16 then";
"r=horner(p(2),x)./horner(p(3),x),";
"return,";
"end;";
"d=max(degree(p));";
"r=coeff(p,d);";
"for k=1:d,";
"r=r*x+coeff(p,d-k)*eye();";
"end;"])
deff("[f]=%p_r_p(p1,p2)",["[l,c]=size(p2);";
"if l*c <>1 then f=p1*invr(p2),return,end;";
"[l,c]=size(p1);";
"[p1 p2]=simp(p1,p2*ones(l,c));";
"f=rlist(p1,p2,[]);"])



horn=horner;
h=1/s;
if cplist(h,rlist(1,s,[]))==0 then pause,end
h1=(z-1)/(z+1);
hrn=horner(h,h1);
if cplist(hrn , rlist(z+1,z-1,[]))==0 then pause,end
//
hrn=horner(h,h1);
if cplist(hrn , rlist(z+1,z-1,[]))==0 then pause,end
//
h=1/s;
if cplist(h,rlist(1,s,[]))==0 then pause,end
h1=(z-1)/(z+1);
hrn=horn(h,h1);
if cplist(hrn , rlist(z+1,z-1,[]))==0 then pause,end
//
hrn=horn(h,h1);
if cplist(hrn , rlist(z+1,z-1,[]))==0 then pause,end
//
//
//
text=["aa=1,if a==2 then aa=b(a),end"]
fic=file("open",TMPDIR+"/test_macro_exec","unknown");
write(fic,text)
file("close",fic)
deff("[x]=b(a)","x=a,prod([1 1])")
deff("[x]=t9(a)","exec(TMPDIR+''/test_macro_exec''),x=aa")
y=t9(2)
if y<>2 then pause,end
ierr = execstr("file(""rewind"",fic);","errcatch");
assert_checkequal(ierr, 999);
errMsg = lasterror();
refMsg = msprintf(gettext("%s: Unknown file format.\n"), "file");
assert_checkequal(errMsg, refMsg);
y=t9(2)
if y<>2 then pause,end
//
deff("[x]=t9(a)","exec(TMPDIR+''/test_macro_exec''),x=aa")
y=t9(2)
if y<>2 then pause,end
y=t9(2)
if y<>2 then pause,end

