function Zoomout_()
    Cmenu='Open/Set'
    xinfo('Zoom in')
    %zoom=%zoom/1.2
    window_set_size()
    drawobjs(scs_m),
    xinfo(' ')
    if pixmap then xset('wshow'),end
endfunction
