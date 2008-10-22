function [xi,xa,np]=graduate( xmi, xma,n1,n2)
// graduate - axis pretty graduations
//%Syntax
// [xa,xi,np]=graduate( xma, xmi,n1,n2)	
// [xa,xi,np]=graduate( xma, xmi)	
//%Parameters
// xmi, xma : real scalars
// n1 , n2  : integer scalars default values 3,10
// xi , xa  : real scalars
// np       :integer scalar
//%Description
// graduate looks for the mimimum interval [xi,xa] and a number of tics np
// such that:
//           xi <= xmi <= xma <= xa
//           xa - xi / np  =  k(10**n)  k in [1 3 5]   for an integer n
//           n1 <= np <= n2
//%Exemple
//  y=0:0.33:145.78
//  xbasc();plot2d1('enn',0,y)
//  [ymn,ymx,np]=graduate(mini(y),maxi(y))
//  rect=[1,ymn,prod(size(y),ymx];
//  xbasc();plot2d1('enn',0,y,-1,'011',' ',rect,[10,3,10,np])

// Copyright INRIA
//!
//%Origine 
// S Steer INRIA 1992

kadm=[1,2,5];nadm=prod(size(kadm))

// test
// ----
//
[lhs,rhs]=argn(0)
if rhs <4 then
  n1=3
  n2=10
end
if  n1 == 0  &  n2 == 0  then
   k1 = 1
   k2 = 1
else
   k1 = mini ( abs(n1) , abs(n2) )
   k1 = maxi (    1    ,     k1  )
   k2 = maxi ( abs(n1) , abs(n2) )
end
if xma == xmi then
   if xma==0 then
     xma=0.1;xmi=-0.1
   else
     xma=xma+xmi/10
     xmi=xmi-xmi/10
   end
end

xx0 = maxi ( xma , xmi )
xx1 = mini ( xma , xmi )
del=abs(xx1-xx0)
if abs(xx0-xx1)<=1d-6*maxi(xx0,xx1) then
  xa = xma
  xi = xmi
  np=1
  return
end

//
// boucle sur les pas possibles
// ----------------------------
//
for npi = k1:k2

//
// recherche de l'intervalle [ x1 , x0 ] tel que :
//           x1 < xmi < xma < x0
//           x0 - x1 / npi  =  k.10**n   k = 1,.,9   n  entier
//
//
// recherche du pas
// ----------------
// il est compris entre  10**ipa-1  et  10**ipa
//
  if xx0*xx1<0 then
    pas=maxi(abs([xx0 xx1])/npi)
  else
    pas = (xx0-xx1)/npi
  end
  ipa = int(log(pas)/log(10))
  if pas<1 then ipa = ipa - 1,end

  pa2 = 10**ipa
//
  ik=find(pas<=kadm*pa2)
  if ik==[] then
    pa2 = 10.0d+00 * pa2
    ipa = kadm(1)
    pa1=ipa*pa2
  else
    ipa=kadm(ik(1))
    pa1=ipa*pa2
  end   
  while %t
//
// recherche des extremites
// ------------------------
//
    if xx1*xx0<0 then
        x1 = xx1/pa1
        np1=int(x1)
        x1=np1*pa1
        if x1>xx1 then x1=x1-pa1,end
    else
      x1 = xx1/pa2
      np1=int(x1)
      x1=np1*pa2
      if x1>xx1 then x1=x1-pa1,end
    end
    x0 = x1+npi*pa1


//
// test
// ----
//
    if x0<xx0 then
      ik=find(kadm==ipa)
      if ik<nadm then 
        ipa = kadm(ik+1)
        pa1 = ipa * pa2
      else
        ipa = kadm(1)
        pa1 = 10.0d+00 * pa2
        pa2 = pa1
      end
    else
      break
    end
  end
  if npi==k1 then 
    xl=x0-x1
    xa=x0
    xi=x1
    np=k1
  else
    if  (x0-x1)< xl then
      np = npi
      xl = x0 - x1
      xa = x0
      xi = x1
    end
  end
end
endfunction
