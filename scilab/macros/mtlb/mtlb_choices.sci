function mtlb_choices(nam,header,labels,callbacks,inter)
// Copyright INRIA
while %t
  num=x_choose(labels,header,'Close')
  if num==0 then break,end
  execstr(callbacks(num))
end
endfunction
