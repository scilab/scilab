function [s]=sincd(n,flag)
//<s>=sincd(n,flag)
//macro which calculates the function Sin(N*x)/Sin(x)
//  n    : value of N in the preceding function
//  flag : flag=1 : the function is centered around the origin
//       : flag=2 : the function is delayed by %pi/(2*n)
//  s    : vector of values of the function on a dense
//       : grid of frequencies
//!
//author: G. Le Vey  Date: 1 Febr 1989
// Copyright INRIA

   npt=4*n;
   pas=%pi/npt;
   om=0:pas:%pi;
   eps=(-1)**(n-1);
   select flag
   case 1,
   s1=sin(n*om);s2=sin(om);
   s1(1)=n;s2(1)=1;s1(npt+1)=n*eps;s2(npt+1)=1;
   s=s1./s2;
   s=[s(npt+1:-1:2) s];
   s=s/n;
   case 2,
   om=om-ones(om)*%pi/2/n;
   s1=sin(n*om);
   s2=sin(om);
   s1(3)=n;s2(3)=1;
   s=s1./s2;
   s=[eps*s s(2:npt+1)];
   s=s/n;
   else,error('flag must be equal to 1 or 2')
   end;
endfunction
