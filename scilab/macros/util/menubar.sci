function menubar(win,menus)
// Copyright INRIA
names=menus(1)
for k=size(names,'*'):-1:1
  delmenu(win,names(k))
end

for k=1:size(names,'*')
  addmenu(win,names(k),menus(k+1),list(0,names(k)))
end
datas=[]
endfunction
