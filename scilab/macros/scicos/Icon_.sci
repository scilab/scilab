function Icon_()
// Copyright INRIA
// 
//** 18 Sept 2006
//** 21 Nov  2006 
//
// Icon :  edit a block icon
// 

    scs_m_save = scs_m     ;
    nc_save = needcompile  ;
    enable_undo = %t       ; 
    [scs_m] = do_block(%pt, scs_m)
    Cmenu = []  ;
    %pt   = []  ;
    edited = %t ;
    
endfunction
