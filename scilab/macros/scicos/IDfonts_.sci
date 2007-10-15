function IDfonts_()
// Copyright INRIA

    Cmenu = [] ;

    [edited, options] = do_options(scs_m.props.options,'ID') ;

    scs_m.props.options = options ;

    if edited then //** if modified force a Replot :)
      // Acquire the current clicked window and put to "on" the pixmap mode
      gh_curwin = scf(%win) ;
      //** Clear the graphic window WITHOUT changing his pamaters ! :)
      drawlater() ;
        delete(gh_curwin.children.children) ; //** wipe out all the temp graphics object
        drawobjs(scs_m, gh_curwin) ;   //** re-create all the graphics object
      drawnow(); show_pixmap() ;      //** re-draw the graphic object and show on screen
     Cmenu = [] ; %pt = [];
    end
endfunction
