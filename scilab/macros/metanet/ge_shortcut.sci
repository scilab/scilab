function r=ge_shortcut(c)
//Copyright INRIA
//Author : Serge Steer 2002

  r=%t
  execstr(['global EGdata_'+w
	   'ShortCuts=EGdata_'+w+'.ShortCuts'
	   'Menus=EGdata_'+w+'.Menus'])
  
  k=find(ShortCuts(:,1)==ascii(c))

  if k<>[] then 
    m=[]
    for i=1:size(Menus)
      l=find(Menus(i)==ShortCuts(k,2))
      if l<>[] then m=i;break;end
    end
    if m==[] then x_message('incoherent shortcut definitions'),r=%f,return,end
    execstr('r=ge_'+convstr(Menus(m)(1))+'(l-1,win)')
  end
  
endfunction
