function [%ll,%ierr]=script2var(%txt,%ll)
  %ierr=0
  %mm=getfield(1,%ll)
  for %mi=%mm(3:$)
    if execstr(%mi+'=%ll(%mi)','errcatch')<>0 then
      disp(lasterror())
      %ierr=1
      return
    end
  end
  
  [%ll,%ierr]=getvardef(%txt,%ll)
  if %ierr<>0 then 
    return,
  end
endfunction

function [%ll,%ierr]=getvardef(%txt,%ll)
  %nww='';%ierr=0;  // to make sure %ww does not enter the difference
  %nww=size(who('get'),'*')
  %ierr=execstr(%txt,'errcatch')
  if %ierr<>0 then 
    return,
  end
  %mm=who('get')
  %nww2=size(%mm,'*')
  %mm=%mm(1:%nww2-%nww)
  for %mi=%mm(:)'
    if type(evstr(%mi)) <> 13 then
      %ll(%mi)=evstr(%mi)
    end
  end
endfunction
