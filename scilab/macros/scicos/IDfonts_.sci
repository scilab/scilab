function IDfonts_()
    Cmenu=[]
    [edited,options]=do_options(scs_m.props.options,'ID')
    scs_m.props.options=options
    if edited then Cmenu='Replot',end
endfunction
