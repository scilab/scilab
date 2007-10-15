function PalTree_()
// Copyright INRIA

  Cmenu = [] ;

    global scs_m_palettes ; //** potentially VERY dangerous 
    scs_m_palettes = do_pal_tree(scicos_pal); 

endfunction
