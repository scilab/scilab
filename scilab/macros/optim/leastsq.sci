function [f,x,g]=leastsq(imp,fun,varargin)

//                                                        n     p          
//   min sum (fun(x).^2)    where fun is a function from R  to R
//     x
// 
// [f]=fun(x) computes the value f of the function at the point x 
// and the gradient g of f at x g(i,j)=Dfi/dxj
if type(imp)<>1 then
  varargin(0)=fun
  fun=imp
  imp=0
end

if type(fun)==15 then
  fn=fun(1);params=fun;params(1)=null()
else
  fn=fun;params=list()
end

Dfun=varargin(1)
if type(Dfun)==10|type(Dfun)==11|type(Dfun)==13 then 
  J=%t  //Jacobian provided
  varargin(1)=null()
elseif type(Dfun)==15 then 
  error('Jacobian cannot be a list, parameters must be set in fun')
else
  J=%f
end

if type(fn)==10 then //hard coded function given by its name
  if size(params)==0 then 
    error('With hard coded function, user must give output size of fun'),
  end
  m=params(1);params(1)=null()
  n=size(x,'*')
  // foo(m,nx,x,params,f)
  deff('f=fn(x,p)','f=call('''+fn+''','+..
      'm,1,''i'',n,2,''i'',x,3,''d'','+..
      'pars,4,''d'',''out'',['+string(m)+',1],5,''d'')')
  pars=[];
  for k=1:size(params)
    p=params(k)
    pars=[pars;p(:)]
  end
  params=list()
  
end

if J then
  if type(Dfun)==10 then //form function to call hard coded external
    // dfoo(m,nx,x,params,g)
    deff('g=Dfun(x)','g=call('''+DG+''','+..
	'm,1,''i'',nx,2,''i'',x,3,''d'','+..
	'pars,4,''out'',['+string(m)+','+string(nx)+'],5,''d'')')
  end
else
  if params==list()
    deff('g=Dfun(x)','g=numdiff(fn,x)')
  else
    deff('g=Dfun(x,varargin)','g=numdiff(list(fn,varargin(:)),x)')
  end
end

if params==list()
  deff('[f,g,ind]=%opt(x,ind)',[
      'ff=fn(x);gf=Dfun(x)'
      'f=sum(ff.^2)'
      'g=2*(gf''*ff(:))'])
else
  deff('[f,g,ind]=%opt(x,ind)',[
      'ff=fn(x,params(:));gf=Dfun(x,params(:))'
      'f=sum(ff.^2)'
      'g=2*(gf''*ff(:))'])
end
[f,x,g]=optim(%opt,varargin(:),imp=imp)

