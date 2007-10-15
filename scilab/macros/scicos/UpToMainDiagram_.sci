function UpToMainDiagram_()
// Copyright INRIA
  Cmenu=[]
  if super_block then
      Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
		       'Cmenu='"[]'";%scicos_navig=[]';
		       'xselect()'
		      ]
  else
      message('This is already the main diagram;')
  end
endfunction
