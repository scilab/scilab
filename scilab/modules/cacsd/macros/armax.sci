function [archap,la,lb,sig,resid]=armax(r,s,y,u,b0f,prf)
//[la,lb,sig,resid]=armax(r,s,y,u,[b0f,prf])
// Identification ARX
// Calcule les coefficients d'un ARX n-dimensionnel
//   A(z^-1)y= B(z^-1)u + sig*e(t)
//           e(t) est un bruit blanc de variance Id n-dimenssionnel
//           sig: est une matrice (n,n)
//           A(z)= 1+a1*z+...+a_r*z^r; ( r=0 => A(z)=1)
//           B(z)= b0+b1*z+...+b_s z^s ( s=-1 => B(z)=0)
// Methode :
//     Cfre : Eykhoff (trends and progress in system identification) page 96
//     En introduisant z(t)=[y(t-1),..,y(t-r),u(t),...,u(t-s)] et
//     coef= [-a1,..,-ar,b0,...,b_s]
//     y(t)= coef* z(t) + sig*e(t) et l'algorithme consiste
//     a trouver coef qui minimise  sum_{t=1}^N ( [y(t)- coef'z(t)]^2)
//
// Entree :
//     y : serie de sortie y(ny,n); ny: dimension,
//                                  n : taille de l'echantillon
//     u : serie d'entree; u(nu,n); nu: dimension,
//                                  n : taille de l'echantillon
//     r et s : les ordres d'autoregression r >=0 et s >=-1
//     b0f : est un parametre optionnel. Par defaut il vaut
//         0, et signifie qu'il faut identifier b0. Si on lui
//         donne la valeur 1, alors b0 est suppose valoir zero et
//         n'est pas identifie.
//     prf : parametre optionnel controlant le display
//         si prf=1, un display est donne (c'est la valeur par defaut)
// Sortie :
//     la est la liste list(a,a+eta,a-eta);(eta : ecart type estime)
//        a=[Id,a1,a2,...,ar] ai(ny,ny)
//     lb est la liste list(b,b+etb,b-etb);(etb : ecart type estime)
//        b=[b0,.....,b_s] bi(nu,nu)
//     sig est l'ecart type estime du bruit
//     et resid=[ sig*e(t0),....]; t0=maxi(maxi(r,s)+1,1));
//
// Exemple :
//     taper [a,b,sig,resid]=armax(); pour voir un exemple
//     en dimension 1.
// Auteur: J-Ph. Chancelier ENPC Cergrene
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==0,write(%io(2),"/ / y_t = 0.2*u_{t-1}+0.01*e(t)");
        write(%io(2)," rand(''normal''); u=rand(1,1000);");
        write(%io(2)," y=arsimul([1],[0,0.2],1,0.01,u);");
        write(%io(2)," [archap,a,b,sig,resid]=armax(0,1,y,u)");
        write(%io(2),"/ / we must find a=1,b=[0,0.2]''");
        rand('normal'),u=rand(1,1000);
        y=arsimul([1],[0,0.2],1,0.01,u);
        [archap,la,lb,sig,resid]=armax(0,1,y,u,1);
        return
end
if rhs<=5,prf=1;end
if rhs<=4,b0f=0;end
[ny,n2]=size(y)
[nu,n2u]=size(u)
// calul de la matrice zz telle que
// zz(:,j)=[ y(t-1),...,y(t-r),u(t),...,u(t-s)]', avec  t=t0-1+j
// on peut calcule zz a partir de t=t0;
 t0=maxi(maxi(r,s)+1,1);
 if r==0;if s==-1;write(%io(2),"if  r==0 and s==-1 there''s nothing to identify");
 return
 end;end
 z=[];
 if r>=1;for i=1:r,z=[z ; y(:,t0-i:(n2-(i)))];end;end
 if s>=-1;for i=b0f:s,z=[z ; u(:,t0-i:(n2-(i)))];end;end
 zz= z*z';
 zy= z*y(:,t0:n2)';
// Test de rang
 [nzl,nzc]=size(zz);
 k=rank(zz);
if k<>nzl then
  write(%io(2),"Warning: z.z'' is numerically singular");
end;
 pv=pinv(zz);
 coef=(pv*zy)';
//le bruit residuel
 resid=y(:,t0:n2) - coef*z;
// la variance du bruit residuel
 sig2= resid*resid'/(n2-t0+1)
// l'ecart type
 sig=sqrtm(sig2);
 a=[eye(ny,ny),-coef(:,1:r*ny)];
 if b0f==0 then
   b=coef(:,r*ny+1:(s+1)*nu+r*ny);
 else 
   b=[0*ones(ny,nu),coef(:,r*ny+1:r*ny+s*nu)];
 end
// Pour les systemes SISO on rajoute les ecarts types des estimateur
// cela reste a faire pour les MIMO
 if ny == 1,
   dve=sqrt(diag(sig*pv,0))';
   la=list(a,a+[0,dve(1:r)],a-[0,dve(1:r)]);
   if b0f==0 then
     lb=list(b,b+dve(r+1:r+s+1),b-dve(r+1:r+s+1)),
   else
     lb=list(b,b+[0,dve(r+1:r+s)],b-[0,dve(r+1:r+s)]);
   end
 else 
   la=a;lb=b;
 end
//si prf vaut 1 on donne un display
archap=armac(a,b,eye(ny,ny),ny,nu,sig);

if prf==1;
   if ny==1,
     [nla,nca]=size(la(2));
     write(%io(2),"  Standard deviation of the estimator a :");
     form_a='(5x,'+string(nca)+'(f7.5,1x))';
     write(%io(2),la(2)-a,form_a);
     if nu<>0 then 
       write(%io(2),"  Standard deviation of the estimator b :");
       [nlb,ncb]=size(lb(2));
       write(%io(2),lb(2)-b,'(5x,'+string(ncb)+'(f7.5,1x))');
     end 
   end
end
endfunction
