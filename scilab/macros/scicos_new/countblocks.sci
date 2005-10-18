function n=countblocks(scs_m)
// count number of blocks used in the scicos data structure scs_m
// Copyright INRIA

n=0
for o=scs_m.objs
  x=getfield(1,o)
  if x(1)=='Block' then
    if o.model.sim=='super'|o.model.sim=='csuper' then
      n=n+countblocks(o.model.rpar)
    else
      n=n+1
    end
  else
    n=n+1
  end
end
endfunction
