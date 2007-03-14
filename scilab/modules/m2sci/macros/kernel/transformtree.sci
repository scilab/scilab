function instr=transformtree(instr)
//Copyright INRIA
//TRANSFORMTREE function
//This function research and transform the equal instructions(if the lhs are a multi_operation and expression is a funcall) 
//of the matlab tree to a sup_equal instructions
//sup_equal is a tlist : tlist([sup_equal,sup_instr,nb_op],sup_instr,nb_op)
//i.e : the equal instruction [a(1),b(2:3)]=f() is replaced by 
//sup_equal, whith sup_intr list is composed to : 
//[%v1,%v2]=f()
//a(1)=%v1
//b(2:3)=%v2
//and nb_op is: the number of insert operation (in this case 2)
//Input
//instr : instruction of matlab tree before tranformation 
//Output
//instr : instruction of matlab tree after transformation
//F.B

Unknown=-1;
// Browse all the instrucions of the matlab tree:
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
//instruction is an equal instruction
elseif typeof(instr)=="equal" then
  if typeof(instr.expression)=="funcall" then //expression is a funcall
    nb_opr=0;
    for ind=1:size(instr.lhs)
      if typeof(instr.lhs(ind))=="operation" then
	nb_opr=nb_opr+1
      end
    end
    if nb_opr>1 then //more than one lhs insert operation
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
//creation of the sup_equal
      instr=tlist(["sup_equal","sup_instr","nb_opr"],sup_instr,nb_opr)
    end
  end 
end
endfunction