function [d]=base2dec(h,b)
// Copyright INRIA  
//base2dec Convert base b string to decimal integer.
if h==[] then d = [];return,end

l=max(length(h))
h=convstr(strsubst(part(h,1:l),' ','0'))
m = size(h,'*');
f = [1,cumprod(b(ones(1,l-1)))];f=f($:-1:1);
d=zeros(h)
for i=1:m
  d(i)=f*str2code(h(i))
end
endfunction

