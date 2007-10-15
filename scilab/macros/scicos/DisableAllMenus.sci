function DisableAllMenus()
// Copyright INRIA
  %ws=intersect(winsid(),[inactive_windows(2);curwin]')
  %men=menus(1)
  for %w=%ws
    for k=2:size(%men,'*')
     unsetmenu(%w,%men(k))
    end  // Suppose here all windows have similar menus
  end
endfunction
