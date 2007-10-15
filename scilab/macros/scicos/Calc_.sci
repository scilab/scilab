function Calc_()
// Copyright INRIA
//**    

    Cmenu = [] ;
    
    xinfo("You may enter any Scilab instruction. Enter ''return'' keyword to come back to Scicos")
    
    xinfo_flag = 1 ; //** keep the message  
//    disablemenus(xinfo_flag);
      pause ; //** recurse in the Scilab command line 
//    enablemenus(); 
    
    Cmenu ="Replot" ; //** force a Replot of the diagram 
    
endfunction
