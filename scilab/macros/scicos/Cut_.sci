function Cut_()
  if Select<>[] then
    if Select(2)==curwin then
      Clipboard=scs_m.objs(Select(1))
      [%pt,win1]=get_selection(Select,%pt,%win)
      Cmenu='Delete'
    end
  end
endfunction
