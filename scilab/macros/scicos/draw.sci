function draw(scs_m)
// Copyright INRIA
nx=size(scs_m)
for k=2:nx
   o=scs_m(k)
   if o(1)<>'Link' then
     execstr(o(5)+'(''plot'',o)')
   else
     drawlink(o)
   end
end




