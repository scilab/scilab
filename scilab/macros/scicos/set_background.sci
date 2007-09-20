function set_background(gh_window)
// Copyright INRIA
//** Modified by Simone Mannori (10 Jan 2006 )
//** 10 Jul 2006 
//** 
  
  rhs = argn(2) ; //** get the number of right side arguments  
  
  if rhs==0 then //** without arguments (default) assume ... 
     //** It is NOT possible to modify [gh_current_window] directly outside [scicos]
     gh_curwin = gh_current_window ; //** get the handle of the current graphics window     
     
  else //** the arguments is explicit 
     gh_curwin = gh_window ; //** get the handle of the current graphics window
                             //** from the input argument of the function     
     
  end    
  
  gh_curwin.background = options.Background(1) ; //** "options" is sub structure of scs_m
  gh_curwin.children.background =  options.Background(1) ;
endfunction
