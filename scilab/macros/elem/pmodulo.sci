function i=pmodulo(n,m)
//i=pmodulo(n,m) the "positive modulo" of m et n.
//i=n-floor(n./m).*m
//!
// Copyright INRIA
  if size(n,'*')==1 then
    i=zeros(m);
    k=find(m==0)
    i(k)=n
    k=find(m~=0)
    i(k)=n-floor(n./m(k)).*m(k)
  elseif size(m,'*')==1 then
    i=zeros(n);
    if m==0 then
      i=n
    else
      i=n-floor(n./m).*m
    end
  else
    if or(size(n)<>size(m)) then 
      error('dimensions does not match")
    end
    i=zeros(n);
    k=find(m==0);i(k)=n(k);
    k=find(m~=0);i(k)=n(k)-floor(n(k)./m(k)).*m(k)
  end
endfunction
