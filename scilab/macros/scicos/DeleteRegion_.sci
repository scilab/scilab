function DeleteRegion_()
    Cmenu='Open/Set'
    xinfo('Delete Region: Click, drag region and click (left to delete, right to cancel)')
    [scs_m,needcompile]=do_delete_region(scs_m,needcompile);
    xinfo(' ')
    %pt=[]
endfunction
