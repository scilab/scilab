function Resize_()
    Cmenu='Open/Set'
    scs_m_save=scs_m;nc_save=needcompile;enable_undo=%t
    xinfo('Click block to resize')
    [%pt,scs_m]=do_resize(%pt,scs_m)
    xinfo(' ')
    edited=%t
endfunction
