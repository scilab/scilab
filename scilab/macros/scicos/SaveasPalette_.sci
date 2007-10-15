function SaveasPalette_()
// Copyright INRIA


    Cmenu = [] ;
    spmode = pal_mode
    pal_mode = %t
    [scs_m,editedx] = do_SaveAs()
    if ~super_block then edited=editedx,end
    pal_mode=spmode
endfunction
