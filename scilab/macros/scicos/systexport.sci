function systexport(scs_m,exp_dir)
//xbasc()
// Copyright INRIA
if getenv('WIN32','NO')=='OK' then
//  do_export(scs_m,exp_dir+'\'+scs_m(1)(2)(1))
  do_export(scs_m,exp_dir+'\'+scs_m(1).title(1))
else
  //do_export(scs_m,exp_dir+'/'+scs_m(1)(2)(1))
  do_export(scs_m,exp_dir+'/'+scs_m(1).title(1))
end
nx=size(scs_m)
for k=2:nx
  o=scs_m(k)
 // if o(1)=='Block' then
  if typeof(o)=='Block' then
    //model=o(3)
    model=o.model
 //   if model(1)=='super' then
    if model.sim=='super' then
//      win=win+1
     // systexport(model(8),exp_dir)
     systexport(model.rpar,exp_dir) 
    end
  end
end
endfunction
