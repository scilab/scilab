function Addcolor_()
    Cmenu='Open/Set'

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//    [edited,options]=do_options(scs_m(1)(7),'Cmap')
    [edited,options]=do_options(scs_m(1).options,'Cmap')
    if edited then 
      //scs_m(1)(7)=options
     scs_m(1).options=options
      set_cmap(scs_m(1).options('Cmap'))

//***************************************
      set_background()
      Cmenu='Replot'
    end       
endfunction
