function n=countblocks(scs_m)
// count number of blocks used in the scicos data structure scs_m

// Copyright INRIA

nx=size(scs_m)
n=0
for o=scs_m
  if o(1)=='Block' then
    model=o(3)
    if model(1)=='super'|model(1)=='csuper' then
      n=n+countblocks(model(8))
    else
      n=n+1
    end
  else
    n=n+1
  end
end




