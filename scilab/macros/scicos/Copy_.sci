function Copy_()
// Copyright INRIA
  
  if size(Select,1)>1 then
    //** ... more than a single object 
    [pt1,win1,scs_m_sel] = get_selection2(Select,%pt,%win)
    if size(scs_m_sel.objs)==1 then
       Clipboard = scs_m_sel.objs(1)   
    else
    	Clipboard = scs_m_sel
    end
    Cmenu=[];
  elseif size(Select,1)==1
    //** single object 
    [pt1,win1,o] = get_selection(Select,%pt,%win)
    Clipboard = o ;
    Cmenu = [];
  else
    //** no object 
    message(['No block is selected'; 
	     'click on a block or select a region first.'])
    Cmenu=[]; %pt=[]; %ppt=[]
  end
endfunction

//**-----------------------------------------------------------------------

function [%pt,%win,reg] = get_selection2(Select,%pt,%win)
 win=Select(1,2)  // all items in the second column are identical
  kc=find(win==windows(:,2))
  if kc==[] then
    reg=[];return // window no longer active
  elseif windows(kc,1)<0 then //palette
    scs_m=palettes(-windows(kc,1))
  elseif win==curwin then //selected object in current window
    // scs_m is fine
  elseif slevel>1 then
    execstr('scs_m=scs_m_'+string(windows(kc,1)))
  end
  
  del=setdiff(1:size(scs_m.objs),Select(:,1))
  [reg,DEL,DELL]=do_delete1(scs_m,del,%f)
  reg=do_purge(reg) 
  
  o=reg.objs(1)
  
  if typeof(o)=='Block' then
    o=disconnect_ports(o)
    [orig,sz]=(o.graphics.orig,o.graphics.sz)
    %pt=orig(:)+sz(:)/2
  elseif typeof(o)=='Text'  then  
    [orig,sz]=(o.graphics.orig,o.graphics.sz)
    %pt=orig(:)+sz(:)/2
  elseif typeof(o)=='Link' then  
    %pt=[(o.xx(1)+o.xx(2))/2,(o.yy(1)+o.yy(2))/2] //middle of first
                                                  //segment
  else
    o=[]  // perhaps deleted
  end
  %win=win
endfunction

  
