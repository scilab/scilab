function Cut_()
  if Select(1,2)==curwin then
    scs_m_save=scs_m,nc_save=needcompile
    del=setdiff(1:size(scs_m.objs),Select(:,1))
    Clipboard=do_delete1(scs_m,del,%f)
    [scs_m,DEL]=do_delete1(scs_m,Select(:,1)',%t)
    if DEL<>[] then 
      needcompile=4,
      edited=%t
      enable_undo=%t
      //suppress right-most deleted elements
      while getfield(1,scs_m.objs($))=='Deleted' then
	scs_m.objs($)=null();
	if lstsize(scs_m.objs)==0 then break,end
      end
    end
    Cmenu=[];
  else
    message(['Only current diagram can be edited'])
    Cmenu=[]; %pt=[]; %ppt=[]
  end
endfunction


