function scs_m=do_resize(scs_m)
// Copyright INRIA
while %t
  [btn,xc,yc,win,Cmenu]=cosclick()
  if Cmenu<>[] then
    Cmenu=resume(Cmenu)
  end
  K=getblocklink(scs_m,[xc;yc])
  if K<>[] then 
    if scs_m(K)(1)=='Block' then
      break,
    else
      //
      [pos,ct]=scs_m(K)(6:7)
      Thick=pos(1)
      Type=pos(2)
      [ok,Thick,Type]=getvalue('Link parameters',['Thickness';'Type'],..
	  list('vec','1','vec',1),[string(Thick);string(Type)])
      if ok then
	drawobj(scs_m(K))
	edited=or(scs_m(K)(6)<>[Thick,Type]);
	scs_m(K)(6)=[Thick,Type];
	drawobj(scs_m(K))
      end
      return
    end
  end
end
o=scs_m(K)

graphics=o(2)
sz=graphics(2)
orig=graphics(1)
[ok,w,h]=getvalue('Set Block sizes',['width';'height'],..
    list('vec',1,'vec',1),string(sz(:)))
if ok  then
  w=maxi(w,5)
  h=maxi(h,5)

  if w<>sz(1) then
    if [get_connected(scs_m,K,'out'),..
	get_connected(scs_m,K,'clkin'),..
	get_connected(scs_m,K,'clkout')]<>[] then 
      message(['Block with connected standard port outputs'
	  'or Event ports cannot be resized horizontally'])
      return
    end
  end
  if h<>sz(2) then
    if [get_connected(scs_m,K,'out'),..
	get_connected(scs_m,K,'in'),..
	get_connected(scs_m,K,'clkin')]<>[] then 
      message(['Block with connected standards ports'
	  'or Event input ports cannot be resized vertically'])
      return
    end
  end
  graphics(2)=[w;h]
  graphics(1)=orig
  drawblock(o)
  o(2)=graphics
  scs_m(K)=o
  drawblock(o)
end


