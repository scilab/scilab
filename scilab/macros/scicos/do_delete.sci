function [%pt,scs_m,needcompile]=do_delete(%pt,scs_m,needcompile)
// do_delete - delete a scicos object
// get first object to delete
//!
// Copyright INRIA
while %t
  
  if %pt==[] then
    [btn,xc,yc,win,Cmenu]=cosclick()
    if Cmenu<>[] then
      %pt=[]
      [Cmenu]=resume(Cmenu)
    elseif btn>31 then
      Cmenu=%tableau(min(100,btn-31));%pt=[xc;yc];
      if Cmenu==emptystr() then Cmenu=[];%pt=[];end
      [%win,Cmenu]=resume(win,Cmenu)
    end
  else
    xc=%pt(1);yc=%pt(2);win=%win;%pt=[]
  end
  K=getobj(scs_m,[xc;yc])
  if K<>[] then break,end
end

needreplay=replayifnecessary()
scs_m_save=scs_m,nc_save=needcompile
[scs_m,DEL]=do_delete1(scs_m,K,%t)
if DEL<>[] then 
  needcompile=4,

  //suppress right-most deleted elements
  while scs_m($)==list('Deleted') then
    scs_m($)=null();
  end
  [scs_m_save,nc_save,enable_undo,edited,needreplay]=resume(scs_m_save,nc_save,%t,%t,needreplay)
end

