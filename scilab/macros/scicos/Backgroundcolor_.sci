function Backgroundcolor_()
    Cmenu='Open/Set'

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//    [edited,options]=do_options(scs_m(1)(7),'Background')
//    scs_m(1)(7)=options
  [edited,options]=do_options(scs_m.props.options,'Background')
 scs_m.props.options=options
 
   if edited then
      //scs_m(1)(7)=options

  scs_m.props.options=options
//***********************************************
      set_background()
      Cmenu='Replot'
    end
endfunction
