function Calc_()
    Cmenu=[]
    xinfo('You may enter any Scilab instruction. enter return to terminate')
    scs_gc=save_scs_gc()
    disablemenus()
    pause
    //xinfo(' ')
    restore_scs_gc(scs_gc);scs_gc=null()
    enablemenus()
    Cmenu='Replot'
endfunction
