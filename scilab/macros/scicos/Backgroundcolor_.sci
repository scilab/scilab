function Backgroundcolor_()
    Cmenu='Open/Set'
    [edited,options]=do_options(scs_m(1)(7),'Background')
    scs_m(1)(7)=options
    if edited then
      scs_m(1)(7)=options
      set_background()
      Cmenu='Replot'
    end

