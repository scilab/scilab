function [ok,%tcur,%cpr,alreadyran,needcompile,%state0,solver]=do_run(%cpr)
// realize action associated to the run button
// performs necessary diagram (re-)compilation
// performs simulation initialisation
// performs simulation error recovery
//
// state  : is current state
// %state0 : is initial state value
//
// define user possible choices

// Copyright INRIA
if size(scs_m(1)(3),'*')<6 then solver =0,else solver=scs_m(1)(3)(6),end
wpar=scs_m(1);tolerances=wpar(3)
if size(tolerances,'*')<6 then tolerances(6) =0,end
solver=tolerances(6)


if needcompile==4 then 
do_terminate(),alreadyran=%f
end
  
if alreadyran&needcompile<=1 then
  choix=['Continue';'Restart';'End']
elseif alreadyran then
  choix=['Restart';'End']
else
  choix=[]
end


// update parameters or compilation results
[%cpr,%state0_n,needcompile,ok]=do_update(%cpr,%state0,needcompile)
if ~ok then %tcur=[],alreadyran=%f,return,end
if or(%state0_n<>%state0) then //initial state has been changed
  %state0=%state0_n
//  %cpr(1)=%state0
  if choix(1)=='Continue' then choix(1)=[],end
end

if %cpr(2).xptr($)-1<size(%cpr(1).x,'*') & solver<100 then
  message(['Diagram has been compiled for implicit solver'
	   'switching to implicit Solver'])
  solver=100
  tolerances(6)=solver
end


  
// ask user what to do
if choix<>[] then
  to_do=x_choose(choix,'What do you want to do')
    if to_do==0 then ok=%f,return,end
  select choix(to_do)
  case 'Continue' then 
    needstart=%f
    state=%cpr(1)
  case 'Restart' then 
    needstart=%t
    state=%state0
  case 'End' then 
    errcatch(-1,'continue')
    state=%cpr(1)
    needstart=%t
    wpar=scs_m(1);tf=wpar(4);

    [state,t]=scicosim(%cpr(1),%tcur,tf,%cpr(2),'finish',tolerances)
    %cpr(1)=state
    alreadyran=%f
    errcatch(-1)
    if iserror(-1)==1 then
      errclear(-1)
      kfun=curblock()
      corinv=%cpr(4)
      if kfun<>0 then
	path=corinv(kfun)
	//if size(path)==1 then path=path(1),end
	xset('window',curwin)
	bad_connection(path,..
	    ['End problem with hilited block';
	    'see message in scilab window'])
      else
	message('End problem, see message in scilab window')
      end
      ok=%f
      return
    end
    xset('window',curwin)
    return
  end
else
  needstart=%t
  state=%state0
end

win=xget('window')

if needstart then //scicos initialisation
  if alreadyran then
    do_terminate()
    alreadyran=%f
  end
  %tcur=0
  %cpr(1)=%state0
  wpar=scs_m(1);tf=wpar(4);
  if tf*tolerances==[] then 
    x_message(['Simulation parameters not set';'use setup button']);
    return;
  end
  errcatch(-1,'continue')
  [state,t]=scicosim(%cpr(1),%tcur,tf,%cpr(2),'start',tolerances)
  %cpr(1)=state
  errcatch(-1)
  if iserror(-1)==1 then
    errclear(-1)
    kfun=curblock()
    corinv=%cpr(4)
    if kfun<>0 then
      //path=get_subobj_path(corinv(kfun))
      //if size(path)==1 then path=path(1),end
      xset('window',curwin)
      path=corinv(kfun)
      bad_connection(path,..
	  ['Initialisation problem with hilited block';
	  'see message in scilab window'])
    else
      message('Initialisation problem, see message in scilab window')
    end
    ok=%f
    xset('window',curwin)
    unsetmenu(curwin,'stop')
    enablemenus()
    return
  end
  xset('window',win);
end  
needreplay=%t

// simulation
wpar=scs_m(1);tf=wpar(4);
disablemenus()
setmenu(curwin,'stop')
timer()
needreplay=%t
errcatch(-1,'continue')
[state,t]=scicosim(%cpr(1),%tcur,tf,%cpr(2),'run',tolerances)

%cpr(1)=state
errcatch(-1)
if iserror(-1)==0 then
  alreadyran=%t
  if tf-t<tolerances(3) then
    needstart=%t
    do_terminate()
    alreadyran=%f
  else
    %tcur=t
  end
else
  errclear(-1)
  kfun=curblock()
   corinv=%cpr(4)
  if kfun<>0 then
    //path=get_subobj_path(corinv(kfun))
    //if size(path)==1 then path=path(1),end
    path=corinv(kfun)
    xset('window',curwin)
    bad_connection(path,..
	['Simulation problem with hilited block';
	'see message in scilab window'])
  else
    message('Simulation problem, see message in scilab window')
  end
  ok=%f
end
xset('window',curwin)
disp(timer())
unsetmenu(curwin,'stop')
enablemenus()
needreplay=resume(needreplay)
endfunction
