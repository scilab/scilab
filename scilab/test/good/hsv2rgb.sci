function [r,g,b]=hsv2rgb(h,s,v)
  if argn(2)==1 then
    v=h(:,3);s=h(:,2);h=h(:,1)
  else
    h = h(:);s = s(:);v = v(:)
  end
 
  h=6*h
  k=fix(h-6*%eps)
  f=h-k
  
  t=1-s
  n=1-s.*f
  p=1-s.*(1-f)
  e=ones(h)

  K=bool2s(k*ones(1,6)==ones(k)*(0:5))
  r=sum(K.*[e n t t p e],2)
  g=sum(K.*[p e e n t t],2)
  b=sum(K.*[t t p e e n],2)
  f=v ./ max([r(:);g(:);b(:)])

  if argn(1)==1 then
    r=[f.*r,f.*g,f.*b]
  else
    r=f.*r;g=f.*g;b=f.*b 
  end
endfunction
