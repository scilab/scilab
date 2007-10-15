function Setgrid_()
// Copyright INRIA

  xinfo('Set grid')
  if exists('%scicos_with_grid') then
    [%scs_wgrid] = do_grid(%scs_wgrid)
  end
  xinfo(' ')
  Cmenu = [] ; %pt = [];
endfunction
