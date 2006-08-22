function [r]=mvcorrel(x)
//
//This function  computes r, the matrix  of correlation of
//the "tableau" x (x is  a numerical matrix mxn) who gives
//the values  of n variables for m  individuals: the (i,j)
//coefficient              of             r             is
//v(i,j)=E(xi-xibar)(xj-xjbar)/(stdi*stdj), where E is the
//first  moment of a  variable, xi  is the  i-th variable,
//xibar the mean of the  xi variable and stdi the standard
//deviation of the i-th variable.
//
//References: Saporta, Gilbert, Probabilites,  Analyse des
//Donnees et Statistique, Editions Technip, Paris, 1990.
//
//author: carlos klimann
//
//date: 2001-09-27
//
  if x==[] then s=%nan; return, end
  [lhs,rhs]=argn(0)
  if rhs <> 1 then error('mvcorrel requires only one argument.'), end
  [lx cx]=size(x)
  if lx==1 then r=zeros(lx,cx), return, end
  xbar=sum(x,'r')/lx
  r=x-ones(lx,1)*xbar
  std=(sum(r .^2,'r')) .^ .5
  r=(r'*r) ./ (std'*std)
endfunction
