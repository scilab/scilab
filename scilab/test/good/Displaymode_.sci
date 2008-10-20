function Displaymode_()
    Cmenu='Replot'
    xinfo('Changing Display mode')
    pixmap=do_Displaymode(pixmap)
    %scicos_display_mode=pixmap
    xinfo(' ')
endfunction
