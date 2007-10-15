function window_read_size(gh_window)
// Copyright INRIA
   rhs = argn(2) ; //** get the number of right side arguments  
  
  if rhs==0 then 
     gh_window = gh_current_window ;   
  end  
  
  r = gh_window.figure_size ; 
  gh_window.auto_resize = "off" ; //**

  gh_window.axes_size = scs_m.props.wpar(5:6)

  //** axes settings
  gh_axes = gh_window.children ; //** axes handle
  gh_axes.tight_limits = "on"  ; //** set the limit "gh_axes.data_bounds" in "hard mode"

  gh_axes.margins = [0,0,0,0] ;     //**


  gh_axes.data_bounds = matrix(scs_m.props.wpar(1:4),2,2)

  wrect = [0 , 0, 1, 1] ;
  gh_axes.axes_bounds = wrect ;

  xset('viewport',scs_m.props.wpar(7),scs_m.props.wpar(8))
				
  xselect(); //** put the current window in foreground

endfunction
