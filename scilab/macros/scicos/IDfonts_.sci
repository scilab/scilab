function IDfonts_()
    Cmenu='Open/Set'
    [edited,options]=do_options(scs_m(1).options,'ID')
    scs_m(1).options=options
    if edited then Cmenu='Replot',end
endfunction
