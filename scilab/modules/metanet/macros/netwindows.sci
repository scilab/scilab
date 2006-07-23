function l=netwindows()
  //w=winsid()
  G=who('global')
  G=G(part(G,1:7)=='EGdata_')
  if G==[] then
    wins=[],cur=0
  else
    global EGcurrent
    wins=evstr(part(G,8:11))
    cur=EGcurrent
  end
  l=list(wins',cur)
endfunction

