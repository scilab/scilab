function ge_set_nodes_id(w)
  global EGcurrent
  execstr('global EGdata_'+string(EGcurrent))

  if type(w)==1 then
    w=find((0:4)==w)
  else
    w=find(['Nothing','Number','Name','Demand','Label']==w)
  end
  if w<>[] then       
    execstr('EGdata_'+string(EGcurrent)+'.NodeId='+string(w-1))
  end
endfunction
