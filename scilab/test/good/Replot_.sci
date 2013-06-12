//** 24 may 2006
//** Comments by Simone Mannori

function Replot_()

    Cmenu=[] ; %pt=[];

    disablemenus() ; //** disable dynamical menu (just to be safe)

    //** xbasc() : // clear the current graphic window

    //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
    gh_curwin = gh_current_window ; //** get the handle of the current graphics window

    clf( gh_curwin ) ; //** clear the current graphic window

    %wdm = scs_m.props.wpar

    window_set_size() ; //** OK

    set_background()  ; //** OK

    drawlater() ;

    //    disp("Start Timer ...."); timer();

    drawobjs(scs_m)   ;  //** OK

    //      disp ("End DrawObjs...."); disp(timer());

    drawnow()  ;

    show_pixmap() ;

    //    disp("End Show... ");disp(timer());

    enablemenus() ;  //** re-enable dynamical menu

    //**--------Debug------------
    //    disp("*windows_ids")
    //    disp(windows)
    //    disp ("#")
    //**-------------------------



endfunction
