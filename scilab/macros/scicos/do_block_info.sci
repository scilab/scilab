function %pt=do_block_info(%pt,scs_m)
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
  kc=find(win==windows(:,2))
  if kc==[] then
    message('This window is not an active palette')
    k=[];break
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    palette=palettes(kpal)
    k=getobj(palette,[xc;yc])
    if k<>[] then txt=get_block_info(palette,k),break,end
  elseif win==curwin then //click dans la fenetre courante
    k=getobj(scs_m,[xc;yc])
    if k<>[] then txt=get_block_info(scs_m,k),break,end
  end
end
if %t then
  if getenv('WIN32','NO')=='OK' then
    [u,ierr]=file('open',TMPDIR+'\scs_info','unknown')
  else
    [u,ierr]=file('open',TMPDIR+'/scs_info','unknown')
  end
  
  if ierr==0 then
    write(u,txt,'(a)')
    file('close',u)
    if getenv('WIN32','NO')=='OK' & getenv('COMPILER','NO')=='VC++' then 
	out_f = strsubst(TMPDIR,'/','\')+'\scs_info';
  	host(''"'+strsubst(SCI,'/','\')+'\bin\xless.exe'" '+ out_f);
    else 
        unix_s(SCI+'/bin/xless '+TMPDIR+'/scs_info&')
    end
  end
else
  x_message(txt)
end
