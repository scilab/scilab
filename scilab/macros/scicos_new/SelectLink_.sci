function SelectLink_()
  
  Select=[];SelectRegion=list()
  //  At this point Select=[]
  if windows(find(%win==windows(:,2)),1)==100000 then
    Cmenu=[]
    %pt=[]
    return
  end
  kc=find(%win==windows(:,2))
  if kc==[] then
    message('This window is not an active scicos window')
    Cmenu=[];%pt=[];return
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    palette=palettes(kpal)
    k=getobj(palette,%pt)
    if k<>[] then 
      Select=[k,%win];Cmenu=[];
      %pt=[];return
    else
      Cmenu==[];%pt=[];return
    end
  elseif %win==curwin then //click dans la fenetre courante
    k=getobj(scs_m,%pt)
    if k<>[] then
      Cmenu=check_edge(scs_m.objs(k),[],%pt)
      if Cmenu==[] then
	Select=[k,%win];Cmenu==[];
	%pt=[]
	return
      end
    else
      Cmenu==[];%ppt=%pt;%pt=[];return  //position %ppt for Paste
    end
  elseif slevel>1 then
    execstr('k=getobj(scs_m_'+string(windows(kc,1))+',%pt)')
    if k<>[] then
      //execstr(' hilite_image=select_hilite(scs_m_'+string(windows(kc,1))+'.objs(k),%win)')
      //xpause(200000)
      //execstr('select_hilite(hilite_image,win)')
      
      Select=[k,%win];Cmenu=[];return
    else
      Cmenu==[];%pt=[];return
    end
  else
    message('This window is not an active scicos window')
     Cmenu=[];%pt=[];return
  end
endfunction


