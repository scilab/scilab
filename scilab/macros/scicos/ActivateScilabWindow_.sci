function ActivateScilabWindow_()
// Copyright INRIA

  Cmenu = [] ;
  %pt = []   ;



  if super_block then
    Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
		     'Cmenu='"Activate Scilab Window'";%scicos_navig=[]']
  else
    Cmenu = 'Leave'
  end
endfunction
