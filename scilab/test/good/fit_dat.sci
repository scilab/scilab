function [p,err]=fit_dat(G,p0,Z,W,pmin,pmax,DG)
//
//         [p,err]=fit_dat(G,p0,Z [,W] [,pmin,pmax] [,DG])
//
//         Function used for fitting data to a model.
// For a given function G(p,z), this function finds the best vector 
// of parameters p for approximating G(p,z_i)=0 for a set of measurement
// vectors z_i. Vector p is found by minimizing
//    G(p,z_1)'WG(p,z_1)+G(p,z_2)'WG(p,z_2)+...+G(p,z_n)'WG(p,z_n)
//
//      G: Scilab function (e=G(p,z), e: nex1, p: npx1, z: nzx1)
//     p0: initial guess (size npx1)
//      Z: matrix [z_1,z_2,...z_n] where z_i (nzx1) is the ith measurement
//      W: weighting matrix of size nexne (optional; default 1)
//   pmin: lower bound on p (optional; size npx1)
//   pmax: upper bound on p (optional; size npx1)
//     DG: partial of G wrt p (optional; S=DG(p,z), S: nexnp)
//
//                     Examples
//
//deff('y=FF(x)','y=a*(x-b)+c*x.*x')
//X=[];Y=[];
//a=34;b=12;c=14;for x=0:.1:3, Y=[Y,FF(x)+100*(rand-.5)];X=[X,x];end
//Z=[Y;X];
//deff('e=G(p,z)','a=p(1),b=p(2),c=p(3),y=z(1),x=z(2),e=y-FF(x)')
//[p,err]=fit_dat(G,[3;5;10],Z)
//xset('window',0)
//xbasc();
//plot2d(X',Y',-1) 
//plot2d(X',FF(X)',5,'002')
//a=p(1),b=p(2),c=p(3);plot2d(X',FF(X)',12,'002')
//
//a=34;b=12;c=14;
//deff('s=DG(p,z)','y=z(1),x=z(2),s=-[x-p(2),-p(1),x*x]')
//[p,err]=fit_dat(G,[3;5;10],Z,DG)
//xset('window',1)
//xbasc();
//plot2d(X',Y',-1) 
//plot2d(X',FF(X)',5,'002')
//a=p(1),b=p(2),c=p(3);plot2d(X',FF(X)',12,'002')
//
//
// Copyright INRIA
[lhs,rhs]=argn(0)
boun=%f
if rhs==3 then 
  W=1;GR=%f  //Jacobian not provided
elseif rhs==4 then
  if type(W)==11|type(W)==13 then 
    DG=W;W=1;GR=%t  //Jacobian provided
  else
    GR=%f  //Jacobian not provided
  end
elseif rhs==5 then
  if type(pmin)==1 then 
    pmax=pmin,pmin=W,W=1,GR=%f,boun=%t
  else 
    DG=pmin;GR=%t
  end
elseif rhs==6 then
  boun=%t
  if type(pmax)==1 then
    GR=%f
  else
    DG=pmax,pmax=pmin,pmin=W,W=1,GR=%t
  end
elseif    rhs==7 then
  GR=%t;
else
  error('wrong number of arguments')
end
if size(W,1)~=size(W,2) then error('Weighting matrix must be square');end
nz=size(Z,2);mz=size(p0,'*');
deff('e=GG(p,Z)','e=0;for i=1:nz,e=e+G(p,Z(:,i))''*W*G(p,Z(:,i)),end')
if ~GR then
  deff('g=DGG(ps,Z)',['g=[]';
      'nrm=norm(ps);if nrm<1 then nrm=1,end';
      '[d,nu]=colcomp(rand(mz,mz));d=10*%eps*nrm*d';
      'for v=d,e=GG(ps+v),g=[g,e-f],end;g=g/d;'])
else
  deff('g=DGG(ps,Z)','g=0;for i=1:nz,g=g+2*DG(ps,Z(:,i))''*W*G(ps,Z(:,i)),end')
end
deff('[f,g,ind]=costf(p,ind)',['f=GG(p,Z);';
    'g=DGG(p,Z);'])

if ~boun then
  [err,p]=optim(costf,p0)
else
  if or(p0<pmin)|or(p0>pmax) then 
    error('initial guess not feasible')
  end
  [err,p]=optim(costf,'b',pmin,pmax,p0)
end
  
endfunction
