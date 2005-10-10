function MoveLink_()
  %xc=%pt(1);%yc=%pt(2);
  if windows(find(%win==windows(:,2)),1)==100000 then
    Cmenu=[]
    %pt=[]
    return
  elseif %win<>curwin then
    Cmenu="Duplicate"
  else
    %kk=getobj(scs_m,[%xc;%yc])
    if %kk<>[] then
      Cmenu=check_edge(scs_m.objs(%kk),'Move',%pt)
    else
      Cmenu=[];%ppt=%pt;%pt=[];
    end
  end
endfunction


