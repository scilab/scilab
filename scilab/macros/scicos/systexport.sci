function systexport(scs_m,exp_dir)
// Copyright INRIA
  if getenv('WIN32','NO')=='OK' then
    do_export(scs_m,exp_dir+'\'+scs_m.props.title(1))
  else
    do_export(scs_m,exp_dir+'/'+scs_m.props.title(1))
  end

  for k=1:lstsize(scs_m.objs)
    o=scs_m.objs(k)
    if typeof(o)=='Block' then
      model=o.model
      if model.sim=='super' then
	systexport(model.rpar,exp_dir) 
      end
    end
  end
endfunction
