function Delete_()
    Cmenu='Open/Set'
    xinfo('Delete: Click on the object to delete')
    [%pt,scs_m,needcompile]=do_delete(%pt,scs_m,needcompile)
    xinfo(' ')
endfunction
