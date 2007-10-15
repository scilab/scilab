function inactive_windows=close_inactive_windows(inactive_windows,path)
// Copyright INRIA
//close all inactive windows assciated with children of Super Block path
  DELL=[]  // inactive windows to kill
  if size(inactive_windows(2),'*')>0 then
    n=size(path,'*');
    mainopen=or(curwin==winsid()) // is current window open
    if mainopen then gh_save=gcf(),end
    for kk=1:size(inactive_windows(2),'*')
      if size(inactive_windows(1)(kk),'*')>n & isequal(inactive_windows(1)(kk)(1:n),path) then
	DELL=[DELL kk];
	win=inactive_windows(2)(kk)
	if or(win==winsid()) then
	  gh_del = scf(win) ; 
	  delete (gh_del)
	end
      end
    end
    if mainopen then scf(gh_save),end
  end
  for kk=DELL($:-1:1)  // backward to keep indices valid
    inactive_windows(1)(kk)=null()
    inactive_windows(2)(kk)=[]
  end
endfunction
