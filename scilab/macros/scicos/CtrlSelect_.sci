function CtrlSelect_()
// Copyright INRIA
  Cmenu=[];
  if windows(find(%win==windows(:,2)),1)==100000 then
    %pt=[]
    return
  end
  kc=find(%win==windows(:,2))
  if kc==[] then
    message('This window is not an active scicos window')
    %pt=[];return
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    palette=palettes(kpal)
    k=getobj(palette,%pt)
  elseif %win==curwin then //click dans la fenetre courante
    k=getobj(scs_m,%pt)  
  elseif slevel>1 then
    execstr('k=getobj(scs_m_'+string(windows(kc,1))+',%pt)')
   else
    message('This window is not an active scicos window')
    %pt=[];return
  end   
 
  if k<>[] then
    ki=find(k==Select(:,1)&%win==Select(:,2))
    if Select<>[] & Select(1,2)<>%win then
      Select=[]  
    end
    if ki==[] then
      Select=[Select;[k,%win]];
      %pt=[];return
    else 
      Select(ki,:)=[];
      %pt=[];return
    end
  else
    %pt=[];return
  end
endfunction


