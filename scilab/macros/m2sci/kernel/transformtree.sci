function instr=transformtree(instr)
//Copyright INRIA
//TRANSFORMTREE function
//Find and replace : 

Unknown=-1;
if typeof(instr)=="ifthenelse" then
  for i=1:size(instr.then)
    instr.then(i)=transformtree((instr.then(i)))
  end
  for i=1:size(instr.elseifs)
  for k=1:size(instr.elseifs(i).then)
  instr.elseifs(i).then(k)=transformtree((instr.elseifs(i).then(k))) 
end
end  
for i=1:size(instr.else)
instr.else(i)=transformtree((instr.else(i)))
end
elseif typeof(instr)=="selectcase" then
  for i=1:size(instr.cases)
    for j=1:size(instr.cases(i).then)
      instr.cases(i).then(j)=transformtree((instr.cases(i).then(j)))
    end   
  end
  for i=1:size(instr.else)
  instr.else(i)=transformtree(instr.else(i))
end
elseif typeof(instr)=="while" then
  for i=1:size(instr.statements)
    instr.statements(i)=transformtree(instr.statements(i))
  end
elseif typeof(instr)=="for" then
  for i=1:size(instr.statements)
    instr.statements(i)=transformtree(instr.statements(i))
  end
elseif typeof(instr)=="equal" then
  if typeof(instr.expression)=="funcall" then
    nb_opr=0;
    for ind=1:size(instr.lhs)
      if typeof(instr.lhs(ind))=="operation" then
	nb_opr=nb_opr+1
      end
    end
    if nb_opr>1 then
      sup_instr=list("");
      lhstemp=list();
      for j=1:size(instr.lhs)
	if typeof(instr.lhs(j))=="operation" then
	  x=gettempvar();
	  sup_instr($+1)=Equal(list(instr.lhs(j)),x);
	  lhstemp(j)=x;
	else
	  lhstemp(j)=instr.lhs(j)
	end	   
      end
      sup_instr(1)=Equal(lhstemp,instr.expression)
      instr=tlist(["sup_equal","sup_instr","nb_opr"],sup_instr,nb_opr)
    end
  end 
end