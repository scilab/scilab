function rho=correl(x,y,fre)
//
//This function computes  the correlation of two variables x
//and y where x is  a numerical vector of length  n, y is  a
//numerical  vector  of length  m  and fre   is an array  of
//dimensions nxm. In    fre  the element  of   indices (i,j)
//corresponds to the number or frequences of x_i&y_j.
//
//References: Wonacott,  T.H. & Wonacott, R.J.; Introductory Statistics,
//J.Wiley & Sons, 1990.
//
//author: carlos klimann
//
//date: 2000-02-06
//
  if x==[] | y==[] then s=%nan; return, end
  [lhs,rhs]=argn(0)
  if rhs <> 3 then error('correl requires three arguments.'), end
  [lfre,cfre]=size(fre);
  [lx,cx]=size(x)
  [ly,cy]=size(y)
  if lx<>1 & cx<>1 then error('First argument must be a vector.'), end
  if ly<>1 & cy<>1 then error('Second argument must be a vector.'), end
  lx=lx*cx;
  cy=ly*cy;

  if cy<>cfre then error('inconsistent dimensions'), end
  if lx<>lfre then error('inconsistent dimensions'), end
  x=matrix(x,lx,1);
  y=matrix(y,1,cy);
  fr=fre/sum(fre)
  px=sum(fr,'c')
  py=sum(fr,'r')
  mx=sum(x .* px)
  my=sum(y .* py)
  sx=sqrt(sum(((x-mx).^2).*px))
  sy=sqrt(sum(((y-my).^2).*py))
  sxy=sum(((x-mx)*ones(1,cy)).*fr.*(ones(lx,1)*(y-my)))
  rho=sxy/(sx*sy)
endfunction
