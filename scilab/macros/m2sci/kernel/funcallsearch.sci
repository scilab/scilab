function   [funcallname,variablename]=funcallsearch(instr,funcallname,fnamvect,variablename) 
// Copyright INRIA
//
//  FUNCALLSEARCH recursive function (used by "translatepaths" function)
//  Searches the functions names in each instruction of mtlbtree 
//  Input-Output
//  -funcallname : a vector which contains the names of called functions if it exists a M-file having the same name in the Paths
//  -variablename : a vector which contains the names of declared variables
//  Input 
//  -instr : mtlbtree instruction
//  -fnamvect : vector which contains all M-files names found in the Paths

// case : ifthenelse instruction
if typeof(instr) == "ifthenelse" then
  [funcallname,variablename]=funcallsearch(instr.expression,funcallname,fnamvect,variablename)
  for i=1:size(instr.then)
    [funcallname,variablename]=funcallsearch((instr.then(i)),funcallname,fnamvect,variablename)
  end
  for i=1:size(instr.elseifs)
    for k=1:size(instr.elseifs(i).then)
      [funcallname,variablename]=funcallsearch((instr.elseifs(i).then(k)),funcallname,fnamvect,variablename) 
    end
  end  
  for i=1:size(instr.else)
    [funcallname,variablename]=funcallsearch((instr.else(i)),funcallname,fnamvect,variablename)
  end
// case : selectcase instruction 
elseif typeof(instr) == "selectcase" then
  [funcallname,variablename]=funcallsearch(instr.expression,funcallname,fnamvect,variablename)
  
  for i=1:size(instr.cases)
  [funcallname,variablename]=funcallsearch((instr.cases(i).expression),funcallname,fnamvect,variablename)
    for j=1:size(instr.cases(i).then)
      [funcallname,variablename]=funcallsearch((instr.cases(i).then(j)),funcallname,fnamvect,variablename)
    end   
  end
  for i=1:size(instr.else)
    [funcallname,variablename]=funcallsearch(instr.else(i),funcallname,fnamvect,variablename)
  end
// case : while instruction   
elseif typeof(instr) == "while" then
  [funcallname,variablename]=funcallsearch(instr.expression,funcallname,fnamvect,variablename)
  for i=1:size(instr.statements)
    [funcallname,variablename]=funcallsearch(instr.statements(i),funcallname,fnamvect,variablename)
  end
// case : for instruction    
elseif typeof(instr) == "for" then
    [funcallname,variablename]=funcallsearch(instr.expression,funcallname,fnamvect,variablename)
for i=1:size(instr.statements)
  [funcallname,variablename]=funcallsearch(instr.statements(i),funcallname,fnamvect,variablename)
end
// case : cste instruction
elseif  typeof(instr)== "cste" then
return
// case : variable instruction
elseif typeof(instr)=="variable" 
  if find(instr.name==variablename)==[] & find(instr.name==stripblanks(part(fnamvect,1:24)))<>[] & find(instr.name==funcallname)==[] then
    funcallname=[funcallname;fnamvect(find(instr.name==stripblanks(part(fnamvect,1:24))))]
  else
    return
  end
// case : equal instruction
elseif typeof(instr) == "equal" then
  [funcallname,variablename]=funcallsearch(instr.expression,funcallname,fnamvect,variablename)
// case : expression is a funcall
elseif typeof(instr) == "funcall" then 
  if find(funcallname==instr.name) == [] & find(instr.name==stripblanks(part(fnamvect,1:24)))<>[]  then
    if size(find(instr.name==stripblanks(part(fnamvect,1:24))),2)==1 then
      funcallname=[funcallname;fnamvect(find(instr.name==stripblanks(part(fnamvect,1:24))))]
    else
      findvect=find(instr.name==stripblanks(part(fnamvect,1:24)))
      funcallname=[funcallname;fnamvect(findvect(2))]
      st = " " + mtlbtree.name + " : " + fnamvect(findvect(1))
      for i=2:size(findvect,2)  
        st = st+ " <-> " + fnamvect(findvect(i)) 
      end
        st = st + " : The 24 first characters of the files names  are equal : "
        warning(st)
    end  
  end
  // case : expression is cste
  if typeof(instr.rhs)== "constant" then
    return
  else
    for ind=1:size(instr.rhs)
      [funcallname,variablename]=funcallsearch(instr.rhs(ind),funcallname,fnamvect,variablename)
    end
  end
// case : operation instruction
elseif typeof(instr) == "operation" then
  for ind=1:size(instr.operands)
    [funcallname,variablename]=funcallsearch(instr.operands(ind),funcallname,fnamvect,variablename)
  end
end

endfunction