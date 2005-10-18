function drawobjs(scs_m)
// Copyright INRIA
  for i=1:lstsize(scs_m.objs)
    drawobj(scs_m.objs(i))
  end
  drawtitle(scs_m.props)
  show_info(scs_m.props.doc)
endfunction
