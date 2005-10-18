function Backgroundcolor_()
    Cmenu='Open/Set'
    [edited,options]=do_options(scs_m.props.options,'Background')
    scs_m.props.options=options
    
    if edited then
      scs_m.props.options=options
      set_background()
      Cmenu='Replot'
    end
endfunction
