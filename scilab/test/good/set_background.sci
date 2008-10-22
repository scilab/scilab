// Copyright INRIA
//** Modified by Simone Mannori (10 Jan 2006 )

function set_background()

  //** OLD graphics 
  //** obsolete, removed
  //** xset('background',options.Background(1));
  
  //** NEW graphics
  //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
  gh_curwin = gh_current_window ; //** get the handle of the current graphics window
  gh_curwin.background = options.Background(1) ; //** "options" is sub structure of scs_m
  gh_curwin.children.background =  options.Background(1) ;

endfunction
