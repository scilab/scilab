function y=mean(x,orient)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then
  if x==[] then y=%nan;return,end
  y=sum(x)/size(x,'*')
else
  if orient<>'r'&orient<>'c'&orient<>1&orient<>2 then
    error('mean : second argument must be ''r'', ''c'', 1 or 2')
  end
  if x==[] then y=[];return,end
  y=sum(x,orient)/size(x,orient)
end
endfunction
