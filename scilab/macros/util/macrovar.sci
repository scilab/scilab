function vars=macrovar(macro)
// Returns in a list the set of varibles used by a macro
//    mac  : macro
//    vars : list(in,out,globals,called,locals)
//           in : input variables
//           out: output variables
//           globals: global variables
//           called : macros called
//           locals : local variables
//!
//origin S Steer inria 1992
// Copyright INRIA
if type(macro)==11 then comp(macro),end
if type(macro)<>13 then error('Argument to macrovars must be a macro!'),end
lst=macr2lst(macro);
out=lst(2)',if prod(size(out))==0 then out =[],end
in=lst(3)'
vars=[]
getted=[]
[vars,getted]=listvars(lst)
ng=prod(size(getted))
globals=[],called=[]
for k=1:ng
  if (find(getted(k)==vars)==[])&(find(getted(k)==in)==[]) then 
    if whereis(getted(k))<>[] then
      called=[called;getted(k)]
    elseif exists(getted(k))==0 then
      globals=[globals;getted(k)]
    else
      w=null()
      w=evstr(getted(k))
      if type(w)==11|type(w)==13 then
        called=[called;getted(k)]
      else
        globals=[globals;getted(k)]
      end
    end
  end
end
locals=[]
nl=prod(size(vars))
for k=1:nl
  if (find(vars(k)==in)==[])&(find(vars(k)==out)==[]) then 
    locals=[locals;vars(k)]
  end
end
vars=list(in,out,globals,called,locals)

endfunction
function [vars,getted]=listvars(lst)
for lstk=lst
  if type(lstk)==15 then
    [vars,getted]=listvars(lstk)
  else
    if lstk(1)=='1'|lstk(1)=='for' then 
      // case 1 retained for 2.7 and earlier versions
       vars=[vars;addvar(lstk(2))],
    elseif lstk(1)=='29' then 
      nlhs=(size(lstk,'*')-2)/2
      for k=0:nlhs-1
	vars=[vars;addvar(lstk(3+2*k))],
      end
    elseif lstk(1)=='2' then 
       getted=[getted;addget(lstk(2))],
    end
  end
end

endfunction
function vnam=addvar(vnam)
if find(vnam==vars)<>[] then 
  vnam=[]
end

endfunction
function vnam=addget(vnam)
if find(vnam==getted)<>[] then 
  vnam=[]
end
endfunction
