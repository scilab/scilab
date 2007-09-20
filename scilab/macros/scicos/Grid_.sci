function Grid_()
//** INRIA
  Cmenu=[]
  gh_window = gcf(%win);

  if exists('%scicos_with_grid') then
    %scicos_with_grid=~%scicos_with_grid;
    if %scicos_with_grid then
      gh_window.children.children($).visible="on"
    else
      gh_window.children.children($).visible="off"
    end
  end
  drawnow
  show_pixmap
endfunction