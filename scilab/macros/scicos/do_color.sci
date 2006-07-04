function [%pt,scs_m]=do_color(%pt,scs_m)
// do_block - edit a block icon
// Copyright INRIA
  win=%win;
  if win<>curwin then return,end
  if Select==[] then
    xc=%pt(1);yc=%pt(2);%pt=[]
    KK=getobj(scs_m,[xc;yc])
    if KK==[] then return,end
  else
    KK=Select(:,1)';%pt=[]
  end
  //
  
  coul=getcolor('Choose a color',1);
  
  for K=KK
    o=scs_m.objs(K)
    if typeof(o)=='Link' then
      [nam,pos,ct]=(o.id,o.thick,o.ct)
      c=coul
      if c<>[] then
	connected=connected_links(scs_m,K)
	for kc=connected
	  o=scs_m.objs(kc);ct=o.ct
	  if ct(1)<>c then
	    drawobj(o)
	    o.ct(1)=c;
	    drawobj(o)
	    scs_m.objs(kc)=o
	  end
	end
      end
    elseif typeof(o)=='Block' then
      if type(o.graphics.gr_i)==10 then,
	o.graphics.gr_i=list(o.graphics.gr_i,[]),
      end
      if o.graphics.gr_i(2)==[] then
	coli=0
      else
	coli=o.graphics.gr_i(2)
      end
      coln=coul
      if coln<>[] then
	if coln<>coli then
	  drawblock(o)
	  o.graphics.gr_i(2)=coln
	  scs_m.objs(K)=o
	  drawblock(o)
	end
      end
    elseif  typeof(o)=='Text' then
      //not implemented
    end
  end
endfunction
