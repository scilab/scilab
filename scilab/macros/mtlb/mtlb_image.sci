function h=mtlb_image(varargin)
h=xget('window')
//find first propertyname position
Rhs=size(varargin)
for k=1:Rhs
  if type(varargin(k))==10 then
    Rhs=k-1
    break,
  end
end
//Only C argument taken into account.
C=varargin(Rhs)
Matplot(C)
endfunction
