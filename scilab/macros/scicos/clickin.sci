function [o,modified,newparameters,needcompile,edited]=clickin(o)
//  o             : structure of clicked object, may be modified
//  modified      : boolean, indicates if simulation time parameters
//                  have been changed
//  newparameters : only defined for super blocks, gives pointers to
//                  sub-blocks for which parameters or states have been changed
//  needcompile   : indicates if modification implies a new compilation
//!
// Copyright INRIA

modified=%f;newparameters=list();needcompile=0;
if typeof(o)=='Block' then
  //find output port location projected on the block
  [%xout,%yout,typout]=getoutputs(o)
  if %xout<>[] then
    %xxyymax=o.graphics.orig(:)+o.graphics.sz(:)
     %xout=max(min(%xout,%xxyymax(1)),o.graphics.orig(1))
    %yout=max(min(%yout,%xxyymax(2)),o.graphics.orig(2))
    // how close to the port is considered a link (/5)
    if or(abs(%xc-%xout)<o.graphics.sz(1)/5&abs(%yc-%yout)<o.graphics.sz(2)/5) then
      //we have clicked near a port
      [Cmenu]=resume('Link')
    end
  end
      

  if o.model.sim=='super' then
    lastwin=curwin
    curwin=get_new_window(windows)
    xset('window',curwin)
    execstr('scs_m_'+string(slevel)+'=scs_m')
    execstr('[o_n,needcompile,newparameters]='+o.gui+'(''set'',o)')
    //edited variable is returned by SUPER_f -- NO LONGER TRUE
    edited=diffobjs(o,o_n)
    if edited then
      o=o_n
      modified=prod(size(newparameters))>0
    end
    curwin=lastwin
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
      modified=or(model.sim<>model_n.sim)|..
	       or(model.state<>model_n.state)|..
	       or(model.dstate<>model_n.dstate)|..
	       or(model.rpar<>model_n.rpar)|..
	       or(model.ipar<>model_n.ipar)
      if or(model.in<>model_n.in)|or(model.out<>model_n.out) then  
	// input or output port sizes changed
	needcompile=1
      end
      if or(model.firing<>model_n.firing)  then 
	// initexe changed
	needcompile=2
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
      if prod(size(model_n.sim))>1 then
        if model_n.sim(2)>1000 then  // Fortran or C Block
	  if model.sim(1)<>model_n.sim(1) then  //function name has changed
	    needcompile=4
	  end
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
