function Label_()
    Cmenu='Open/Set'
    xinfo('Click block to label')
    scs_m_save=scs_m;nc_save=needcompile;enable_undo=%t
    [%mod,scs_m]=do_label(scs_m)
    edited=edited|%mod
    xinfo(' ')

