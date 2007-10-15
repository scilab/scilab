function Replot_()
// Copyright INRIA

    Cmenu = [] ;
    %pt   = [] ;
    Select= [] ;
    
    //** BEWARE: this assignement can be erroneus because the user can destroy manually
    //**         the Scicos window.
    //**         How to reproduce the error: 
    //**         1) launch "scicos();"
    //**         2) use "Tools->Calc" ; look at the scilex window
    //**         3) close manually the Scicos window (click over [X])
    //**         4) type "resume" on the scilex window  
    
    gh_curwin = gh_current_window ; //** get the handle of the current graphics window
        
     drawlater();
       clf(gh_curwin) ; //** clear the current graphic window
       %wdm = scs_m.props.wpar
       window_set_size(gh_curwin) ; //** OK
       set_background(gh_curwin)  ; //** OK
       drawobjs(scs_m)   ;  //** Redraw all
     drawnow(); show_pixmap();
    
endfunction
