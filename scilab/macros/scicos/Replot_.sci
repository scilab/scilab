function Replot_()
    Cmenu='Open/Set'
    %pt=[];
    disablemenus()
    xbasc()
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   // %wdm=scs_m(1)(1)

    %wdm=scs_m.props.wpar
//*****************************************

    window_set_size()
    set_background()
    drawobjs(scs_m),
    if pixmap then xset('wshow'),end
    enablemenus()
endfunction
