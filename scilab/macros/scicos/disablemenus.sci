function disablemenus(men)
// Copyright INRIA
[lhs,rhs]=argn()
xinfo('Please be patient...')
curwin=xget('window')
if rhs<1 then men=menus(1),end
for k=1:size(men,'*')
  unsetmenu(curwin,men(k))
end
