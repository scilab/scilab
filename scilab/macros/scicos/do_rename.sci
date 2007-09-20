function [scs_m,edited]=do_rename(scs_m)
// Copyright INRIA
  if pal_mode then
    mess='Enter the new palette name'
  else
    mess='Enter the new diagram name'
  end

  new = dialog(mess,scs_m.props.title(1))
  new = new(1)
  
  if new<>[] then
    drawtitle(scs_m.props)     //erase title
    scs_m.props.title(1) = new 
    drawtitle(scs_m.props)     //draw title
    edited = %t
  end
endfunction
