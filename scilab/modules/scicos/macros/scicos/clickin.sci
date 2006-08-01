function [o,modified,newparameters,needcompile,edited]=clickin(o)
//  o             : structure of clicked object, may be modified
//  modified      : boolean, indicates if simulation time parameters
//                  have been changed
//  newparameters : only defined for super blocks, gives pointers to
//                  sub-blocks for which parameters or states have been changed
//  needcompile   : indicates if modification implies a new compilation
//!
// Copyright INRIA

if needcompile==4 then %cpr=list(),end  // for standard_document to work

modified=%f;newparameters=list();needcompile=0;
Cmenu=check_edge(o,Cmenu,%pt)
if Cmenu==('Link') then
  //we have clicked near a port
  [Cmenu]=resume('Link')
end

if typeof(o)=='Block' then  
  if o.model.sim=='super' then
    lastwin=curwin
    curwin=get_new_window(windows)
    xset('window',curwin)
    execstr('scs_m_'+string(slevel)+'=scs_m')
    execstr('[o_n,needcompile,newparameters]='+o.gui+'(''set'',o)')
    //edited variable is returned by SUPER_f -- NO LONGER TRUE
    if ~%exit then
      edited=diffobjs(o,o_n)
      if edited then
	o=o_n
	modified=prod(size(newparameters))>0
      end
    end
    
    curwin=lastwin
    if(~(or(curwin==winsid()))) then Cmenu=resume('Open/Set');end
    xset('window',curwin)
    xselect()
  elseif o.model.sim=='csuper' then
    execstr('[o_n,needcompile,newparameters]='+o.gui+'(''set'',o)')
    modified=prod(size(newparameters))>0
    edited=modified
    if edited then
      o=o_n
    end
  else
    execstr('o_n='+o.gui+'(''set'',o)')
    edited=or(o<>o_n)
    if edited then
      model=o.model
      model_n=o_n.model
      if ~is_modelica_block(o) then
	modified=or(model.sim<>model_n.sim)|..
		 ~isequal(model.state,model_n.state)|..
		 ~isequal(model.dstate,model_n.dstate)|..
		 ~isequal(model.rpar,model_n.rpar)|..
		 ~isequal(model.ipar,model_n.ipar)|..
		 ~isequal(model.label,model_n.label)
	if or(model.in<>model_n.in)|or(model.out<>model_n.out) then  
	  // input or output port sizes changed
	  needcompile=1
	end
	if or(model.firing<>model_n.firing)  then 
	  // initexe changed
	  needcompile=2
	end
	if (size(model.in,'*')<>size(model_n.in,'*'))|..
	      (size(model.out,'*')<>size(model_n.out,'*')) then  
	  // number of input or output  changed
	  needcompile=4
	end
	if model.sim=='input'|model.sim=='output' then
	  if model.ipar<>model_n.ipar then
	    needcompile=4
	  end
	end
	if or(model.blocktype<>model_n.blocktype)|..
	      or(model.dep_ut<>model_n.dep_ut)  then 
	  // type 'c','d','z','l' or dep_ut changed
	  needcompile=4
	end
	if (model.nzcross<>model_n.nzcross)|(model.nmode<>model_n.nmode) then 
	  // size of zero cross changed
	  needcompile=4
	end
	if prod(size(model_n.sim))>1 then
	  if model_n.sim(2)>1000 then  // Fortran or C Block
	    if model.sim(1)<>model_n.sim(1) then  //function name has changed
	      needcompile=4
	    end
	  end
	end
      else //implicit block
	//force compilation if an implicit block has been edited
	modified=or(model_n<>model)
	eq=model.equations;eqn=model_n.equations;
	if or(eq.model<>eqn.model)|or(eq.inputs<>eqn.inputs)|..
				      or(eq.outputs<>eqn.outputs) then  
	  needcompile=4
	end
      end
      o=o_n
    end
  end
elseif typeof(o)=='Link' then  
  [Cmenu]=resume('Link')
elseif typeof(o)=='Text' then
  execstr('o_n='+o.gui+'(''set'',o)')
  edited=or(o<>o_n)
  o=o_n
end
endfunction
