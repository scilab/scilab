function Help_()
// Copyright INRIA
//**
//** 03 May 2007: BEWARE : this code handle the "help" only for Scicos block (no link, no menu)  
//**  
xinfo("Click on object or menu to get help") ; 

  do_help() ; //** call the help function 

xinfo(" ") ;
    
//** Clear state machine variables   
  Cmenu = [];
  %pt = [];

endfunction
