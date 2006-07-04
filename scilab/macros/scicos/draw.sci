function draw(scs_m)
// Copyright INRIA
  nx=lstsize(scs_m.objs)
  for k=1:nx
    o=scs_m.objs(k)
    if typeof(o)<>'Link' then
      execstr(o.id+'(''plot'',o)')
    else
      drawlink(o)
    end
  end
endfunction
