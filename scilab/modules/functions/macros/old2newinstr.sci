function instr=old2newinstr(instr,oldname,newname)
// This function replaces by a new name (given in a input argument:newname) in a Scilab instruction all the variables names and functions names matching to a given name in a input argument:oldname
// INPUTS:
// -instr: Scilab instruction
// -oldname: a string, matching to the name variable which must be replaced
// -newname: a string, matching to the new variable name 
// OUTPUT:
// -instr: Scilab instruction after modification
//
// Author: F.Belahcene
// Copyright INRIA

// VARIABLE tlist //
// If the variable name matches to the oldname argument then replace the variable name by the newname argument
if typeof(instr)=="variable" then
	if instr.name==oldname then
		instr.name=newname
	end
// FUNCALL tlist //
// If the function name matches to oldname argument then replace the function name by newname argument
elseif typeof(instr)=="funcall"
	if instr.name==oldname then
		instr.name=newname
	end
	// ex: function return has not rhs: return.rhs is not a list  
	if typeof(instr.rhs)=="list" then
		for i=1:size(instr.rhs)
			instr.rhs(i)=old2newinstr(instr.rhs(i),oldname,newname)
		end
	end
// OPERATION tlist//
elseif typeof(instr)=="operation" then
	for i=1:size(instr.operands)
		instr.operands(i)=old2newinstr(instr.operands(i),oldname,newname)
	end
// IF-THEN-ELSE instruction //
elseif typeof(instr)=="ifthenelse" then
	instr.expression=old2newinstr(instr.expression,oldname,newname)
	for i=1:size(instr.then)
		instr.then(i)=old2newinstr(instr.then(i),oldname,newname)
	end
	for i=1:size(instr.elseifs)
		for k=1:size(instr.elseifs(i).then)
			instr.elseifs(i).then(k)=old2newinstr((instr.elseifs(i).then(k)),oldname,newname) 
		end
	end  
	for i=1:size(instr.else)
		instr.else(i)=old2newinstr((instr.else(i)),oldname,newname)
	end
// SELECT-CASE instruction //	
elseif typeof(instr)=="selectcase" then
	instr.expression=old2newinstr(instr.expression,oldname,newname)
	for i=1:size(instr.cases)
		for j=1:size(instr.cases(i).then)
			instr.cases(i).then(j)=old2newinstr((instr.cases(i).then(j)),oldname,newname)
		end   
	end
	for i=1:size(instr.else)
		instr.else(i)=old2newinstr(instr.else(i),oldname,newname)
	end
// WHILE instruction //	
elseif typeof(instr)=="while" then
	instr.expression=old2newinstr(instr.expression,oldname,newname)
	for i=1:size(instr.statements)
		instr.statements(i)=old2newinstr(instr.statements(i),oldname,newname)
	end	
// TRY-CATCH instruction //	
elseif typeof(instr)=="trycatch"
	for i=1:size(instr.trystat)
		instr.trystat(i)=old2newinstr(instr.trystat(i),oldname,newname)
	end
	for i=1:size(instr.catchstat)
		instr.catchstat(i)=old2newinstr(instr.catchstat(i),oldname,newname)
	end
// FOR instruction //	
elseif typeof(instr)=="for" then
	instr.expression=old2newinstr(instr.expression,oldname,newname)
	for i=1:size(instr.statements)
		instr.statements(i)=old2newinstr(instr.statements(i),oldname,newname)
	end
// EQUAL instruction //
elseif typeof(instr)=="equal" then
	instr.expression=old2newinstr(instr.expression,oldname,newname)
	for i=1:size(instr.lhs)
		instr.lhs(i)=old2newinstr(instr.lhs(i),oldname,newname)
	end
end
endfunction