function ppath=getparpath(scs_m,bpath,ppath)
// getparpath - computes path to block parameter data structure in scicos structure
//%Syntax
//  ppath=getparpath(scs_m)  standard call
//  ppath=getparpath(scs_m,bpath,ppath) recursive call
//%Parameters
//   scs_m : scicos data structure
//   bpath : current path to scs_m
//   ppath : list, each element is a vector giving the path to a block
//           with non empty rpar or ipar or states
//!
// Copyright INRIA
excluded=['IN_f','OUT_f','CLKIN_f','CLKOUT_f','CLKINV_f','CLKOUTV_f']
[lhs,rhs]=argn(0)
if rhs<2 then bpath=[],end
if rhs<3 then ppath=list(),end
for k=2:size(scs_m)
  o=scs_m(k)
  if o(1)=='Block' then
    if and(o(5)<>excluded) then
      model=o(3)
      if model(1)=='super'|model(1)=='csuper' then
	o=get_tree_elt(scs_m,[k,3,8])
	ppath=getparpath(o,[bpath k],ppath)
      else
	if model(6)<>[]|model(7)<>[]|model(8)<>[]|model(9)<>[] then
	  ppath(size(ppath)+1)=[bpath k],
	end
      end
    end
  end
end


