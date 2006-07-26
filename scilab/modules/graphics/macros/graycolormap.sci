function cmap = graycolormap(n)
//graycmap   linear gray-scale color map.
//r=g=b
// Copyright INRIA
if size(n,'*')<>1|or(n<1) then
  error('graycolormap : n must be a positive integer')
end
cmap=((0:n-1)'/(n-1))*[1 1 1];
endfunction
