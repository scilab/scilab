function win=get_new_window(windows)
global inactive_windows

//  RN? wfree=find(windows(:,1)==0)
// Copyright INRIA
//if wfree<>[] then
//  win=wfree(1)
//else
  win=max(windows(:,2))+1
//end

if inactive_windows(2)<>[] then
   win=max(win,max(inactive_windows(2))+1)
end
endfunction
