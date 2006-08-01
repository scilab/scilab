function Icon_()
    scs_m_save=scs_m;nc_save=needcompile;enable_undo=%t
    [%pt,scs_m]=do_block(%pt,scs_m)
    Cmenu=[];%pt=[];
    edited=%t
endfunction
