function d=hex2dec(h)
//hex2dec(h) returns in vector d the numbers corresponding to the 
//hexadecimal representation h.

// Copyright INRIA
[nr,nc]=size(h)
n=length(h)
p=cumprod([1,16*ones(1,max(n)-1)]);

d=zeros(h)
for i=1:nr
  for j=1:nc
    s=abs(str2code(h(i,j)))
    if max(s)>15 then
      error('hex2dec : an entry is not a valid hexadecimal representation')
    end
    d(i,j)=p(n(i,j):-1:1)*abs(str2code(h(i,j)))
  end
end
endfunction
