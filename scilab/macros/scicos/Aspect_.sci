 function Aspect_()
    Cmenu='Open/Set'
    [edited,options]=do_options(scs_m(1)(7),'3D')
    scs_m(1)(7)=options
    if edited then Cmenu='Replot',end

