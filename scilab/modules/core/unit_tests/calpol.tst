// Copyright INRIA
//
deff('[ok]=cmpr(h1,h2,eps)',['h1=h1-h2;';
         'if norm(coeff(h1(2)))>eps then ok=0,else ok=1,end'])

s=poly(0,'s');
//
//rationals
//
num=1;den=1+s;
eps=5000*%eps;
rtyp=['r','num','den','dt'];
if cmpr(num/den,tlist(rtyp,num,den,[]),eps)<>1 then pause,end
if cmpr(den\num,tlist(rtyp,num,den,[]),eps)<>1 then pause,end
if cmpr(num./den,tlist(rtyp,num,den,[]),eps)<>1 then pause,end
if cmpr(den.\num,tlist(rtyp,num,den,[]),eps)<>1 then pause,end
num=1.5+s**3;
if cmpr(num/den,tlist(rtyp,num,den,[]),eps)<>1 then pause,end
if cmpr(den\num,tlist(rtyp,num,den,[]),eps)<>1 then pause,end
if cmpr(num./den,tlist(rtyp,num,den,[]),eps)<>1 then pause,end
if cmpr(den.\num,tlist(rtyp,num,den,[]),eps)<>1 then pause,end
//
h1=num/den;x=1.5;
if cmpr(h1+x,tlist(rtyp,num+x*den,den,[]),eps)<>1 then pause,end
if cmpr(x+h1,tlist(rtyp,num+x*den,den,[]),eps)<>1 then pause,end
if cmpr(h1-x,tlist(rtyp,num-x*den,den,[]),eps)<>1 then pause,end
if cmpr(x-h1,tlist(rtyp,-num+x*den,den,[]),eps)<>1 then pause,end
 
x=1.5+3*s;
if cmpr(h1+x,tlist(rtyp,num+x*den,den,[]),eps)<>1 then pause,end
if cmpr(x+h1,tlist(rtyp,num+x*den,den,[]),eps)<>1 then pause,end
if cmpr(h1-x,tlist(rtyp,num-x*den,den,[]),eps)<>1 then pause,end
if cmpr(x-h1,tlist(rtyp,-num+x*den,den,[]),eps)<>1 then pause,end
y=s**3;h2=x/y;
if cmpr(h1+h2,tlist(rtyp,num*y+x*den,den*y,[]),eps)<>1 then pause,end
if cmpr(h1-h2,tlist(rtyp,num*y-x*den,den*y,[]),eps)<>1 then pause,end
//
// concatenations
//
h1=num/den;x=1.5;
if cmpr([h1,x],tlist(rtyp,[num,x],[den,1],[]),eps)<>1 then pause,end
if cmpr([x,h1],tlist(rtyp,[x,num],[1,den],[]),eps)<>1 then pause,end
if cmpr([h1;x],tlist(rtyp,[num;x],[den;1],[]),eps)<>1 then pause,end
if cmpr([x;h1],tlist(rtyp,[x;num],[1;den],[]),eps)<>1 then pause,end
x=1.5+3*s;
if cmpr([h1,x],tlist(rtyp,[num,x],[den,1],[]),eps)<>1 then pause,end
if cmpr([x,h1],tlist(rtyp,[x,num],[1,den],[]),eps)<>1 then pause,end
if cmpr([h1;x],tlist(rtyp,[num;x],[den;1],[]),eps)<>1 then pause,end
if cmpr([x;h1],tlist(rtyp,[x;num],[1;den],[]),eps)<>1 then pause,end
y=-0.5+s**3;h2=x/y;
if cmpr([h1,h2],tlist(rtyp,[num,x],[den,y],[]),eps)<>1 then pause,end
if cmpr([h1;h2],tlist(rtyp,[num;x],[den;y],[]),eps)<>1 then pause,end
h1=[num/den,den/num];x=[0.3 1.5];
if cmpr([h1,x],tlist(rtyp,[h1(2),x],[h1(3),ones(x)],[]),eps)<>1 then pause,end
if cmpr([x,h1],tlist(rtyp,[x,h1(2)],[ones(x),h1(3)],[]),eps)<>1 then pause,end
if cmpr([h1;x],tlist(rtyp,[h1(2);x],[h1(3);ones(x)],[]),eps)<>1 then pause,end
if cmpr([x;h1],tlist(rtyp,[x;h1(2)],[ones(x);h1(3)],[]),eps)<>1 then pause,end
h1=[num/den;den/num];x=[0.3;-1.5];
if cmpr([h1,x],tlist(rtyp,[h1(2),x],[h1(3),ones(x)],[]),eps)<>1 then pause,end
if cmpr([x,h1],tlist(rtyp,[x,h1(2)],[ones(x),h1(3)],[]),eps)<>1 then pause,end
if cmpr([h1;x],tlist(rtyp,[h1(2);x],[h1(3);ones(x)],[]),eps)<>1 then pause,end
if cmpr([x;h1],tlist(rtyp,[x;h1(2)],[ones(x);h1(3)],[]),eps)<>1 then pause,end
x=[1.5+3*s;-1+s**3];
if cmpr([h1,x],tlist(rtyp,[h1(2),x],[h1(3),ones(x)],[]),eps)<>1 then pause,end
if cmpr([x,h1],tlist(rtyp,[x,h1(2)],[ones(x),h1(3)],[]),eps)<>1 then pause,end
if cmpr([h1;x],tlist(rtyp,[h1(2);x],[h1(3);ones(x)],[]),eps)<>1 then pause,end
if cmpr([x;h1],tlist(rtyp,[x;h1(2)],[ones(x);h1(3)],[]),eps)<>1 then pause,end
h1=[num/den,den/num];x=[1.5+3*s,-1+s**2];
if cmpr([h1,x],tlist(rtyp,[h1(2),x],[h1(3),ones(x)],[]),eps)<>1 then pause,end
if cmpr([x,h1],tlist(rtyp,[x,h1(2)],[ones(x),h1(3)],[]),eps)<>1 then pause,end
if cmpr([h1;x],tlist(rtyp,[h1(2);x],[h1(3);ones(x)],[]),eps)<>1 then pause,end
if cmpr([x;h1],tlist(rtyp,[x;h1(2)],[ones(x);h1(3)],[]),eps)<>1 then pause,end
h1=[num/den;den/num];y=-0.5+s**3;h2=[num/y;y*y/(y+1)];
if cmpr([h1,h2],tlist(rtyp,[h1(2),h2(2)],[h1(3),h2(3)],[]),eps)<>1 then pause,end
if cmpr([h1;h2],tlist(rtyp,[h1(2);h2(2)],[h1(3);h2(3)],[]),eps)<>1 then pause,end
h1=[num/den,den/num];y=-0.5+s**3;h2=[num/y,y*y/(y+1)];
if cmpr([h1,h2],tlist(rtyp,[h1(2),h2(2)],[h1(3),h2(3)],[]),eps)<>1 then pause,end
if cmpr([h1;h2],tlist(rtyp,[h1(2);h2(2)],[h1(3);h2(3)],[]),eps)<>1 then pause,end
//
// extraction
//
h1=[num/den,den/num];
if cmpr(h1(1,1),num/den,eps)<>1 then pause,end
if cmpr(h1(1,1:2),h1,eps)<>1 then pause,end
if cmpr(h1(1,[2 1]),[den/num,num/den],eps)<>1 then pause,end
h1=[num/den;den/num];
if cmpr(h1(2,1),den/num,eps)<>1 then pause,end
if cmpr(h1(1:2,1),h1,eps)<>1 then pause,end
if cmpr(h1([2 1],1),[den/num;num/den],eps)<>1 then pause,end
y=-0.5+s**3;h1=[num/den,den/num];h2=[num/den,den/num;num/y,y*y/(y+1)];
if cmpr(h2(2,1),num/y,eps)<>1 then pause,end
if cmpr(h2(1:2,1:2),h2,eps)<>1 then pause,end
if cmpr(h2([2 1],1),[num/y;num/den],eps)<>1 then pause,end
if cmpr(h2(:,1),[num/den;num/y],eps)<>1 then pause,end
if cmpr(h2(2,:),[num/y,y*y/(y+1)],eps)<>1 then pause,end
if cmpr(h2(:,:),h2,eps)<>1 then pause,end
//
// insertions
//
h1=[num/den,den/num];x=0.33;
hh=h1;hh(1,1)=x;if cmpr(hh,[x,h1(1,2)],eps)<>1 then pause,end;
x=[-2.67 0.8];
hh=h1;hh(1,1:2)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(1,[2 1])=x;if cmpr(hh,x([2,1]),eps)<>1 then pause,end;
hh=h1;hh(1,[2;1])=x;if cmpr(hh,x([2,1]),eps)<>1 then pause,end;
hh=h1;hh(1,:)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(:,:)=x;if cmpr(hh,x,eps)<>1 then pause,end;
h1=[num/den;den/num];x=0.33;
hh=h1;hh(1,1)=x;if cmpr(hh,[x;h1(2,1)],eps)<>1 then pause,end;
x=[-2.67;0.8];
hh=h1;hh(1:2,1)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(:,1)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(:,:)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh([2 1],1)=x;if cmpr(hh,x([2 1]),eps)<>1 then pause,end;
hh=h1;hh([2;1],1)=x;if cmpr(hh,x([2 1]),eps)<>1 then pause,end;
 
h1=[num/den,den/num];x=0.33*s+1;
hh=h1;hh(1,1)=x;if cmpr(hh,[x,h1(1,2)],eps)<>1 then pause,end;
x=[-2.67 0.8+s**3];
hh=h1;hh(1,1:2)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(1,[2 1])=x;if cmpr(hh,x([2,1]),eps)<>1 then pause,end;
hh=h1;hh(1,[2;1])=x;if cmpr(hh,x([2,1]),eps)<>1 then pause,end;
hh=h1;hh(1,:)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(:,:)=x;if cmpr(hh,x,eps)<>1 then pause,end;
h1=[num/den;den/num];x=-0.33+38*s;
hh=h1;hh(1,1)=x;if cmpr(hh,[x;h1(2,1)],eps)<>1 then pause,end;
x=[-2.67-s*8;0.8];
hh=h1;hh(1:2,1)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(:,1)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(:,:)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh([2 1],1)=x;if cmpr(hh,x([2 1]),eps)<>1 then pause,end;
hh=h1;hh([2;1],1)=x;if cmpr(hh,x([2 1]),eps)<>1 then pause,end;
h1=[num/den,den/num];y=0.33*s+1;x=y*y/(y+1);
hh=h1;hh(1,1)=x;if cmpr(hh,[x,h1(1,2)],eps)<>1 then pause,end;
x=[num/y,y*y/(y+1)];
hh=h1;hh(1,1:2)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(1,[2 1])=x;if cmpr(hh,x(1,[2,1]),eps)<>1 then pause,end;
hh=h1;hh(1,[2;1])=x;if cmpr(hh,x(1,[2,1]),eps)<>1 then pause,end;
hh=h1;hh(1,:)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(:,:)=x;if cmpr(hh,x,eps)<>1 then pause,end;
h1=[num/den;den/num];x=[num/y;y*y/(y+1)];
hh=h1;hh(1:2,1)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(:,1)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh(:,:)=x;if cmpr(hh,x,eps)<>1 then pause,end;
hh=h1;hh([2 1],1)=x;if cmpr(hh,x([2 1],1),eps)<>1 then pause,end;
hh=h1;hh([2;1],1)=x;if cmpr(hh,x([2 1],1),eps)<>1 then pause,end;
//
// matrix operations 
//
h1=[num/den,den/num];x=[0.3 1.5];
if cmpr(-h1,(-1)*h1,eps)<>1 then pause,end
if cmpr(h1+x,[h1(1,1)+x(1,1) h1(1,2)+x(1,2)],eps)<>1 then pause,end
if cmpr(x+h1,[h1(1,1)+x(1,1) h1(1,2)+x(1,2)],eps)<>1 then pause,end
if cmpr(h1-x,[h1(1,1)-x(1,1) h1(1,2)-x(1,2)],eps)<>1 then pause,end
if cmpr(x-h1,[-h1(1,1)+x(1,1) -h1(1,2)+x(1,2)],eps)<>1 then pause,end
h1=[num/den;den/num];x=[0.3;1.5];
if cmpr(-h1,(-1)*h1,eps)<>1 then pause,end
if cmpr(h1+x,[h1(1,1)+x(1,1); h1(2,1)+x(2,1)],eps)<>1 then pause,end
if cmpr(x+h1,[h1(1,1)+x(1,1); h1(2,1)+x(2,1)],eps)<>1 then pause,end
if cmpr(h1-x,[h1(1,1)-x(1,1); h1(2,1)-x(2,1)],eps)<>1 then pause,end
if cmpr(x-h1,[-h1(1,1)+x(1,1); -h1(2,1)+x(2,1)],eps)<>1 then pause,end
 
 
h1=[num/den,den/num];x=[0.3+s, 1.5];
if cmpr(-h1,(-1)*h1,eps)<>1 then pause,end
if cmpr(h1+x,[h1(1,1)+x(1,1) h1(1,2)+x(1,2)],eps)<>1 then pause,end
if cmpr(x+h1,[h1(1,1)+x(1,1) h1(1,2)+x(1,2)],eps)<>1 then pause,end
if cmpr(h1-x,[h1(1,1)-x(1,1) h1(1,2)-x(1,2)],eps)<>1 then pause,end
if cmpr(x-h1,[-h1(1,1)+x(1,1) -h1(1,2)+x(1,2)],eps)<>1 then pause,end
h1=[num/den;den/num];x=[0.3;1.5-3*s];
if cmpr(-h1,(-1)*h1,eps)<>1 then pause,end
if cmpr(h1+x,[h1(1,1)+x(1,1); h1(2,1)+x(2,1)],eps)<>1 then pause,end
if cmpr(x+h1,[h1(1,1)+x(1,1); h1(2,1)+x(2,1)],eps)<>1 then pause,end
if cmpr(h1-x,[h1(1,1)-x(1,1); h1(2,1)-x(2,1)],eps)<>1 then pause,end
if cmpr(x-h1,[-h1(1,1)+x(1,1); -h1(2,1)+x(2,1)],eps)<>1 then pause,end
//
//
h1=[num/den,den/num];
if cmpr([num,den]/den,[num/den,1],eps)<>1 then pause,end
if cmpr(den\[num,den],[num/den,1],eps)<>1 then pause,end
if cmpr([num,den]./[den,num],h1,eps)<>1 then pause,end
if cmpr([den,num].\[num,den],h1,eps)<>1 then pause,end
h1=[num/den,den/num];x=[0.3 1.5];
if cmpr(h1/x(1),[h1(1,1)/x(1),h1(1,2)/x(1)],eps)<>1 then pause,end
if cmpr(x(1)\h1,[h1(1,1)/x(1),h1(1,2)/x(1)],eps)<>1 then pause,end
if cmpr(h1./x,[h1(1,1)/x(1),h1(1,2)/x(2)],eps)<>1 then pause,end
if cmpr(x.\h1,[h1(1,1)/x(1),h1(1,2)/x(2)],eps)<>1 then pause,end
if cmpr(h1*x(1),[h1(1,1)*x(1),h1(1,2)*x(1)],eps)<>1 then pause,end
if cmpr(h1.*x,[h1(1,1)*x(1),h1(1,2)*x(2)],eps)<>1 then pause,end
h1=[num/den;den/num];
if cmpr([num;den]/den,[num/den;1],eps)<>1 then pause,end
if cmpr(den\[num;den],[num/den;1],eps)<>1 then pause,end
if cmpr([num;den]./[den;num],h1,eps)<>1 then pause,end
if cmpr([den;num].\[num;den],h1,eps)<>1 then pause,end
 
x=[0.3;1.5];
if cmpr(h1/x(1),[h1(1,1)/x(1);h1(2,1)/x(1)],eps)<>1 then pause,end
if cmpr(x(1)\h1,[h1(1,1)/x(1);h1(2,1)/x(1)],eps)<>1 then pause,end
if cmpr(h1./x,[h1(1,1)/x(1);h1(2,1)/x(2)],eps)<>1 then pause,end
if cmpr(x.\h1,[h1(1,1)/x(1);h1(2,1)/x(2)],eps)<>1 then pause,end
if cmpr(h1*x(1),[h1(1,1)*x(1);h1(2,1)*x(1)],eps)<>1 then pause,end
if cmpr(h1.*x,[h1(1,1)*x(1);h1(2,1)*x(2)],eps)<>1 then pause,end
if cmpr(h1'*x,h1(1,1)*x(1)+h1(2,1)*x(2),eps)<>1 then pause,end
if cmpr(h1*x',[h1(1,1)*x(1),h1(1,1)*x(2);h1(2,1)*x(1),h1(2,1)*x(2)],eps)<>1,
	then pause,end
h1=[num/den,den/num];x=[0.3+3*s, 1.5];
if cmpr(h1/x(1),[h1(1,1)/x(1),h1(1,2)/x(1)],eps)<>1 then pause,end
if cmpr(x(1)\h1,[h1(1,1)/x(1),h1(1,2)/x(1)],eps)<>1 then pause,end
if cmpr(h1./x,[h1(1,1)/x(1),h1(1,2)/x(2)],eps)<>1 then pause,end
if cmpr(x.\h1,[h1(1,1)/x(1),h1(1,2)/x(2)],eps)<>1 then pause,end
if cmpr(h1*x(1),[h1(1,1)*x(1),h1(1,2)*x(1)],eps)<>1 then pause,end
if cmpr(h1.*x,[h1(1,1)*x(1),h1(1,2)*x(2)],eps)<>1 then pause,end
if cmpr(h1*x',h1(1,1)*x(1)+h1(1,2)*x(2),eps)<>1 then pause,end
if cmpr(h1'*x,[h1(1,1)*x(1),h1(1,1)*x(2);h1(1,2)*x(1),h1(1,2)*x(2)],eps)<>1,
	then pause,end
h1=[num/den;den/num];x=[0.3+3*s; 1.5];
if cmpr(h1/x(1),[h1(1,1)/x(1);h1(2,1)/x(1)],eps)<>1 then pause,end
if cmpr(x(1)\h1,[h1(1,1)/x(1);h1(2,1)/x(1)],eps)<>1 then pause,end
if cmpr(h1./x,[h1(1,1)/x(1);h1(2,1)/x(2)],eps)<>1 then pause,end
if cmpr(x.\h1,[h1(1,1)/x(1);h1(2,1)/x(2)],eps)<>1 then pause,end
if cmpr(h1*x(1),[h1(1,1)*x(1);h1(2,1)*x(1)],eps)<>1 then pause,end
if cmpr(h1.*x,[h1(1,1)*x(1);h1(2,1)*x(2)],eps)<>1 then pause,end
if cmpr(h1'*x,h1(1,1)*x(1)+h1(2,1)*x(2),eps)<>1 then pause,end
if cmpr(h1*x',[h1(1,1)*x(1),h1(1,1)*x(2);h1(2,1)*x(1),h1(2,1)*x(2)],eps)<>1,
	then pause,end
h1=[num/den,den/num];x=[0.3/s,1.5-s**2/(1+s**2)];
if cmpr(h1/x(1,1),[h1(1,1)/x(1,1),h1(1,2)/x(1,1)],eps)<>1 then pause,end
if cmpr(x(1,1)\h1,[h1(1,1)/x(1,1),h1(1,2)/x(1,1)],eps)<>1 then pause,end
if cmpr(h1./x,[h1(1,1)/x(1,1),h1(1,2)/x(1,2)],eps)<>1 then pause,end
if cmpr(x.\h1,[h1(1,1)/x(1,1),h1(1,2)/x(1,2)],eps)<>1 then pause,end
if cmpr(h1*x(1,1),[h1(1,1)*x(1,1),h1(1,2)*x(1,1)],eps)<>1 then pause,end
if cmpr(h1.*x,[h1(1,1)*x(1,1),h1(1,2)*x(1,2)],eps)<>1 then pause,end
 
h1=[num/den;den/num];x=[0.3/s;1.5-s**2/(1+s**2)];
if cmpr(h1/x(1,1),[h1(1,1)/x(1,1);h1(2,1)/x(1,1)],eps)<>1 then pause,end
if cmpr(x(1,1)\h1,[h1(1,1)/x(1,1);h1(2,1)/x(1,1)],eps)<>1 then pause,end
if cmpr(h1./x,[h1(1,1)/x(1,1);h1(2,1)/x(2,1)],eps)<>1 then pause,end
if cmpr(x.\h1,[h1(1,1)/x(1,1);h1(2,1)/x(2,1)],eps)<>1 then pause,end
if cmpr(h1*x(1,1),[h1(1,1)*x(1,1);h1(2,1)*x(1,1)],eps)<>1 then pause,end
if cmpr(h1.*x,[h1(1,1)*x(1,1);h1(2,1)*x(2,1)],eps)<>1 then pause,end
if cmpr(h1'*x,h1(1,1)*x(1,1)+h1(2,1)*x(2,1),eps)<>1 then pause,end
if cmpr(h1*x',[h1(1,1)*x(1,1),h1(1,1)*x(2,1)
               h1(2,1)*x(1,1),h1(2,1)*x(2,1)],eps)<>1,
	then pause,end 
//
 
h1=h1*x';x=[1 2;3 4];
if cmpr(h1/x,h1*inv(x),eps)<>1 then pause,end
if cmpr(x\h1,inv(x)*h1,eps)<>1 then pause,end
x=[s s*s+s-1;1 s+1];xi=[1+s,1-s-s*s;-1 s];
if norm(coeff(invr(x)-xi))>eps then pause,end
if cmpr(h1/x,h1*xi,eps)<>1 then pause,end
if cmpr(x\h1,xi*h1,eps)<>1 then pause,end
if cmpr(x**(-1),xi,eps)<>1 then pause,end
x=[1/(1+s) 1/(s*(s+1))-1;1 1/s];xi=[1/s,-1/(s*(s+1))+1;-1,1/(1+s)];
if cmpr(invr(x),xi,eps)<>1 then pause,end
if cmpr(h1/x,h1*xi,eps)<>1 then pause,end
if cmpr(x\h1,xi*h1,eps)<>1 then pause,end
if cmpr((1/(1+s))**3,1/((1+s)**3),eps)<>1 then pause,end
x=[1/(1+s),1;0 1/s];x3=[x(1,1)**3,x(1,1)**2+x(2,2)*(x(2,2)+x(1,1));0,x(2,2)**3];
if cmpr(x**3,x3,eps)<>1 then pause,end


//Bezout

mode(5)
//test
un=poly(1,'s','c');
zer=0*un;
s=poly(0,'s');
[p,q]=bezout(un,s);
if norm(coeff([un s]*q-[p 0]))>10*%eps then pause,end
[p,q]=bezout(s,un);
if norm(coeff([s un]*q-[p 0]))>10*%eps then pause,end
[p,q]=bezout(un,un);
if norm(coeff([un un]*q-[p 0]))>10*%eps then pause,end
//
[p,q]=bezout(zer,s);
if norm(coeff([zer s]*q-[p 0]))>10*%eps then pause,end
[p,q]=bezout(s,zer);
if norm(coeff([s zer]*q-[p 0]))>10*%eps then pause,end
[p,q]=bezout(zer,zer);
if norm(coeff([zer zer]*q-[p 0]))>10*%eps then pause,end
//
[p,q]=bezout(zer,un);
if norm(coeff([zer un]*q-[p 0]))>10*%eps then pause,end
[p,q]=bezout(un,zer);
if norm(coeff([un zer]*q-[p 0]))>10*%eps then pause,end
//
 
//simple
a=poly([1 2 3],'z');
b=poly([4 1],'z');
 
[p q]=bezout(a,b);
dt=q(1,1)*q(2,2)-q(1,2)*q(2,1);dt0=coeff(dt,0);
if norm(coeff(dt/dt0-1))>10*%eps then pause,end
qi=[q(2,2) -q(1,2);-q(2,1) q(1,1)]/dt0;
if norm(coeff([p 0]*qi-[a b]))>100*%eps then pause,end
 
//more difficult
 
b1=poly([4 1+1000*%eps],'z');del=10*norm(coeff(b1-b));
[p,q]=bezout(a,b1);
dt=q(1,1)*q(2,2)-q(1,2)*q(2,1);dt0=coeff(dt,0);
if norm(coeff(dt/dt0-1))>del then pause,end
qi=[q(2,2) -q(1,2);-q(2,1) q(1,1)]/dt0;
if norm(coeff([p 0]*qi-[a b1]))>del then pause,end
 
b1=poly([4 1+.001],'z');del=10*norm(coeff(b1-b));
[p,q]=bezout(a,b1);
dt=q(1,1)*q(2,2)-q(1,2)*q(2,1);dt0=coeff(dt,0);
if norm(coeff(dt/dt0-1))>100000*%eps then pause,end
qi=[q(2,2) -q(1,2);-q(2,1) q(1,1)]/dt0;
if norm(coeff([p 0]*qi-[a b1]))>100000*%eps then pause,end
 
 
b1=poly([4 1+10000*%eps],'z');del=10*norm(coeff(b1-b));
[p,q]=bezout(a,b1);
dt=q(1,1)*q(2,2)-q(1,2)*q(2,1);dt0=coeff(dt,0);
if norm(coeff(dt/dt0-1))>del then pause,end
qi=[q(2,2) -q(1,2);-q(2,1) q(1,1)]/dt0;
if norm(coeff([p 0]*qi-[a b1]))>del then pause,end
//
z=poly(0,'z');
num = 0.99999999999999922+z*(-4.24619123578530730+..
      z*(10.0503215227460350+z*(-14.6836461849931740+..
      z*(13.924822877119892+z*(-5.63165998008533460+..
      z*(-5.63165998008530710+z*(13.9248228771198730+..
      z*(-14.683646184993167+z*(10.0503215227460330+..
      z*(-4.24619123578530910+z*(0.99999999999999989)))))))))));
den = -0.17323463717888873+z*(1.91435457459735380+..
      z*(-9.90126732768255560+z*(31.6286096652930410+..
      z*(-69.3385546880304280+z*(109.586435800377690+..
      z*(-127.516160100808290+z*(109.388684898145950+..
      z*(-67.92645394857864+z*(29.1602681026148110+..
      z*(-7.8212498781094952+z*(0.99999999999999989)))))))))));
//
[p,q]=bezout(num,den);del=1.d-4;
dt=q(1,1)*q(2,2)-q(1,2)*q(2,1);dt0=coeff(dt,0);
if norm(coeff(dt/dt0-1))>del then pause,end
qi=[q(2,2) -q(1,2);-q(2,1) q(1,1)]/dt0;
// JPC 
del=3*del
if norm(coeff([p 0]*qi-[num den]))>del then pause,end
if degree(p)>0 then pause,end
// une autre "solution" telle que l'erreur directe est petite mais l'erreur
// inverse grande
q1=[]
q1(1,1)=poly([0.011533588674 , 3.570224012502 , -28.78817740957 ,...
            102.9479419903, -210.8258579715 , 266.2028963639 ,...
           -207.427018299 , 92.74478640319, -18.5259652457],'z','c');
q1(1,2)=poly([0.000270220664 , -0.002465565223 , 0.010039706635 ,...
           -0.023913827007, 0.036387144032 , -0.036175176058 ,...
            0.022954475171 , -0.008514798968,  0.001417382492],'z','c');
q1(2,1)=poly([0.000639302018 , 20.502472606 , -26.66621972106 ,...
            39.74001534015,  -5.945830824342 , -7.973226036298 ,...
            39.84118622788 , -26.51337424414, 18.5259652457],'z','c');
q1(2,2) =poly( [0.001562930385 , -0.003589174974 , 0.005076237479 ,...
            -0.003483568682,  -0.000135940266 , 0.003651509121 ,...
            -0.005059502869 , 0.003447573440, -0.001417382492],'z','c');
p1 =poly( [0.011422839421 , -0.029264363070 , 0.030070175223 ,...
         -0.012596066108],'z','c');
 
//
//simplification
num =[0.03398330733500143,-0.20716057008572933,0.64660689206696986,...
     -1.97665462134021790,3.38751027286891300,-3.58940006392108120,...
      5.09956159043231680,5.2514918861834694,1.00000000000000020];
den = [0.03398330733500360,-0.20716057008816283,0.64660689204312,...
      -1.97665462079896660,3.38751027286891300,-3.58940006392108350,...
       5.09956159043232040,5.2514918861834712,1];
num=poly(num,'z','c');
den=poly(den,'z','c');
[p,q]=bezout(num,den);del=1.d-8;
dt=q(1,1)*q(2,2)-q(1,2)*q(2,1);dt0=coeff(dt,0);
if norm(coeff(dt/dt0-1))>del then pause,end
qi=[q(2,2) -q(1,2);-q(2,1) q(1,1)]/dt0;
if norm(coeff([p 0]*qi-[num den]))>del then pause,end
if degree(p)<8 then pause,end
