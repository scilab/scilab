function []=updatevarslist(instr_lhs)
// Copyright INRIA (2 functions in this file: merge_vars() at the end)
// Update list of M2SCI variables with converted instruction lhs
// Input:
// - instr_lhs: list of lhs of current instruction
// - in_a_clause: boolean value
//   Set to 1 if instruction is in a clause
//   In this case, type and dimensions are set to unknown if differ from those already stored in varslist
//   (Default value is %F)
// V.C.

// Global variable for M2SCI
global("varslist")

// level is declared in m2sci.sci and modified in clause2sci.sci
level;

rhs=argn(2)
if rhs==2 then
  in_a_clause=%F
end

// Special case when end of conversion of a clause
// Merge infered data from clause and those from level-1
if instr_lhs=="END OF CLAUSE" then
  while varslist($).level==level(1)
    
    // Search for last variable created in clause which ends here
    index=[]
    for k=size(varslist):-1:1
      if varslist(k).level==level(1) then
	index=k
	return
      end
    end
    
    // Search for same variable created before
    index_lower_level=[]
    for k=size(varslist):-1:1
      if varslist(k).matname==varslist(index) & varslist(k).level==level(1)-1 then
	index_lower_level=k
	return
      end
    end
    
    if index<>[] then // Found a variable created in this clause
      if index_lower_level<>[] then // Found same variable created before, then merge...
	merge_vars(index_lower_level,varslist(index))
        varslist(index)=null()
      else // Same variable not created before, change level of variable created in clause
        varslist(index).level=0
      end
    else
      break
    end
  end
  return
end

// Expression: lhs name is empty => nothing to do
if instr_lhs==list() then
  return
end

// Remove lhs which are not variables
k=1
while k<=size(instr_lhs)
  // Insertion operation
  if typeof(instr_lhs(k))=="operation" then
    instr_lhs(k)=null()
  // List of indices in recursive extraction
  //elseif typeof(instr_lhs(k))=="list" then
  //  instr_lhs(k)=null()
  else
    k=k+1
  end
end

if size(instr_lhs)==0 then
  return
end


// Update varslist  
for k=1:size(instr_lhs)
  [bval,index]=isdefinedvar(instr_lhs(k))

  if bval then
    if level(1)>0 then // If in a clause, create a new variable
      // Search if variable also exists for current level
      for l=1:size(varslist)
	if varslist(l).matname==instr_lhs(k).name & varslist(l).level==level(1) then
	  bval=%T
	  index=l
	  break
	end
      end
      
      // If variable exists for currentlevel then merge all data
      if bval then
	newvar=M2scivar(varslist(index).sciname,..
	    varslist(index).matname,..
	    instr_lhs(k).infer,..
	    level(1))
	merge_vars(index,newvar)
      else // Create new variable
	varslist($+1)=M2scivar(varslist(index).sciname,..
	    varslist(index).matname,..
	    instr_lhs(k).infer,..
	    level(1))
      end
    else // Else, update existing variable
      varslist(index)=M2scivar(varslist(index).sciname,..
	  varslist(index).matname,..
	  instr_lhs(k).infer,..
	  varslist(index).level)
    end
  else
    // Variable added to varslist if as a name (not done for expressions)
    if execstr("f=instr_lhs(k).name","errcatch")<>0 then pause;end;errclear();
    if instr_lhs(k).name<>"ans" then
      varslist($+1)=M2scivar(instr_lhs(k).name,..
	  instr_lhs(k).name,..
	  instr_lhs(k).infer,..
	  level(1))
    end
  end
end

endfunction


function []=merge_vars(oldvarindex,newvar)
// Copyright INRIA
// M2SCI function
// Merge two variables inference properties, if different then set to Unknown
// Input:
// - oldvarindex: index of old variable in varslist
// - newvar: new variable to take in account to update oldvar properties
// V.C.

// Global variable for M2SCI
global("varslist")

oldvar=varslist(oldvarindex)

olddims=oldvar.dims
oldvtype=oldvar.vtype
oldprop=oldvar.property

newdims=newvar.dims
newvtype=newvar.vtype
newprop=newvar.property
      
// Verify dims
for l=1:min(size(newdims),size(olddims))
  if newdims(l)<>olddims(l) then
    newdims(l)=Unknown
  end
end
if size(newdims)>size(olddims) then
  for l=size(olddims):size(newdims)
    newdims(l)=null()
  end
end

// Verify vtype
if newvtype<>oldvtype then
  newvtype=Unknown
end

// Verify property
if newprop<>oldprop then
  newprop=Unknown
end

// Write result in varslist
if type(newvar.contents)==15 then
  newvar.infer.contents=[]
end
varslist(oldvarindex)=M2scivar(oldvar.sciname,..
    oldvar.matname,..
    Infer(newdims,Type(newvtype,newprop),newvar.contents),..
    oldvar.level)

endfunction
