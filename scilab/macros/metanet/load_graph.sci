function [g]=load_graph(path)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39),end
// path
if type(path)<>10|prod(size(path))<>1 then
  error('Argument must be a string')
end
g=m6loadg(path)
g=glist(g(:))
g($+1)=[]; g($+1)=[]
g.name(2)=part(path,1:length(path)-6)
endfunction
