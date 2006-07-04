function MoveLink_()
  if windows(find(%win==windows(:,2)),1)==100000 then
    Cmenu=[]
    %pt=[]
    return
  elseif %win<>curwin then
    
    kc=find(%win==windows(:,2))
    if kc==[] then
      message('This window is not an active scicos window')
      Cmenu=[];%pt=[];return
    elseif windows(kc,1)<0 then //click dans une palette
      kpal=-windows(kc,1)
      palette=palettes(kpal)
      %kk=getobj(palette,%pt)
    elseif slevel>1 then
      execstr('%kk=getobj(scs_m_'+string(windows(kc,1))+',%pt)')
    end
    if %kk<>[] then
      Cmenu="Duplicate"
       Select=[%kk,%win]
    else
      Cmenu="SelectRegion"
       Select=[];
    end
  else
    %kk=getobj(scs_m,%pt)
    if %kk<>[] then
      Cmenu=check_edge(scs_m.objs(%kk),'Move',%pt)
      Select=[%kk,%win]
    else
      //Cmenu=[];%ppt=%pt;%pt=[];
      Cmenu="SelectRegion"
      %ppt=[];Select=[]
    end
  end
endfunction


