function Flip_()
    // 29 Jun 2006 :)

    scs_m_save = scs_m ; nc_save = needcompile;

    [%pt,scs_m] = do_tild(%pt,scs_m)

    //**---------- Force a Replot -----------

    Cmenu = "Replot" ;
    %pt = [] ;

endfunction
