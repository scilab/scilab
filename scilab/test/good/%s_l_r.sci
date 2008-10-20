function b=%s_l_r(a,b)
// a\b a scalar matrix, b rational matrix
//!
// Copyright INRIA
if  size(a,'*')==0  then b=[],return,end
if size(a,1)==-1 then a=a+0,end

[ma,na]=size(a);
if ma==1&na==1 then 
  b('num')=a\b('num'),
elseif size(b('num'),1)==1 then
  b=rlist(a\b('num'),ones(na,ma)*b('den'),b('dt'))
else
  [num,den]=b(['num','den']);

  dd=[];nn=[]
  for j=1:size(num,2)
    [y,fact]=lcm(den(:,j)),
    nn=[nn,a\(num(:,j).*fact)];
    dd=[dd y]
  end
  [num,den]=simp(nn,ones(na,1)*dd)
  b=rlist(num,den,b('dt'))
end
endfunction
