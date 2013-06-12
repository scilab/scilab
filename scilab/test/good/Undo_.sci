function Undo_()
    Cmenu=[]
    %pt=[]
    if enable_undo then
        disablemenus()
        scs_m=scs_m_save;needcompile=nc_save
        %wdm=scs_m.props.wpar

        // xbasc()
        // window_set_size()
        // drawobjs(scs_m),

        //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
        gh_curwin = gh_current_window ; //** get the handle of the current graphics window
        clf( gh_curwin ) ; //** clear the current graphic window
        window_set_size() ; //** OK
        set_background()  ; //** OK
        drawlater() ;

        //    disp("Start Timer ...."); timer();

        drawobjs(scs_m)   ;  //** OK

        //    disp ("End DrawObjs...."); disp(timer());

        drawnow(); show_pixmap() ;

        enable_undo=%f
        enablemenus()
    end
endfunction
