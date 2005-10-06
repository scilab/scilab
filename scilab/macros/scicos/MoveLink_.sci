function MoveLink_()
  %xc=%pt(1);%yc=%pt(2);
  if windows(find(%win==windows(:,2)),1)==100000 then
    Cmenu=[]
    %pt=[]
    return
  elseif %win<>curwin then
    Cmenu="Copy"
  else
    %kk=getobj(scs_m,[%xc;%yc])
    %Path=list('objs',%kk) 
    if %kk<>[] then
      Cmenu=check_edge(scs_m(%Path),'Move')
    else
      Cmenu=[]
    end
  end
endfunction


