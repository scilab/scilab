function %zoom=restore(gh_curwin)
//gh_curwin = scf(curwin);
  gh_current_window = gh_curwin ; 
  
  clf(gh_curwin) ;
  gh_curwin.pixmap = "on"
  
  if ~set_cmap(scs_m.props.options('Cmap')) then // add colors if required
    scs_m.props.options('3D')(1)=%f //disable 3D block shape
  end
  
  set_background(gh_curwin)   ;
  
  if size(scs_m.props.wpar,'*')>12 then
    %zoom=scs_m.props.wpar(13)
    pwindow_read_size(gh_curwin) ;
    window_read_size(gh_curwin)  ;
  else
    pwindow_set_size(gh_curwin) ;
    window_set_size(gh_curwin)  ;
  end
  
  menu_stuff() ;
  
  if ~super_block then
    delmenu(curwin,'stop')
    addmenu(curwin,'stop',list(1,'haltscicos'))
    unsetmenu(curwin,'stop')
  else
    unsetmenu(curwin,'Simulate')
  end
endfunction
