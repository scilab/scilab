function r=isfield(s,field)
if ~isstruct(s) then
r=0;return;end
w=getfield(1,s);
r=bool2s(or(w(3:$)==field));
endfunction
