function win=get_new_window(windows)
// Copyright INRIA

  global inactive_windows
  win=max(windows(:,2))+1


if inactive_windows(2)<>[] then
   win=max(win,max(inactive_windows(2))+1)
end
endfunction
