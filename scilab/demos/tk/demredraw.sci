function demredraw(alpha)
global alpha0
if alpha0==[] then alpha0=30,end
if done&abs(alpha-alpha0)>2 then
  done=%f
  xclear();
  xtape('replayna',0,alpha,30)
  alpha0=alpha
  done=%t
end

