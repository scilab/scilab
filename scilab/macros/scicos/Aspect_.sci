 function Aspect_()
    Cmenu='Open/Set'

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//    [edited,options]=do_options(scs_m(1)(7),'3D')
    [edited,options]=do_options(scs_m(1).options,'3D')

 //   scs_m(1)(7)=options
  scs_m(1).options=options

    if edited then Cmenu='Replot',end

