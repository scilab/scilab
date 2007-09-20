function scs_show(scs_m, win)
// Copyright INRIA
//**
//** 24 Nov 2006: Super simplified version
//**

  if %scicos_debug_gr then
    disp("scs_show active...")
  end

  gh_curwin = scf(win) ;
  gh_curwin.pixmap="on"

  if ~set_cmap(scs_m.props.options('Cmap')) then // add colors if required
    scs_m.props.options('3D')(1)=%f //disable 3D block shape
  end

  options = scs_m.props.options ; //** not used 

  set_background(gh_curwin)     ;
  pwindow_set_size(gh_curwin)   ;
  window_set_size(gh_curwin)    ;

  scs_m.props.title(1) = 'Scilab Graphics of '+scs_m.props.title(1)

  drawobjs(scs_m, gh_curwin)
  drawnow();
endfunction
