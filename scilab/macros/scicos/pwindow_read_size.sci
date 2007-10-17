function pwindow_read_size(gh_window)
// Copyright INRIA
  rhs = argn(2) ; //** get the number of right side arguments  
  
  if rhs==0 then 
     gh_window = gh_current_window ;   
  end  
  gh_window.auto_resize = "on"  // due to scilab bug
  gh_window.figure_size = scs_m.props.wpar(9:10)
  gh_window.figure_position = scs_m.props.wpar(11:12)
endfunction
