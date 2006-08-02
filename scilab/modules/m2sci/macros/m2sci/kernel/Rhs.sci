function rhs_tlist=Rhs(varargin)
// Copyright INRIA
// Create a new rhs tlist

expressions=list()
if size(varargin)==1 then
  if or(typeof(varargin(1))==["string","constant"]) then
    expressions(1)=Cste(varargin(1))
  else
    expressions(1)=varargin(1)
  end
else
  for k=1:size(varargin)
    if or(typeof(varargin(k))==["string","constant"]) then
      expressions(k)=Cste(varargin(k))
    elseif varargin(k)<>list() then //list() is used for rand()...
      expressions(k)=varargin(k)
    end
  end
end
rhs_tlist=expressions

endfunction
