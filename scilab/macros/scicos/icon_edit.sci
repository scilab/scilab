function txt=icon_edit()
win=winsid()
if win==[] then
  win=0
else
  win=max(win)+1
end
xset('window',win)
xselect()

sd=gr_menu([0 0 1 1])
txt=sd2sci(sd,['sz(1)','sz(2)'],['orig(1)','orig(2)'])
txt(1)=[]
txt=['thick=xget(''thickness'')';
     'pat=xget(''pattern'')';
     'fnt=xget(''font'')';
     txt
     'xset(''thickness'',thick)'
     'xset(''pattern'',pat)'
     'xset(''font'',fnt(1),fnt(2))']

 xdel(win)
 
 
endfunction
