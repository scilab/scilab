function mtlb_hold(flag)
[lhs,rhs]=argn(0)
global('%MTLBHOLD')
if rhs==0 then
  if ~%MTLBHOLD|%MTLBHOLD==[] then
    %MTLBHOLD=%t
  else
    %MTLBHOLD=~%MTLBHOLD
  end
elseif convstr(flag)=='on' then
  %MTLBHOLD=%t
else
  %MTLBHOLD=%f
end
endfunction
