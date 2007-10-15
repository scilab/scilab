function Addcolor_()
// Copyright INRIA
//
    [edited,options] = do_options(scs_m.props.options,'Cmap')

    if edited then
      scs_m.props.options=options
      set_cmap(scs_m.props.options('Cmap'))
      set_background()
      // Acquire the current clicked window and put to "on" the pixmap mode
      gh_curwin = scf(%win) ;

      //** Clear the graphic window WITHOUT changing his pamaters ! :)
      drawlater() ;
        delete(gh_curwin.children.children) ; //** wipe out all the temp graphics object
        drawobjs(scs_m, gh_curwin) ;   //** re-create all the graphics object
      drawnow(); show_pixmap() ;      //** re-draw the graphic object and show on screen
     Cmenu = [] ; %pt = [];

    else
      Cmenu=[];%pt=[];
    end

endfunction
