function [%pt,scs_m]=do_icon_edit(%pt,scs_m)
// do_block - edit a block icon
// Copyright INRIA
  while %t
    if %pt==[] then
      [btn,%pt,win,Cmenu]=cosclick()
      if Cmenu<>[] then
	[%win,Cmenu]=resume(win,Cmenu)
      end
    else
      win=%win;
    end
    xc=%pt(1);yc=%pt(2);%pt=[];
    K=getblock(scs_m,[xc;yc])
    if K<>[] then break,end
  end
  
gr_i=scs_m.objs(K).graphics.gr_i

if type(gr_i)<>15 then
  gr_i=list(gr_i,[],list('sd',[0 0 1 1]))
end
if length(gr_i)==2 then gr_i(3)=list('sd',[0 0 1 1]);end

oldwin=xget('window')
win=winsid()
if win==[] then
  win=0
else
  win=max(win)+1
end

xset('window',win)
xselect()
coli=gr_i(2)
sd=gr_i(3)

sd=gr_menu(sd);
if or(win==winsid()) then xdel(win);end
txt=sd2sci(sd,['sz(1)','sz(2)'],['orig(1)','orig(2)'])
txt(1)=[]
gr_i=['thick=xget(''thickness'')';
     'pat=xget(''pattern'')';
     'fnt=xget(''font'')';
     txt
     'xset(''thickness'',thick)'
     'xset(''pattern'',pat)'
     'xset(''font'',fnt(1),fnt(2))']

xset('window',oldwin)

mac=null();deff('[]=mac()',gr_i,'n')


if check_mac(mac) then
  o=scs_m.objs(K)
  drawblock(o)
  o.graphics.gr_i=list(gr_i,coli,sd)
  drawblock(o)
  scs_m.objs(K)=o
end
endfunction
