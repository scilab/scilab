function head_comments(name,%paths)
//displays the first comments of a function
  if exists('%paths')==0 then %paths='./',end
  name=stripblanks(name)
  if exists(name)==0 then error('undefined function'),end
  execstr('t=type('+name+')')
  if t<>11&t<>13 then
    error('argument is not the name of a Scilab function')
  end
  l=whereis(name)
  if l<>[] then
    execstr('path=string('+l+');path=path(1)')
    path=path+name+'.sci'
  else
    files= listfiles(%paths+'*.sci')
    if files==[] then
      error(name+'.sci file cannot be found with the given paths')
    end
    k=grep(files,name+'.sci')
    if k<>[] then
      path=files(k(1))
    else
      path=[]
    end
  end
  if path==[] then
    error(name+'.sci file cannot be found with the given paths')
  end
  txt=mgetl(path);
  k=grep(txt,'function');
  if k==[] then
    error('argument is not the name of a Scilab function')
  end
  head=txt(k(1))
  txt=txt(k(1)+1:$)
  K=grep(part(txt,1:2),'//')
  if K(1)<>1 then 
     write(%io(2),'No comment available','(a)')
     return
  end
  k2=find(K(2:$)-K(1:$-1)<>1,1)
  if k2==[] then k2=size(K,'*'),end
  sel=k(1):k2(1)
  if sel<>[] then
    write(%io(2),[head;strsubst(txt(sel),'//','')],'(a)')
  else
    write(%io(2),'No comment available','(a)')
  end
endfunction
