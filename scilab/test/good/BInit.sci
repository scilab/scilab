function []=BInit()
// Macro qui initialise les donnees du
// td3
//!
// Copyright INRIA
n1=10;
n2=10;
n3=10;
[n1,n2,n3]=resume(n1,n2,n3);


endfunction
function [VB,feed]=Bellman(n1,n2,n3,uvect)
//  Fonction de Bellman 
//  etat de dimension (n1,n2) temp n3 
//   -- n3-1
//   \
//   /        L_b(x(k),u,k) +VB_f(x1,x2,n3)
//   -- k=1
//  avec : x(k+1)=f_b(x(k),u)
//
// la fonction gain instantane L_b(x,u,k).
// VB_f(x,k) est un cout final (i.e valeur de Bellamn en fin de temps)
// uvect est un vecteur donnant les commandes possibles discr\`etes 
// utilis\'e dans f_b et L_b.
//!
usize=prod(size(uvect))
xgr=1:n1
ygr=1:n2
ietatmax=n1*n2
// tableau ou l'on stocke la fonction de Bellman
VB=0*ones(ietatmax,n3);
// tableau ou l'on stocke le feedback u(x,t)
feed=0*ones(ietatmax,n3);
// calul de la fonction de Bellman au  temps final
penal=10000;
for i=1:n1
  for j=1:n2
	cout(i+n1*(j-1),n3)=VB_f(i,j,n3)
end
end
// calcul retrograde de la fonction de Bellman et
// du controle optimal au temps temp par l'equation de Bellman
for temp=n3:-1:2,
  loc=list();
  for i=1:n1
    for j=1:n2
      loc=ones(1,usize)
      for l=1:usize,
	[i1,j1]=f_b(i,j,uvect(l),temp)
	loc(l)=VB(i1+n1*(j1-1),temp)+L_b(i,j,uvect(l),temp-1),
      end;
      [mm,kk]=maxi(loc),
    VB(i+n1*(j-1),temp-1)=mm;
    feed(i+n1*(j-1),temp-1)=kk;
end
end
end

endfunction
function [y]=L_b(i,j,u,t)
y=0.5*u**2 -(i-5)**2 - (j-5)**2

endfunction
function [y]=VB_f(i,j,t)
y= (i-10)**2 +(j-10)**2

endfunction
function [i1,j1]=f_b(i,j,u,t)
i1=mini(maxi(i+u,1),n1)
j1=mini(maxi(i+j-2*u,1),n2)

endfunction
function [x1opt,x2opt]=OptTraj(i0,j0,feed)
// optimal Trajectory for initial point (i0,j0)
//!
x1opt=i0*ones(1,n3)
x2opt=j0*ones(1,n3)
uopt=0*ones(1,n3)
xset("window",0)
for i=2:(n3),uopt(i-1)=uvect(feed(x1opt(i-1)+n1*(x2opt(i-1)-1),i-1));
	write(%io(2),uopt(i-1))
	[xx,yy]=f_b(x1opt(i-1),x2opt(i-1),uopt(i-1),i-1);
	x1opt(i)=xx;x2opt(i)=yy
end
plot2d2("gnn",[1:(n3)]',[x1opt]',[2],"111",...
       "trajectoire optimale",...
       [1,1,n3,n1]);
xset("window",1);
plot2d2("gnn",[1:(n3)]',[x2opt]',[2],"111",...
       "trajectoire optimale",...
       [1,1,n3,n2]);
xset("window",2);
umin=min(uopt)
umax=max(uopt)
plot2d2("gnn",[1:(n3-1)]',uopt(1:n3-1)',[1,-4],"111",...
       "commande optimale",[1,umin-1,n3,umax+1]);
endfunction
