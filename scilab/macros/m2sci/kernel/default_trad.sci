function tree=default_trad(tree)
// Copyright INRIA
// M2SCI function
// Create a default translation function
// V.C.

global("mtlbref_fun") //contains the matlab reference functions which not yet converted 
global("mtlbtool_fun")//contains the matlab  toolboxes functions  
global("not_mtlb_fun") // contains the not matlab functions

if ~exists('mtlbref_fun') then
  mtlb_fun=[]
end
if ~exists('mtlbtool_fun') then
  mtlbtool_fun=[]
end
if ~exists('not_mtlb_fun') then
  not_mtlb_fun=[]
end

name=tree.name

ispriminame=%f; 
//true if the name function is the name of scilab function primitive
if funptr(tree.name)<>0 then
  name1="%"+tree.name
  tree.name=name1
  ispriminame=%t;   
end  
//ismtlbfun is true if the the function is in a matlab toolboxe, mtlbpath is the path where is the function
[mtlbpath,ismtlbtoolfun]=mtlbtoolfun(name)
//Matlab reference functions
if or(name==not_yet_converted()) then
  set_infos("Matlab function "+name+" not yet converted, original calling sequence used",2)
  if ~or(name==mtlbref_fun(:,1)) then
    mtlbref_fun($+1,1)=name
    if ispriminame then
      mtlbref_fun($,2)=" (Warning name conflict: function name changed from "+name+" to "+name1+")"
    else
      mtlbref_fun($,2)=""
    end
  end   
  //Matlab toolboxes functions 
elseif ismtlbtoolfun then
  set_infos("Matlab toolbox(es) function "+name+" not converted, original calling sequence used",2)
  if ~or(name==mtlbtool_fun(:,1)) then
    mtlbtool_fun($+1,1)=name
    if ispriminame then
      mtlbtool_fun($,2)="(Warning Name conflict: function name changed from "+name+" to "+name1+", find this function in matlab/"+mtlbpath+")" 
    else
      mtlbtool_fun($,2)="(Find this function in "+"matlab/"+ mtlbpath+")"  
    end
  end 
elseif isdefinedvar(Variable(tree.name,Infer())) then
  operands=list()
  operands(1)=Variable(tree.name,Infer())
  for krhs=1:lstsize(tree.rhs)
    operands($+1)=tree.rhs(krhs)
  end
  tree=Operation("ext",operands,tree.lhs)
  tree=operation2sci(tree)

  //Not matlbb function
else 
  set_infos("Unknown function "+name+" not converted, original calling sequence used",2)  
  if ~or(name==not_mtlb_fun(:,1)) then
    not_mtlb_fun($+1,1)=name
    if ispriminame then 
      not_mtlb_fun($,2)="(Warning Name conflict: function name changed from "+name+" to "+name1+")"
    else
      not_mtlb_fun($,2)=""
    end
  end
end
if ispriminame then
  set_infos("Name conflict: function name changed from "+name+" to "+name1,0)
end
[tree]=sci_generic(tree)
endfunction
















