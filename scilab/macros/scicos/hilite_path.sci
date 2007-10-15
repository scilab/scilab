function hilite_path(path,mess,with_intermediates)
// Copyright INRIA
//hilite a block included in a superblock hierarchy and displays a
//message.   If required all the intermediate superblocs are displayed
//first entry of the path must be a bloc of currently opened diagram  

  global   Scicos_commands

  Scicos_commands=['%diagram_path_objective='+sci2exp(path(1:$-1))+';%scicos_navig=1';
		   'Cmenu=[];xselect();%scicos_navig=[];']

  if argn(2)<3 then with_intermediates=%f,end
  if argn(2)<2 then mess=' ',end
  scs_m;
  mxwin=maxi(winsid()),opened_windows=[]

  //** save the current figure handle
  gh_wins = gcf();

  hilite_obj(path(1))

  if with_intermediates then
    scs_m=scs_m.objs(path(1)).model.rpar;
    for k=2:size(path,'*')
      scs_show(scs_m,mxwin+k);opened_windows=[mxwin+k opened_windows]
      hilite_obj(path(k))
      scs_m=scs_m.objs(path(k)).model.rpar;
    end
  else
    if size(path,'*')==1 then
      hilite_obj(path)
    else
      for k=1:size(path,'*')-1;scs_m=scs_m.objs(path(k)).model.rpar;end
      scs_show(scs_m,mxwin+1);opened_windows=[mxwin+1 opened_windows]
      hilite_obj(path($))
    end
  end
  message(mess)

  for k=1:size(opened_windows,'*') //** close opened_windows
    //** select the opened_windows(k) and get the handle
    gh_del = scf(opened_windows(k));
    //** delete the window
    delete(gh_del)
  end
  //scs_m=null()

  //** restore the active window
  scf(gh_wins);

  //** unhilite entity path(1)
  unhilite_obj(path(1))
endfunction
