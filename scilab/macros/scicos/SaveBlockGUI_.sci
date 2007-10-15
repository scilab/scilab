function SaveBlockGUI_()
// Copyright INRIA
  if size(find(Select(:,2)==curwin),2)<>1 then
     message('Select one and only one block in the current window.')
     Cmenu=[]
     return
  else
    if size(scs_m.objs)<Select(1) | typeof(scs_m.objs(Select(1)))<>"Block" then
       Select=[]
       return
    else
       Cmenu=[]
       if scs_m.objs(Select(1)).gui<>'DSUPER' then
          message('Only Masked blocks can be saved.')
       else
       	  fname=do_saveblockgui(scs_m.objs(Select(1)))
          if fname<>emptystr() then
            Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
                             'exec('+sci2exp(fname)+');%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1';
		            ]
          end
       end
    end
  end
endfunction

