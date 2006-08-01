function Defaultlinkcolors_()
    Cmenu=[]
    [edited,options]=do_options(scs_m.props.options,'LinkColor')
    scs_m.props.options=options,
    if edited then Cmenu='Replot',end
endfunction
