function Icon_()
    Cmenu='Open/Set'
    xinfo('Click on block to edit its icon')
    scs_m_save=scs_m;nc_save=needcompile;enable_undo=%t
    scs_m=do_block(scs_m)
    xinfo(' ')
    edited=%t
endfunction
