function [%pt,scs_m]=do_color(%pt,scs_m)
// do_block - edit a block icon
// Copyright INRIA
while %t
  if %pt==[] then
    [btn,xc,yc,win,Cmenu]=cosclick()
    if Cmenu<>[] then
      %pt=[];
      [Cmenu]=resume(Cmenu)
    elseif btn>31 then
      Cmenu=%tableau(min(100,btn-31));%pt=[xc;yc];
      if Cmenu==emptystr() then Cmenu=[];%pt=[];end
	
      [%win,Cmenu]=resume(win,Cmenu)
    end
  else
    xc=%pt(1);yc=%pt(2);win=%win;%pt=[]
  end 

  K=getobj(scs_m,[xc;yc])
  if K<>[] then break,end
end
o=scs_m(K)
if o(1)=='Link' then
  [nam,pos,ct]=o(5:7)
  c=getcolor('Choose a color',ct(1));
  if c<>[] then
    connected=connected_links(scs_m,K)
    for kc=connected
      o=scs_m(kc);ct=o(7)
      if ct(1)<>c then
	drawobj(o)
	o(7)(1)=c;
	drawobj(o)
	scs_m(kc)=o
      end
    end
  end
elseif o(1)=='Block' then
  graphics=o(2)
  gr_i=graphics(9)
  if type(gr_i)==10 then,gr_i=list(gr_i,[]),end
  if gr_i(2)==[] then
    coli=0
  else
    coli=gr_i(2)
  end
  coln=getcolor('color',coli)
  if coln<>[] then
    if coln<>coli then
      gr_i(2)=coln

      graphics(9)=gr_i
      drawblock(o)
      o(2)=graphics
      scs_m(K)=o
      drawblock(o)
    end
  end
elseif o(1)=='Text' then
  //not implemented
end
