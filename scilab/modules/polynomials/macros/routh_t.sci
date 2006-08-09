function r=routh_t(h,k)
// Copyright INRIA
//r=routh_t(h,k) computes routh table of denominator of the
//system described by transfer matrix SISO continue h with the
//feedback by the gain k
//If  k=poly(0,'k') we will have a polynomial matrix with dummy variable 
//k, formal expression of the Routh table.
//r=routh_t(d) computes Routh table of h :attention ! d=denom of system
//!
//modified 15/10/99, by Lucien.Povy@eudil.fr to get a good table
[lhs,rhs]=argn(0);
h1=h(1);
if rhs==2 then
  if type(h)<>16 then
    error('first argument must be rational')
  end;
  if h1(1)<>'r' then
    error('first argument must be rational')
  end;
  [n,d]=h(2:3)
  nd=maxi([degree(d) degree(n)])+1;
  cod=coeff(d,0:nd-1);//coeff du denominateur
  con=coeff(n,0:nd-1);//coeff du numerateur
  cobf=cod+k*con //coeff de la boucle fermee
else
  if type(h)>2 then error('argument must be polynomial'),end
  nd=degree(h)+1;
  cobf=coeff(h,0:nd-1)
end;
//
r1=cobf(nd:-2:1);
r2=cobf(nd-1:-2:1);
ncol=length(r1);
if length(r2)<>ncol then r2=[r2,0],end
r=[r1;r2]

if ncol<2 then r=[],return,end;
if rhs==2 then
  for i=3:nd,
    r(i,1:ncol-1)=[r(i-1,1),-r(i-2,1)]*[r(i-2,2:ncol);r(i-1,2:ncol)]
  end;
else
  for i=3:nd,
    if r(i-1,1)==0 then 
      r(i,1:ncol-1)=[1.,-r(i-2,1)*%inf]*[r(i-2,2:ncol);r(i-1,2:ncol)]
    else
      r(i,1:ncol-1)=[1.,-r(i-2,1)/r(i-1,1)]*[r(i-2,2:ncol);r(i-1,2:ncol)]
    end
  end;
end;
endfunction
