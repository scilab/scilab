function Replace_()
    Cmenu='Open/Set'
    xinfo('Click on new object , click on object to be replaced')
    scs_m_save=scs_m;nc_save=needcompile;enable_undo=%t
    [scs_m,needcompile]=do_replace(scs_m,needcompile);
    xinfo(' ')

