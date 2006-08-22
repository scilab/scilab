function [s]=covar(x,y,fre)
//
//This function computes the  covariance  of two variables x
//and  y where x  is a numerical vector of  length n, y is a
//numerical   vector  of length m and    fre is an  array of
//dimensions  nxm.   In fre  the  element  of indices  (i,j)
//corresponds to the number or frequences of x_i&y_j.
//
//References: Wonacott,  T.H. & Wonacott, R.J.; Introductory Statistics,
//J.Wiley & Sons, 1990.
//
//author: carlos klimann
//
//date: 2000-04-14
//
//small correction (bug number 1072) date : 8 nov 2004
//
  if x==[] | y==[] then s=%nan; return, end
  [lhs,rhs]=argn(0)
  if rhs <> 3 then error('covar requires three arguments.'), end
  [lfre cfre]=size(fre);
  [lx cx]=size(x)
  [ly cy]=size(y)
  if lx<>1 & cx<>1 then error('First parameter must be a vector.'), end
  if ly<>1 & cy<>1 then error('Second parameter must be a vector.'), end
  fre(isnan(fre))=0
  lx=lx*cx;
  cy=ly*cy;
  if lx<>lfre then error('inconsistent dimensions'), end
  if cy<>cfre then error('inconsistent dimensions'), end
  x=matrix(x,lx,1);
  y=matrix(y,1,cy);
  fr=fre/sum(fre)
  s=sum(((x-sum(x.*sum(fr,'c')))*ones(1,cy)) ..
	.* (fr) .* (ones(lx,1)*(y-sum(y.* sum(fr,'r')))))
endfunction
