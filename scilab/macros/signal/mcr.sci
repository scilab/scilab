function [z,flag]=mcr(y,phi,m,lbd0,psi)
//<z,flag>=mcr(y,phi[,m[,lbd0[,psi]]])
//
// Resolution MC ou MCR pour l'identification
// du model d'un systeme sous forme d'entree-sortie :
//
// y(n) = phi'(n) . z + e(n)
//
// Estimation recursive de z minimisant le critere
// quadratique d'erreur entre l'entree relle y et son estime.
//
// y : vecteur reel ou complexe des donnees.
// phi : vecteur reel ou complexe des observations.
// psi : vecteur reel ou complexe des observations de la
//       variable instrumentale.
// m : denomination entiere de la methode MCR utilisee
//     = 0 MC  ordinaire
//     = 1 MCR multiplicatif simple.
//     = 2 MCR a facteur d'oubli constant.
//     = 3 MCR a facteur d'oubli exponentiel.
// lbd0 : initialisation du facteur d'oubli dans l'intervalle ]0,1[.
// z : vecteur parametre a estimer z.
// flag : test de convergence
//        = 1 convergence correcte
//        = 0 convergence forcee (/ au nombre d'echantillons disponibles)
//!
// Copyright INRIA
[o,i]=argn(0);
   if i < 2 then error(58); end;
//
// Controle sur le type des entrees
//
   if i >= 1 then
     if type(y) <> 1 then error(53,1); end;
     if i >= 2 then
       if type(phi) <> 1 then error(53,2); end;
       if i >= 3 then
         if type(m) <> 1 then error(53,3); end;
         if i >= 4 then
           if type(lbd0) <> 1 then error(53,4); end;
           if i >= 5 then
             if type(psi) <> 1 then error(53,5); end;
           end;
         end;
       end;
     end;
   end;
//
// Controle sur la validite des entrees
//
   [o,i]=argn(0);
   y=testvec(y,'l');
   select i
     case 2 then psi=phi; m=0; to=1;
     case 3 then
       psi=phi;
       select m
         case 0 then to=1;
         case 1 then to=1;
         case 2 then lbd0=0.8; to=lbd0;
         case 3 then lbd0=0.8; to=lbd0;
         else error(36,3);
       end;
     case 4 then
       psi=phi;
       to=lbd0;
       if to <= 0 then error(36,4); end;
       if to >  1 then error(36,4); end;
       if m <= 1 then error(36,3); end;
       if m > 3 then error(36,3); end;
     case 5 then
       K=maxi(size(y));
       sphi=size(phi);
       spsi=size(psi);
       if norm(sphi - spsi) <> 0 then error(89,2); end;
       if sphi(1,1) <> K then error(5); end;
       if spsi(1,1) <> K then error(5); end;
       to=lbd0;
       if to <= 0 then error(36,4); end;
       if to >  1 then error(36,4); end;
       if m == 0 then error(36,3); end;
       if m > 3 then error(36,3); end;
     else error(58), end;
//
// Initialisations
//
   K=maxi(size(y));
   tphi=size(phi);
   dim=tphi(1,2);
//
   if m==0 then
     mphi=phi' * phi;
     th=inv(mphi) * (phi' * y');
     flag=1;
   else
     P1p=1000 * norm(phi) * eye(dim,dim);
     th1p=0 * ones(dim,1);
     th  =0 * ones(dim,1);
     dis=1000;
     mu=1/to;
     s=1D-10;
//
     flag=1;
     n=1;
     z=[];
     while dis > s
       e=y(n) - phi(n,:) * th1p;
       num=P1p * psi(n,:)';
       den=(1/mu) + phi(n,:) * P1p * psi(n,:)';
       G=num/den;
       P= (1/to) * (eye(P1p) - G*phi(n,:)) * P1p;
       th= th1p + G * e;
       dis=abs(1 - norm(th1p)/norm(th));
       if n == K-1 then dis=0; flag=0; end;
       th1p=th;
       P1p=P;
       if m==3 then
         to=lbd0 * to + (1-lbd0);
         mu=1/to;
       end;
       n=n+1;
//       z=<z  th>;
     end;  // while
   end;
   z=th';
