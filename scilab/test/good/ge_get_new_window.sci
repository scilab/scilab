function win=ge_get_new_window(windows)
//Copyright INRIA
//Author : Serge Steer 2002

wfree=find(windows(:,1)==0)
// Copyright INRIA
if wfree<>[] then
  win=wfree(1)
else
  win=maxi(windows(:,2))+1
end
endfunction
