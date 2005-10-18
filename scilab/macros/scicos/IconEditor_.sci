function IconEditor_()
    Cmenu='Open/Set'
    scs_m_save=scs_m;nc_save=needcompile;enable_undo=%t
    disablemenus()
    [%pt,scs_m]=do_icon_edit(%pt,scs_m)
    Cmenu='Open/Set'
    %pt=[]
    enablemenus()
    xinfo(' ')
    edited=%t  
endfunction
