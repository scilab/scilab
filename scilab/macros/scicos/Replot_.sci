function Replot_()
    Cmenu='Open/Set'
    %pt=[];
    disablemenus()
    xbasc()

    %wdm=scs_m(1)(1)
    window_set_size()
    drawobjs(scs_m),
    if pixmap then xset('wshow'),end
    enablemenus()

