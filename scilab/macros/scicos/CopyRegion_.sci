function CopyRegion_()
    Cmenu='Open/Set'
    xinfo('Copy Region: Click, drag region, click (left to fix, right to cancel)')
    [scs_m,needcompile]=do_copy_region(scs_m,needcompile);
    xinfo(' ')
endfunction
