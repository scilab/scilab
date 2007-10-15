function SaveAs_()
// Copyright INRIA
  Cmenu=[]

  if super_block then
    r = x_choose(['Diagram';'Super Block'],..
	         ['Save content of the Super Block or'
		  'the complete diagram?'],'Cancel')
    if r==0 then 
      return
    end
    if r==1 then
      Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
		       'Cmenu='"Save As'";%scicos_navig=[]';
		       '%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1';
		      ]
      return
    end
  end

  [scs_m,editedx]=do_SaveAs()
  if ~super_block then edited=editedx;end

endfunction
