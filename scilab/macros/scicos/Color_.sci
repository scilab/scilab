function Color_()
    scs_m_save=scs_m;nc_save=needcompile;enable_undo=%t
    [%pt,scs_m]=do_color(%pt,scs_m)
    Cmenu=[];%pt=[];
    edited=%t
endfunction
