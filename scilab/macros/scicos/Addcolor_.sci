function Addcolor_()
    Cmenu='Open/Set'
    [edited,options]=do_options(scs_m(1)(7),'Cmap')
    if edited then 
      scs_m(1)(7)=options
      set_cmap(options('Cmap'))
      set_background()
      Cmenu='Replot'
    end       

