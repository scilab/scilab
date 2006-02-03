function [sci_clause,nblines]=clause2sci(mtlb_clause,nblines,leveltemp)
// Copyright INRIA
// M2SCI function
// V.C.

// Global variables for M2SCI
global("m2sci_to_insert_b")
global("varslist")

// Increment level of clause indentation
level

// Temp variable used to store instructions to insert before clause
to_insert=list()
select typeof(mtlb_clause)
  // --- TRYCATCH ---
case "trycatch"
  level=[level;0]  
  
  // Get instructions to insert if there are
  if m2sci_to_insert_b<>list() then
    to_insert=m2sci_to_insert_b
    m2sci_to_insert_b=list()
  end
  
  // Convert try
  sci_try=list()
  level($)=level($)+1
 for k=1:size(mtlb_clause.trystat)   
    if typeof(mtlb_clause.trystat(k))=="sup_equal" then
      sci_try_temp=list()
      for i=1:size(mtlb_clause.trystat(k).sup_instr)
	[instr,nblines]=instruction2sci(mtlb_clause.trystat(k).sup_instr(i),nblines)
	sci_try_temp=update_instr_list(sci_try_temp,instr)
      end
      sci_try($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_try_temp,mtlb_clause.trystat(k).nb_opr)
    else
      [instr,nblines]=instruction2sci(mtlb_clause.trystat(k),nblines)
      sci_try=update_instr_list(sci_try,instr)
    end
  end
    
  // Convert catch
  sci_catch=list()
  level($)=level($)+1
 for k=1:size(mtlb_clause.catchstat)   
    if typeof(mtlb_clause.catchstat(k))=="sup_equal" then
      sci_catch_temp=list()
      for i=1:size(mtlb_clause.catchstat(k).sup_instr)
	[instr,nblines]=instruction2sci(mtlb_clause.catchstat(k).sup_instr(i),nblines)
	sci_catch_temp=update_instr_list(sci_catch_temp,instr)
      end
      sci_catch($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_catch_temp,mtlb_clause.catchstat(k).nb_opr)
    else
      [instr,nblines]=instruction2sci(mtlb_clause.catchstat(k),nblines)
      sci_catch=update_instr_list(sci_catch,instr)
    end
  end
  
// Create Scilab trycatch
sci_clause=tlist(["trycatch","trystat","catchstat"],sci_try,sci_catch)
level($)=level($)+1 
updatevarslist("END OF CLAUSE")

  // --- IF ---
case "ifthenelse"
  level=[level;0]  
  
  // Convert expression
  [sci_expr]=expression2sci(mtlb_clause.expression)
  
  // Get instructions to insert if there are
  if m2sci_to_insert_b<>list() then
    to_insert=m2sci_to_insert_b
    m2sci_to_insert_b=list()
  end
  
  // Convert then statements
  sci_then=list()
  level($)=level($)+1
 for k=1:size(mtlb_clause.then)   
    if typeof(mtlb_clause.then(k))=="sup_equal" then
      sci_then_temp=list()
      for i=1:size(mtlb_clause.then(k).sup_instr)
	[instr,nblines]=instruction2sci(mtlb_clause.then(k).sup_instr(i),nblines)
	sci_then_temp=update_instr_list(sci_then_temp,instr)
      end
      sci_then($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_then_temp,mtlb_clause.then(k).nb_opr)
    else
      [instr,nblines]=instruction2sci(mtlb_clause.then(k),nblines)
      sci_then=update_instr_list(sci_then,instr)
    end
  end
  
  // Convert elseifs
sci_elseifs=list()
  for k=1:size(mtlb_clause.elseifs)
  level($)=level($)+1
  
  // Convert expression
[sci_exprn]=expression2sci(mtlb_clause.elseifs(k).expression)
  
  // Get instructions to insert if there are
  if m2sci_to_insert_b<>list() then
    to_insert=m2sci_to_insert_b
    m2sci_to_insert_b=list()
  end
  
  // Convert statements
  sci_stat=list()
  for l=1:size(mtlb_clause.elseifs(k).then)
  if typeof(mtlb_clause.elseifs(k).then(l))=="sup_equal" then
  sci_stat_temp=list()
  for i=1:size(mtlb_clause.elseifs(k).then(l).sup_instr)
[instr,nblines]=instruction2sci(mtlb_clause.elseifs(k).then(l).sup_instr(i),nblines)
  sci_stat_temp=update_instr_list(sci_stat_temp,instr)
end
sci_stat($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_stat_temp,mtlb_clause.elseifs(k).then(l).nb_opr)
else
[instr,nblines]=instruction2sci(mtlb_clause.elseifs(k).then(l),nblines)
  sci_stat=update_instr_list(sci_stat,instr)
end
end
sci_elseifs($+1)=tlist(["elseif","expression","then"],sci_exprn,sci_stat)
end

// Convert else
sci_else=list()
  if size(mtlb_clause.else)<>0 then
  level($)=level($)+1 
end 
for k=1:size(mtlb_clause.else)
if typeof(mtlb_clause.else(k))=="sup_equal" then
sci_else_temp=list()
  for i=1:size(mtlb_clause.else(k).sup_instr)
[instr,nblines]=instruction2sci(mtlb_clause.else(k).sup_instr(i),nblines)
  sci_else_temp=update_instr_list(sci_else_temp,instr)
end
sci_else($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_else_temp,mtlb_clause.else(k).nb_opr)
else
[instr,nblines]=instruction2sci(mtlb_clause.else(k),nblines)
  sci_else=update_instr_list(sci_else,instr)
end
end

// Create Scilab ifthenelse
sci_clause=tlist(["ifthenelse","expression","then","elseifs","else"],sci_expr,sci_then,sci_elseifs,sci_else)
level($)=level($)+1 
updatevarslist("END OF CLAUSE")

// --- SELECT ---
case "selectcase"
  level=[level;0] 
  // Convert expression
  sci_expr=list()
  [sci_expr(1)]=expression2sci(mtlb_clause.expression(1))
  for i=2:size(mtlb_clause.expression)
    sci_expr(i)=mtlb_clause.expression(i) // EOL or comment
  end  
  
  // Get instructions to insert if there are
  if m2sci_to_insert_b<>list() then
    to_insert=m2sci_to_insert_b
    m2sci_to_insert_b=list()
  end
  
  // Convert cases
  sci_cases=list()
  k=0
  while k<size(mtlb_clause.cases)
    k=k+1
    level($)=level($)+1
    // Convert expression
    if typeof(mtlb_clause.cases(k).expression)=="funcall" then
      if mtlb_clause.cases(k).expression.name=="makecell" then
	// Insert new cases
	for nbcas=size(mtlb_clause.cases):-1:k+1
	  mtlb_clause.cases(nbcas+size(mtlb_clause.cases(k).expression.rhs))=mtlb_clause.cases(nbcas)
	end
	for nbrhs=1:size(mtlb_clause.cases(k).expression.rhs)
	  mtlb_clause.cases(nbrhs+k)=tlist(["case","expression","then"],mtlb_clause.cases(k).expression.rhs(nbrhs),mtlb_clause.cases(k).then)
	end
	mtlb_clause.cases(k)=null()
      end
    end
    [sci_exprn]=expression2sci(mtlb_clause.cases(k).expression)
    // Get instructions to insert if there are
    if m2sci_to_insert_b<>list() then
      to_insert=m2sci_to_insert_b
      m2sci_to_insert_b=list()
    end
    
    // Convert statements
    sci_stat=list()
    for l=1:size(mtlb_clause.cases(k).then)
      if typeof(mtlb_clause.cases(k).then(l))=="sup_equal" then
	sci_stat_temp=list()
	for i=1:size(mtlb_clause.cases(k).then(l).sup_instr)
	  [instr,nblines]=instruction2sci(mtlb_clause.cases(k).then(l).sup_instr(i),nblines)
	  sci_stat_temp=update_instr_list(sci_stat_temp,instr)
	end
	sci_stat($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_stat_temp,mtlb_clause.cases(k).then(l).nb_opr)
      else
	[instr,nblines]=instruction2sci(mtlb_clause.cases(k).then(l),nblines)
	sci_stat=update_instr_list(sci_stat,instr)
      end
    end
    sci_cases($+1)=tlist(["case","expression","then"],sci_exprn,sci_stat)
  end
  
  // Convert else
sci_else=list()
  if size(mtlb_clause.else)<>0 then
  level($)=level($)+1 
end 
for k=1:size(mtlb_clause.else)
if typeof(mtlb_clause.else(k))=="sup_equal" then
sci_else_temp=list();
  for i=1:size(mtlb_clause.else(k).sup_instr)
[instr,nblines]=instruction2sci(mtlb_clause.else(k).sup_instr(i),nblines)
  sci_else_temp=update_instr_list(sci_else_temp,instr)
end
sci_else($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_else_temp,mtlb_clause.else(k).nb_opr)
else
[instr,nblines]=instruction2sci(mtlb_clause.else(k),nblines)
  sci_else=update_instr_list(sci_else,instr)
end
end
// Create Scilab selectcase
sci_clause=tlist(["selectcase","expression","cases","else"],sci_expr,sci_cases,sci_else)
level($)=level($)+1 
updatevarslist("END OF CLAUSE")

// --- WHILE ---
case "while"
  level=[level;0] 
  sci_do=list()
  // Convert expression
  [sci_expr]=expression2sci(mtlb_clause.expression)
  // If there are instructions to insert, while is modified so that inserted instruction is evaluated in each loop
  if m2sci_to_insert_b<>list() then
    newif=tlist(["ifthenelse","expression","then","elseifs","else"],sci_expr,list(Funcall("break",1,list(),list())),list(),list())
    m2sci_to_insert_b($+1)=newif
    sci_expr=Cste(%T)
    sci_do=m2sci_to_insert_b
    m2sci_to_insert_b=list()
  end

  // Convert all do instructions
  level($)=level($)+1
  for k=1:size(mtlb_clause.statements)
    if typeof(mtlb_clause.statements(k))=="sup_equal" then
      sci_do_temp=list()
      for i=1:size(mtlb_clause.statements(k).sup_instr)
	[instr,nblines]=instruction2sci(mtlb_clause.statements(k).sup_instr(i),nblines)
	// If inserted instruction is an initialisation, it has to be done just one time and before loop
	l=1;
	while l<=lstsize(m2sci_to_insert_b)
	  if typeof(m2sci_to_insert_b(l))=="equal" & ..
	      (and(m2sci_to_insert_b(l).expression==Cste([])) | ..
	      and(m2sci_to_insert_b(l).expression==Funcall("struct",1,list(),list())) | ..
	      and(m2sci_to_insert_b(l).expression==Funcall("cell",1,list(),list())) ) then
	  to_insert($+1)=m2sci_to_insert_b(l)
	  m2sci_to_insert_b(l)=null()
	  if lstsize(m2sci_to_insert_b)>=l & m2sci_to_insert_b(l)==list("EOL") then
	    to_insert($+1)=m2sci_to_insert_b(l)
	    m2sci_to_insert_b(l)=null()
	  end
	else
	  l=l+1;
	end
      end
      sci_do_temp=update_instr_list(sci_do_temp,instr)
    end
    sci_do($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_do_temp,mtlb_clause.statements(k).nb_opr)
  else
    [instr,nblines]=instruction2sci(mtlb_clause.statements(k),nblines)
    // If inserted instruction is an initialisation, it has to be done just one time and before loop
    l=1;
    while l<=lstsize(m2sci_to_insert_b)
      if typeof(m2sci_to_insert_b(l))=="equal" & ..
	  (and(m2sci_to_insert_b(l).expression==Cste([])) | ..
	  and(m2sci_to_insert_b(l).expression==Funcall("struct",1,list(),list())) | ..
	  and(m2sci_to_insert_b(l).expression==Funcall("cell",1,list(),list())) ) then
      to_insert($+1)=m2sci_to_insert_b(l)
      m2sci_to_insert_b(l)=null()
      if lstsize(m2sci_to_insert_b)>=l & m2sci_to_insert_b(l)==list("EOL") then
	to_insert($+1)=m2sci_to_insert_b(l)
	m2sci_to_insert_b(l)=null()
      end
    else
      l=l+1;
    end
  end
  sci_do=update_instr_list(sci_do,instr)
end
end

// Create Scilab while
sci_clause=tlist(["while","expression","statements"],sci_expr,sci_do)
level($)=level($)+1 
updatevarslist("END OF CLAUSE")

// --- FOR ---
case "for"
  //level=[level;1]  
  // Convert expression
  [sci_expr,nblines]=instruction2sci(mtlb_clause.expression,nblines)
  if typeof(sci_expr)=="equal" then
    [bval,pos]=isdefinedvar(sci_expr.lhs(1))
    if bval then
      varslist(pos).infer.dims=list(varslist(pos).infer.dims(1),1)
    end
  end
  // Get instructions to insert if there are
  if m2sci_to_insert_b<>list() then
    to_insert=m2sci_to_insert_b
    m2sci_to_insert_b=list()
  end
  sci_instr=list()
  // Convert all do instructions
  for k=1:size(mtlb_clause.statements)
    if typeof(mtlb_clause.statements(k))=="sup_equal" then
      sci_instr_temp=list()
      for i=1:size(mtlb_clause.statements(k).sup_instr)
	[instr,nblines]=instruction2sci(mtlb_clause.statements(k).sup_instr(i),nblines)
	// If inserted instruction is an initialisation, it has to be done just one time and before loop
	l=1;
	while l<=lstsize(m2sci_to_insert_b)
	  if typeof(m2sci_to_insert_b(l))=="equal" & ..
	      (and(m2sci_to_insert_b(l).expression==Cste([])) | ..
	      and(m2sci_to_insert_b(l).expression==Funcall("struct",1,list(),list())) | ..
	      and(m2sci_to_insert_b(l).expression==Funcall("cell",1,list(),list())) ) then
	  to_insert($+1)=m2sci_to_insert_b(l)
	  m2sci_to_insert_b(l)=null()
	  if lstsize(m2sci_to_insert_b)>=l & m2sci_to_insert_b(l)==list("EOL") then
	    to_insert($+1)=m2sci_to_insert_b(l)
	    m2sci_to_insert_b(l)=null()
	  end
	else
	  l=l+1;
	end
      end
      sci_instr_temp=update_instr_list(sci_instr_temp,instr)
    end
    sci_instr($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_instr_temp,mtlb_clause.statements(k).nb_opr)
  else
    [instr,nblines]=instruction2sci(mtlb_clause.statements(k),nblines)
    // If inserted instruction is an initialisation, it has to be done just one time and before loop
    l=1;
    while l<=lstsize(m2sci_to_insert_b)
      if typeof(m2sci_to_insert_b(l))=="equal" & ..
	  (and(m2sci_to_insert_b(l).expression==Cste([])) | ..
	  and(m2sci_to_insert_b(l).expression==Funcall("struct",1,list(),list())) | ..
	  and(m2sci_to_insert_b(l).expression==Funcall("cell",1,list(),list())) ) then
      to_insert($+1)=m2sci_to_insert_b(l)
      m2sci_to_insert_b(l)=null()
      if lstsize(m2sci_to_insert_b)>=l & m2sci_to_insert_b(l)==list("EOL") then
	to_insert($+1)=m2sci_to_insert_b(l)
	m2sci_to_insert_b(l)=null()
      end
    else
      l=l+1;
    end
  end
  sci_instr=update_instr_list(sci_instr,instr)
end   
end
// Create Scilab while
sci_clause=tlist(["for","expression","statements"],sci_expr,sci_instr)
else
  error("clause2sci(): unknown clause type: "+typeof(mtlb_clause))
end
m2sci_to_insert_b=to_insert
if m2sci_to_insert_b<>list() then
  m2sci_to_insert_b($+1)=list("EOL");
end
endfunction
