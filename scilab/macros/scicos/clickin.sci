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
if o(1)=='Block' then
  model=o(3)  
  //find output port location projected on the block

  [%xout,%yout,typout]=getoutputs(o)
  if %xout<>[] then
    %xxyymax=o(2)(1)(:)+o(2)(2)(:)
    %xout=max(min(%xout,%xxyymax(1)),o(2)(1)(1))
    %yout=max(min(%yout,%xxyymax(2)),o(2)(1)(2))
    // how close to the port is considered a link (/5)
    if or(abs(%xc-%xout)<o(2)(2)(1)/5&abs(%yc-%yout)<o(2)(2)(2)/5) then
      //we have clicked near a port
      [Cmenu]=resume('Link')
    end
  end
      

  if model(1)=='super' then
    lastwin=curwin
    curwin=get_new_window(windows)
    xset('window',curwin)
    execstr('scs_m_'+string(slevel)+'=scs_m')
    execstr('[o_n,needcompile,newparameters]='+o(5)+'(''set'',o)')
    //edited variable is returned by SUPER_f
    modified=prod(size(newparameters))>0
    curwin=lastwin
    if(~(or(curwin==winsid()))) then o=o_n;Cmenu=resume('Open/Set');end
    xset('window',curwin)
    xselect()
  elseif model(1)=='csuper' then
    execstr('[o_n,needcompile,newparameters]='+o(5)+'(''set'',o)')
    modified=prod(size(newparameters))>0
    edited=modified
  else
    
    execstr('o_n='+o(5)+'(''set'',o)')

    edited=or(o<>o_n)

    model=o(3)
    model_n=o_n(3)
    modified=or(model(1)<>model_n(1))|..
	     or(model(6)<>model_n(6))|..
	     or(model(7)<>model_n(7))|..
	     or(model(8)<>model_n(8))|..
	     or(model(9)<>model_n(9))

    if or(model(2)<>model_n(2))|or(model(3)<>model_n(3)) then  
      // input or output port sizes changed
      needcompile=1
    end
    if or(model(11)<>model_n(11))  then 
      // initexe changed
      needcompile=2
    end
    if model(1)=='input'|model(1)=='output' then
      if model(9)<>model_n(9) then
	needcompile=4
      end
    end
    if or(model(10)<>model_n(10))|or(model(12)<>model_n(12))  then 
      // type 'c','d','z','l' or dep_ut changed
      needcompile=4
    end
    if prod(size(model_n(1)))>1 then
      if model_n(1)(2)>1000 then  // Fortran or C Block
	if model(1)(1)<>model_n(1)(1) then  //function name has changed
	  needcompile=4
	end
      end
    end
  end
  o=o_n
elseif o(1)=='Link' then
  
  [Cmenu]=resume('Link')

elseif o(1)=='Text' then
  execstr('o_n='+o(5)+'(''set'',o)')
  edited=or(o<>o_n)
  o=o_n
end





