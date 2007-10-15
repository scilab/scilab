function Flip_()
// Copyright INRIA
//** Alan/Simone 13/12/06 : Replot no more needed
   scs_m_save = scs_m    ; //** save the old diagram for "undo" operation 
   nc_save = needcompile ; //** save the old diagram state
   
   [%pt, scs_m] = do_tild(%pt, scs_m) ; //** do the "tilt" operation 
   
   Cmenu = []; //** 
   %pt   = [];
endfunction
