function [p,err]=datafit(imp,G,varargin)
//
//         [p,err]=datafit([imp,] G [,DG],Z [,W],...)
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
//      W: weighting matrix of size nexne (optional)
//     DG: partial of G wrt p (optional; S=DG(p,z), S: nexnp)
//
//                     Examples
//
//deff('y=FF(x)','y=a*(x-b)+c*x.*x')
//X=[];Y=[];
//a=34;b=12;c=14;for x=0:.1:3, Y=[Y,FF(x)+100*(rand()-.5)];X=[X,x];end
//Z=[Y;X];
//deff('e=G(p,z)','a=p(1),b=p(2),c=p(3),y=z(1),x=z(2),e=y-FF(x)')
//[p,err]=datafit(G,Z,[3;5;10])
//xset('window',0)
//xbasc();
//plot2d(X',Y',-1) 
//plot2d(X',FF(X)',5,'002')
//a=p(1),b=p(2),c=p(3);plot2d(X',FF(X)',12,'002')
//
//a=34;b=12;c=14;
//deff('s=DG(p,z)','y=z(1),x=z(2),s=-[x-p(2),-p(1),x*x]')
//[p,err]=datafit(G,DG,Z,[3;5;10])
//xset('window',1)
//xbasc(); 
//plot2d(X',Y',-1) 
//plot2d(X',FF(X)',5,'002')
//a=p(1),b=p(2),c=p(3);plot2d(X',FF(X)',12,'002')
//
//
// Copyright INRIA

[lhs,rhs]=argn(0)

if type(imp)<>1 then
  varargin(0)=G
  G=imp
  imp=0
end

if type(G)==15 then
  Gparams=G;Gparams(1)=null();
  G=G(1)
else
  Gparams=list()
end

  
DG=varargin(1)
if type(DG)==10|type(DG)==11|type(DG)==13 then 
  GR=%t  //Jacobian provided
  varargin(1)=null()
elseif type(DG)==15 then 
  error('Jacobian cannot be a list, parameters must be set in G')
else
  GR=%f
end

Z=varargin(1);
varargin(1)=null()
if type(Z)<>1 then 
  error('datafit : wrong measurement matrix')
end

nv=size(varargin)
if nv>=1 then
  if size(varargin(1),2)==1 then // p0 ou 'b'
    W=1
  else
    W=varargin(1);varargin(1)=null()
    if size(W,1)~=size(W,2) then 
      error('Weighting matrix must be square');
    end
  end
end  
if type(varargin(1))==1 then // p0
  p0=varargin(1)
else
  p0=varargin(4)
end

[mz,nz]=size(Z);np=size(p0,'*');

if type(G)==10 then  //form function to call hard coded external
  if size(Gparams)==0 then 
    error('With hard coded function, user must give output size of G'),
  end
  m=Gparams(1);Gparams(1)=null()
  
  // foo(m,np,p,mz,nz,Z,pars,f)
  deff('f=G(p,Z)','f=call('''+G+''','+..
      'm,1,''i'',np,2,''i'',p,3,''d'',mz,4,''i'',nz,5,''i'',Z,6,''d'','+..
      'pars,7,''out'',['+string(m)+',1],8,''d'')')
  
  pars=[];
  for k=1:size(Gparams)
    p=Gparams(k)
    pars=[pars;p(:)]
  end
  Gparams=list()
end

if type(DG)==10 then //form function to call hard coded external
  // dfoo(m,np,p,mz,nz,Z,pars,f)
  deff('f=DG(p,Z)','f=call('''+DG+''','+..
      'm,1,''i'',np,2,''i'',p,3,''d'',mz,4,''i'',nz,5,''i'',Z,6,''d'','+..
      'pars,7,''out'',['+string(m)+','+string(np)+'],8,''d'')')
end


// form square cost gradient function DGG

if Gparams==list() then
  GP   = 'G(p,Z(:,i))'
  GPPV = 'G(p+v,Z(:,i))'
  DGP  = 'DG(p,Z(:,i))'
else
  GP   = 'G(p,Z(:,i),Gparams(:))'
  GPPV = 'G(p+v,Z(:,i),Gparams(:))'
  DGP  = 'DG(p,Z(:,i),Gparams(:))'  
end
  
if ~GR then // finite difference
  DGG=['g=0*p';
       'pa=sqrt(%eps)*(1+1d-3*abs(p))'
       'f=0;'
       'for i=1:'+string(nz)
       '  g1='+GP
       '  f=f+g1''*W*g1'
       'end'
       'for j=1:'+string(np)
       '  v=0*p;v(j)=pa(j),'
       '  e=0;'
       '  for i=1:'+string(nz)
       '    g1='+GPPV
       '    e=e+g1''*W*g1'
       '  end'
       '  g(j)=e-f;'
       'end;'
       'g=g./pa;']
else // using Jacobian of G
  DGG='g=0;for i=1:nz,g=g+2*'+DGP+'''*W*'+GP+',end'
end

// form cost function for optim
deff('[f,g,ind]=costf(p,ind)',[
    'if ind==2|ind==4 then '
    '  f=0;'
    '   for i=1:'+string(nz)
    '     g1='+GP
    '     f=f+g1''*W*g1'
    '   end'
    'else '
    '  f=0;'
    'end';
    'if ind==3|ind==4 then' 
      DGG
    'else' 
    ' g=0*p;'
    'end'])

[err,p]=optim(costf,varargin(:),imp=imp)

 
endfunction
