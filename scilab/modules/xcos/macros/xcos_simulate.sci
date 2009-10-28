//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function xcos_simulate(scs_m)
   //** extract tolerances from scs_m.props.tol
  tolerances = scs_m.props.tol ;
  //** extract solver type from tolerances
  solver = tolerances(6) ; 
  //** initialize a "scicos_debug_gr" variable
  %scicos_debug_gr = %f;

  ////////////////////////////////////////////////////////////////
  // Add global environment variable so that scicos is not lost //
  ////////////////////////////////////////////////////////////////
  %state0     = list();
  needcompile = 4;
  curwin      = 1000;
  %cpr        = struct();
  %tcur       = 0;
  %cpr.state  = %state0;
  tf          = scs_m.props.tf;
  %zoom       = 1.4;
  Select      = [];
  
  //** extract tolerances from scs_m.props.tol
  tolerances = scs_m.props.tol ;
  //** extract solver type from tolerances
  solver = tolerances(6) ; 

  // Propagate context through all blocks
  %scicos_context = struct();
  context = scs_m.props.context;
  //** context eval here
  [%scicos_context, ierr] = script2var(context, %scicos_context);
  
  //for backward compatibility for scifunc
  if ierr==0 then
    %mm = getfield(1,%scicos_context)
    for %mi=%mm(3:$)
      ierr = execstr(%mi+'=%scicos_context(%mi)','errcatch')
      if ierr<>0 then
	break; //** in case of error exit 
      end
    end
  end
  //end of for backward compatibility for scifuncpagate context values
  
  [scs_m,%cpr,needcompile,ok] = do_eval(scs_m, %cpr);
  
  
  //** update parameters or compilation results
  [%cpr,%state0_n,needcompile,alreadyran,ok] = do_update(%cpr,%state0,needcompile)
  
  //** if an error has ocurred in do_update
  //** then we exit from do_run
  if ~ok then
    %tcur      = []
    alreadyran = %f ;
    return
  end

  //** if alreadyran then set the var choice
  if alreadyran then
    choix = ['Continue';'Restart';'End']
  else
    choix = [] ;
  end

  issequal = %t;
  //** initial state has been changed
  if ~isequal(%state0_n,%state0) then
    issequal = %f
  else
    //** test typeof outtb element
    for i=1:lstsize(%state0_n.outtb)
      if typeof(%state0_n.outtb(i))<>typeof(%state0.outtb(i))
        issequal = %f
        break
      end
    end
    //** test typeof oz element
    for i=1:lstsize(%state0_n.oz)
      if typeof(%state0_n.oz(i))<>typeof(%state0.oz(i))
        issequal = %f
        break
      end
    end
  end

  //** if state have changed
  //** finish the simulation via do_terminate()
  if ~issequal then
     %state0 = %state0_n
    [alreadyran,%cpr] = do_terminate()
    choix = []
  end

  //** switch appropriate solver
  if %cpr.sim.xptr($)-1<size(%cpr.state.x,'*') & solver<100 then
    message(["Diagram has been compiled for implicit solver"
             "switching to implicit Solver"])
    solver = 100 ; //** Magic number 
    tolerances(6) = solver ; //** save Magic number solver type
  elseif (%cpr.sim.xptr($)-1==size(%cpr.state.x,'*')) & (solver==100 & size(%cpr.state.x,'*')<>0) then
    message(["Diagram has been compiled for explicit solver"
             "switching to explicit Solver"])
    solver = 0 ; //** Magic number 
    tolerances(6) = solver ; //** save Magic number solver type
  end

  //** ask user what to do
  if choix<>[] then
    //** open dialog box
    to_do = choose(choix,"What do you want to do")

    //** if cancel then exit
    if to_do==0 then
      ok = %f
      return
    end

    select choix(to_do)

      case "Continue" then 
        needstart = %f ;
        state     = %cpr.state ;

      case "Restart" then 
        needstart = %t ;
        state     = %state0 ;

      case "End" then 
        state     = %cpr.state ;
        needstart = %t ;
        tf        = scs_m.props.tf;

        //Alan: ONPEUTPASAPPELLERDOTERMINATEICI?
        //reponse : non, car do_terminate() ne rend
        //          pas forc�ment la main � l'utilisateur

        //** run scicosim via 'finish' flag
        ierr = execstr('[state,t]=scicosim(%cpr.state,%tcur,tf,%cpr.sim,'+..
                       '''finish'',tolerances)','errcatch')

        %cpr.state = state
        alreadyran = %f

        //** error case
        if ierr<>0 then
          str_err = split_lasterror(lasterror());
          kfun    = curblock()
          corinv  = %cpr.corinv

          if kfun<>0 then //** block error
            path = corinv(kfun)
            //** get error cmd for the block
            cmd = get_errorcmd(path,'End problem.',str_err);
            //** send error cmd to scicos via the Scicos_commands global variable
            global Scicos_commands ; 
            Scicos_commands = cmd;
          else //** simulator error
            message(["End problem:";str_err])
            //scf(curwin);
          end
          ok = %f
        end
       
        return
    end
  
  else //** Normal first start simulation 

    needstart = %t
    state     = %state0

  end

  //gh_win = gcf();

  //** scicos initialisation
  if needstart then
    //** if the simulation have already ran
    //** and is not finished then call do_terminate
    if alreadyran then
      [alreadyran,%cpr] = do_terminate()
      alreadyran = %f ;
    end
    //** set initial values for a new simulation
    %tcur      = 0
    %cpr.state = %state0
    tf         = scs_m.props.tf;
    if tf*tolerances==[] then 
      message(["Simulation parameters not set";"use setup button"]);
      return;
    end

    //** Run the normal first start simulation here 

    //** run scicosim via 'start' flag
    ierr = execstr('[state,t]=scicosim(%cpr.state,%tcur,tf,%cpr.sim,'+..
                   '''start'',tolerances)','errcatch')

    %cpr.state = state ; //** save the state 
    //** error case
    if ierr<>0 then
      str_err=split_lasterror(lasterror());

      kfun=curblock()
      corinv=%cpr.corinv
      if kfun<>0 then  //** block error
        path=corinv(kfun)
        //** get error cmd for the block
        cmd=get_errorcmd(path,'Initialisation problem.',str_err);
        //** send error cmd to scicos via the Scicos_commands global variable
        global Scicos_commands
        Scicos_commands=cmd;

      else //** simulator error
        message(['Initialisation problem:';str_err])
        //scf(curwin);
      end
      ok = %f;
      //xset('window',curwin)
      return
    end
    //scf(gh_win);
    //xset('window',win);
  end

  //** scicos simulation
  //needreplay=%t
  tf = scs_m.props.tf;
  //setmenu(curwin,'Stop')
  //timer()
  needreplay = %t

  //** run scicosim via 'start' flag
  ierr = execstr('[state,t]=scicosim(%cpr.state,%tcur,tf,%cpr.sim,'+..
	             '''run'',tolerances)','errcatch')

  %cpr.state = state
  
  //** no error
  if ierr==0 then
    alreadyran = %t;
    //** we are at the end of the simulation
    //** finish the simulation via do_terminate()
    if tf-t<tolerances(3) then
      //disp('fin');
      //Alan : j'enl�ve do_terminate ici car do_terminate
      //       ne rend pas la main
      //[alreadyran,%cpr]=do_terminate()
      needstart  = %t;
      alreadyran = %f;
      //** run scicosim via 'finish' flag
      ierr = execstr('[state,t]=scicosim(%cpr.state,tf,tf,%cpr.sim,'+..
                     '''finish'',tolerances)','errcatch')

      %cpr.state = state;

      //** error case
      if ierr<>0 then
        str_err = split_lasterror(lasterror());

        kfun   = curblock()
        corinv = %cpr.corinv

        if kfun<>0 then //** block error
          path = corinv(kfun)
          //** get error cmd for the block
          cmd = get_errorcmd(path,'End problem.',str_err);
          //** send error cmd to scicos via the Scicos_commands global variable
          global Scicos_commands
          Scicos_commands = cmd;
        else //** simulator error
          message(['End problem:';str_err])
          //scf(curwin);
        end
      end
    else
      %tcur = t;
    end
  //** error case
  else
    str_err = split_lasterror(lasterror());

    alreadyran = %f;
    kfun       = curblock();
    corinv     = %cpr.corinv;

    if kfun<>0 then //** block error
      path = corinv(kfun);
      //** get error cmd for the block
      cmd = get_errorcmd(path,"Simulation problem.",str_err);
      //** send error cmd to scicos via the Scicos_commands global variable
      global Scicos_commands;
      Scicos_commands = cmd;

    else //** simulateur error
      message(['Simulation problem:';str_err])
      //scf(curwin);
    end
    ok = %f;
  end

  needreplay = resume(needreplay);
endfunction

