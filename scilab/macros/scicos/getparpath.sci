function ppath=getparpath(scs_m,bpath,ppath)
// Copyright INRIA
// getparpath - computes path to block parameter data structure in scicos structure
//%Syntax
//  ppath=getparpath(scs_m)  standard call
//  ppath=getparpath(scs_m,bpath,ppath) recursive call
//%Parameters
//   scs_m : scicos data structure
//   bpath : current path to scs_m
//   ppath : list, each element is a vector giving the path to a block
//           with non empty rpar or ipar or states

  excluded=['IN_f','OUT_f','CLKIN_f','CLKOUT_f','CLKINV_f','CLKOUTV_f']
  [lhs,rhs]=argn(0)
  if rhs<2 then bpath=[],end
  if rhs<3 then ppath=list(),end
  for k=1:lstsize(scs_m.objs)
    o=scs_m.objs(k)
    if typeof(o)=='Block' then
      if and(o.gui<>excluded) then
	model=o.model
	if model.sim=='super'| model.sim=='csuper' then
	  o=scs_m.objs(k).model.rpar
	  ppath=getparpath(o,[bpath k],ppath)
	else
	  if model.state<>[] | model.dstate<>[] | ..
		model.rpar<>[] | model.ipar<>[] | model.firing<>[]  then
	    ppath(size(ppath)+1)=[bpath k],
	  end
	end
      end
    end
  end
endfunction
