function IconEditor_()
    scs_m_save=scs_m;nc_save=needcompile;enable_undo=%t
    disablemenus()
    [scs_m]=do_icon_edit(%pt,scs_m)
    Cmenu=[]
    %pt=[]
    enablemenus()
    edited=%t  
endfunction
