function Color_()
// Copyright INRIA
    
    scs_m_save = scs_m    ;
    nc_save = needcompile ;
    enable_undo = %t      ;
    
    [scs_m] = do_color(%win, %pt, scs_m); 
    
    Cmenu = []; %pt = [];
    
    edited = %t
    
endfunction
