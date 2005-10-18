function Align_()
    Cmenu='Open/Set'
    xinfo('Click on an a port , click on a port of object to be moved')
    scs_m_save=scs_m;nc_save=needcompile;
    [%pt,scs_m]=prt_align(%pt,scs_m)
    xinfo(' ')
endfunction
