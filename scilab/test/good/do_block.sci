function [%pt,scs_m]=do_block(%pt,scs_m)
// do_block - edit a block icon
// Copyright INRIA
  win=%win;
  if Select==[] then
    xc=%pt(1);yc=%pt(2);%pt=[];
    K=getblock(scs_m,[xc;yc])
    if K==[] then Cmenu=[];%pt=[];return,end
  else
    K=Select(:,1)';%pt=[]
    if size(K,'*')>1|%win<>Select(1,2) then
    message("Only one block can be selected in current window for this operation.")
    Cmenu=[];%pt=[];return
  end

  end
  
    
  gr_i=scs_m.objs(K).graphics.gr_i
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
      o=scs_m.objs(K)
      drawblock(o)
      o.graphics.gr_i=list(gr_i,coli)
      drawblock(o)
      scs_m.objs(K)=o
      break
    end
  end
endfunction
