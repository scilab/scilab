function Copy_()
  [pt1,win1,o]=get_selection(Select,%pt,%win)
  o=disconnect_ports(o)
  Clipboard=o
  Cmenu=[];
  %pt=[];
endfunction
