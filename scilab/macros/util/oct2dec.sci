function d=oct2dec(o)
//octal2dec Convert base 8 string to decimal integer.
if o==[] then d = [];return,end
b=8
l=max(length(o))
o=convstr(strsubst(part(o,1:l),' ','0'))
m = size(o,'*');
f = [1,cumprod(b(ones(1,l-1)))];f=f($:-1:1);
d=zeros(o);
for i=1:m
  d(i)=f*str2code(o(i))
end
endfunction

