function h=mtlb_pcolor(varargin)
h=xget('window')
//find first propertyname position
Rhs=size(varargin)
for k=1:Rhs
  if type(varargin(k))==10 then
    Rhs=k-1
    break,
  end
end
C=varargin(Rhs)'
if Rhs==3 then
  grayplot(varargin(1:2),C)
else
  grayplot(1:size(C,1),1:size(C,2),C)
end
endfunction
