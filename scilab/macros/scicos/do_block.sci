function scs_m=do_block(scs_m)
// do_block - edit a block icon
// Copyright INRIA
while %t
  [btn,xc,yc,win,Cmenu]=cosclick()
  if Cmenu<>[] then
    Cmenu=resume(Cmenu)
  end
  K=getblock(scs_m,[xc;yc])
  if K<>[] then break,end
end
gr_i=scs_m(K)(2)(9)
if type(gr_i)==15 then
  [gr_i,coli]=gr_i(1:2)
else
  coli=[]
end
if gr_i==[] then gr_i=' ',end
while %t do
  gr_i=dialog(['Give scilab instructions to draw block';
      'shape.';
      'orig(1) : block down left corner x coordinate';
	'orig(2) : block down left corner y coordinate';
	  'sz(1)   : block width';
	  'sz(2)   : block height'],gr_i)
  if gr_i==[] then return,end
  mac=null();deff('[]=mac()',gr_i,'n')
  if check_mac(mac) then
    o=scs_m(K)
    drawblock(o)
    o(2)(9)=list(gr_i,coli)
    drawblock(o)
    scs_m(K)=o
    break
  end
end


