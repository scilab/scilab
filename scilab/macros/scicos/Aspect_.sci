function Aspect_()
// Copyright INRIA
//
  Cmenu=[]
  [edited,options]=do_options(scs_m.props.options,'3D')
  scs_m.props.options=options
  
  if edited then Cmenu='Replot',end
  
endfunction
