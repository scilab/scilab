function Defaultlinkcolors_()
    Cmenu='Open/Set'
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //[edited,options]=do_options(scs_m(1)(7),'LinkColor')
    //scs_m(1)(7)=options,

    [edited,options]=do_options(scs_m.props.options,'LinkColor')
    scs_m.props.options=options,
//*****************************************************************
    if edited then Cmenu='Replot',end
endfunction
