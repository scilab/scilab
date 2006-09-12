function [lambda,facpr,comprinc]=pca(x,N)
//
//This  function performs  several  computations known  as
//"principal component  analysis". It includes  drawing of
//"correlations circle",  i.e. in the  horizontal axis the
//correlation   values  r(c1;xj)   and  in   the  vertical
//r(c2;xj). It is an extension of the pca function.
//
//The  idea  behind this  method  is  to  represent in  an
//approximative  manner a  cluster of  n individuals  in a
//smaller  dimensional subspace.  In order  to do  that it
//projects the cluster onto a subspace.  The choice of the
//k-dimensional projection subspace is  made in such a way
//that  the distances  in  the projection  have a  minimal
//deformation: we are looking for a k-dimensional subspace
//such that the squares of the distances in the projection
//is  as  big  as  possible  (in  fact  in  a  projection,
//distances can only stretch).  In other words, inertia of
//the projection  onto the k dimensional  subspace must be
//maximal.
//
//x is a nxp (n  individuals, p variables) real matrix.
//
//lambda is a px2 numerical matrix. In the first column we
//find the  eigenvalues of V,  where V is  the correlation
//pxp matrix  and in the  second column are the  ratios of
//the   corresponding   eigenvalue   over   the   sum   of
//eigenvalues.
//
//facpr  are  the principal  factors:  eigenvectors of  V.
//Each  column is an  eigenvector element  of the  dual of
//R^p. Is an orthogonal matrix.
//
//comprinc  are  the  principal components.   Each  column
//(c_i=Xu_i)  of  this  nxn  matrix  is  the  M-orthogonal
//projection of individuals onto principal axis.  Each one
//of this columns is a linear combination of the variables
//x1,  ...,xp   with  maximum  variance   under  condition
//u'_iM^(-1)u_i=1.
//
//Verification: comprinc*facpr=x
//
//References: Saporta, Gilbert, Probabilites,  Analyse des
//Donnees et Statistique, Editions Technip, Paris, 1990.
//
//author: carlos klimann
//
//date: 2002-02-05
//commentary fixed 2003-19-24
//
  [lhs,rhs]=argn(0)
  if  rhs==0 then
    error('pca must have at least one parameter'), 
  end
  if x==[] then
    lambda=%nan
    facpr=%nan
    comprinc=%nan, 
    return, 
  end
  [rowx colx]=size(x)
  if rhs==2 then
    if size(N,'*')<>2 then error('Second parameter has bad dimensions'), end,
    if max(N)>colx then disp('Graph demand out of bounds'), return, end
  else 
    N=[1 2],
  end
  xbar=sum(x,'r')/rowx
  y=x-ones(rowx,1)*xbar
  std=(sum(y .^2,'r')) .^ .5 
  V=(y'*y) ./ (std'*std)
  [lambda facpr]=bdiag(V,(1/%eps))
  [lambda order]=sort(diag(lambda))
  lambda(:,2)=lambda(:,1)/sum(lambda(:,1))
  facpr=facpr(:,order)
  comprinc=x*facpr
  
  
  w=winsid();if w==[] then w=0, else w=max(w)+1,end
  fig1=scf(w)

  rc= (ones(colx,1)* sqrt((lambda(N,1))')) .* facpr(:,order(N)) 
  
  rango=rank(V)
  ra=[1:rango]'
  if rango<=1 then return, end
  plot2d(-rc(ra,1),rc(ra,2),style=-10)
  legend('(r(c1,xj),r(c2,xj)')
  ax=gca();ax.x_location="middle";ax.y_location = "middle";
  blue=color('blue')
  for k=1:rango,
    xstring(rc(k,1),rc(k,2),'X'+string(k)),
    e=gce();e.foreground=blue;
  end
  title(' -Correlations Circle- ')
  fig2=scf(w+1);
  plot2d3([0;ra;rango+1]',[0; lambda(ra,2);0])
  plot2d(ra,lambda(ra,2),style=9)
  ax=gca(); ax.grid=[31 31]
  plot2d3([0;ra;rango+1]',[0; lambda(ra,2);0])
  plot2d(ra,lambda(ra,2),style=9)
  for k=1:rango,
    xstring(k,0,'l'+string(k)),
    e=gce();e.font_style=1
  end
  title(' -Eigenvalues (in percent)- ')
  ylabel('%')

endfunction


