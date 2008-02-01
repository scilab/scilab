function ge_set_arcs_id(w)
  global EGcurrent
  execstr('global EGdata_'+string(EGcurrent))

  if type(w)==1 then
    w=find((1:10)==w)
  else
    w=find(['Nothing','Number','Name','Cost','Min cap','Max cap',..
	    'Length', 'Quadratic weight','Quadratic origin', ...
	    'Weight','Label']==w)
  end
  if w<>[] then       
    execstr('EGdata_'+string(EGcurrent)+'.ArcId='+string(w-1))
  end
endfunction
