function z=narsimul(x1,x2,x3,x4,x5,x6,x7,x8)
//function z=narsimul(a,b,d,sig,u,up,yp,ep)
//   or 
// function z=arsimul(ar,u,up,yp,ep)
//
// Armax simulation  using rtitr 
//   A(z)= Id+a1*z+...+a_r*z^r;  ( r=0  => A(z)=Id)
//   B(z)= b0+b1*z+...+b_s z^s;  ( s=-1 => B(z)=0)
//   D(z)= Id+d1*z+...+d_t z^t;  ( t=0  => D(z)=Id)
// z et e sont a valeurs dans dans R^n et u dans R^m
// Auteur : J-Ph. Chancelier ENPC Cergrene
//
// Copyright Enpc

[lhs,rhs]=argn(0)
// switch to ar representation 
if type(x1)<>15&type(x1)<>16 then 
   if rhs < 5, write(%io(2),'arsimul: error expecting at least 5 arguments');
	return;end;
   ar=armac(x1,x2,x3,size(x1,'r'),size(x5,'r'),x4);
   if rhs==5,z=narsimul(ar,x5);return;end
   if rhs==6,z=narsimul(ar,x5,x6);return;end
   if rhs==7,z=narsimul(ar,x5,x6,x7);return;end;
   if rhs==8,z=narsimul(ar,x5,x6,x7,x8);return;end;
end
// Here the call is always arsimul(ar,....)
a=x1('a');b=x1('b');d=x1('d');sig=x1('sig');
u=x2;
[mmu,Nu]=size(u);
if mmu<>x1('nu') then 
  write(%io(2),'number of rows of u are incompatible with arma object');
  return;
end;
// dimensions 
   [al,ac]=size(a);adeg=int(ac/al);
   [dl,dc]=size(d);ddeg=int(dc/dl);
   [bl,bc]=size(b);[mmu,Nu]=size(u);bdeg=int(bc/mmu);
// quelques tests a faire : bl=al=dl,
// <i>deg*<i>l=<i>c, pour i=a,b,d
//
// On genere d'abord y(k) solution de : A(z^-1)y(k)=B^(z-1)u(k)
s=poly(0,'s');
// Build polynomial matrix A(s)
mata= a*((s.^[adeg-1:-1:0]).*.eye(al,al))';
// Build polynomial matrix B(s)
matb= b*((s.^[bdeg-1:-1:0]).*.eye(mmu,mmu))';
// 
num=matb*s**(adeg-1)
den=mata*s**(bdeg-1);
// Using past values 
// yp doit etre de taille (al,(adeg-1))
// up doit etre de taille (al,(bdeg-1))
// ep doit etre de taille (al,(adeg-1))
//
if rhs <=5,
   up=0*ones(mmu,(bdeg-1));
else
   if size(up)<>[mmu,(bdeg-1)],
    write(%io(2)," up=[u(0),u(-1),..,] must be of dimension ("...
    +strin(mmu)+','+string(bdeg-1));
    return;end
end
if rhs <=6,
   yp=0*ones(al,(adeg-1));
else
  if size(yp)<>[al,(adeg-1)]
    write(%io(2)," yp=[y(0),y(-1),..,] must be of dimension ("...
    +strin(al)+','+string(adeg-1));
    return;end
end
if rhs <=7,
   ep=0*ones(al,(ddeg-1));
else
  if size(ep)<>[al,(ddeg-1)]
    write(%io(2)," ep=[e(0),e(-1),..,] must be of dimension ("...
    +strin(al)+','+string(ddeg-1));
    return;end
end;
// 
degnum=maxi(degree(den));
yp=[0*ones(al,degnum+1-adeg),yp(:,(adeg-1):-1:1)];
up=[0*ones(mmu,degnum+1-bdeg),up(:,(bdeg-1):-1:1)];
y=rtitr(num,den,u,up,yp);
// truncate the solution to only keep y_1,..y_Nu 
// (if b0=0 rtitr computes y_{Nu+1})
y=y(:,1:Nu);
// Generate bru such that A(z^-1)bru= D(z^-1) sig*e(t)
// Build polynomial matrix D(s)
matd= d*((s.^[ddeg-1:-1:0]).*.eye(al,al))';
num=matd*s**(adeg-1)
den=mata*s**(ddeg-1);
degnum=maxi(degree(den));
ep=[0*ones(al,degnum+1-ddeg),ep(:,(ddeg-1):-1:1)];
// Normal noise 
br=sig*rand(al,Nu,'normal')
bru=rtitr(num,den,br,ep,0*ones(ep));
// z(k) = y(k) + bru(k)
z=y+bru(:,1:Nu);
endfunction
