function r=mtlb_ishold()
[lhs,rhs]=argn(0)
global('%MTLBHOLD')
if ~%MTLBHOLD|%MTLBHOLD==[] then
  r=%f
else
  r=%MTLBHOLD
end
endfunction
