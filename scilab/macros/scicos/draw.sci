function draw(scs_m)
// Copyright INRIA
nx=size(scs_m)
for k=2:nx
   o=scs_m(k)
 //  if o(1)<>'Link' then
   if typeof(o)<>'Link' then
    // execstr(o(5)+'(''plot'',o)')
     execstr(o.id+'(''plot'',o)')
   else
     drawlink(o)
   end
end
endfunction
