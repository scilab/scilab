function Copy_()
  if Select<>[] then
    [pt1,win1,o]=get_selection(Select,%pt,%win)
    o=disconnect_ports(o)
    Clipboard=o
    Cmenu=[];
    %pt=[];
  else
    message('No block is selected; click on a block first.')
    Cmenu=[]; %pt=[];
  end
endfunction
