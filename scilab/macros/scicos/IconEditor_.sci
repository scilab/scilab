function IconEditor_()
    Cmenu='Open/Set'
    scs_m_save=scs_m;nc_save=needcompile;enable_undo=%t
    disablemenus()
    scs_m=do_icon_edit(scs_m)
    Cmenu='Open/Set'
    enablemenus()
    xinfo(' ')
    edited=%t  
endfunction
