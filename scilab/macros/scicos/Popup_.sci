function Popup_()
  kc=find(%win==windows(:,2))
  if kc==[] then
    message('This window is not an active scicos window')
    Cmenu=[];%pt=[];%ppt=[];Select=[];return
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    palette=palettes(kpal)
    k=getobj(palette,%pt)
    if k<>[] then 
      hilite_obj(palette.objs(k),%win)
      xpause(200000)
      unhilite_obj(palette.objs(k),%win)
      j=3;Select=[k,%win];
    else
      Cmenu==[];%pt=[];%ppt=[];Select=[];return
    end
  elseif %win==curwin then //click dans la fenetre courante
    k=getobj(scs_m,%pt)
    if k<>[] then
      j=1
      hilite_obj(scs_m.objs(k),%win);
      xpause(200000)
      hilite_obj(scs_m.objs(k),%win);
      Select=[k,%win]
    else
      j=2;%ppt=%pt    // for pasting
    end
  elseif slevel>1 then
    execstr('k=getobj(scs_m_'+string(windows(kc,1))+',%pt)')
    if k<>[] then
      execstr('hilite_obj(scs_m_'+string(windows(kc,1))+'.objs(k),%"+...
	      "win)')
      xpause(200000)
      execstr('hilite_obj(scs_m_'+string(windows(kc,1))+'.objs(k),%"+...
	      "win)')
      Select=[k,%win];j=3
    else
      Cmenu==[];%pt=[];%ppt=[];Select=[];return
    end
  else
    message('This window is not an active scicos window')
    Cmenu=[];%pt=[];%ppt=[];Select=[];return
  end
  
  Cmenu=mpopup(%scicos_lhb_list(j));
  
  if Cmenu==[] then %pt=[];%ppt=[];Select=[];end
endfunction
