function Addcolor_()
    Cmenu='Open/Set'
    [edited,options]=do_options(scs_m.props.options,'Cmap')
    if edited then 
     scs_m.props.options=options
      set_cmap(scs_m.props.options('Cmap'))
      set_background()
      Cmenu='Replot'
    end       
endfunction
