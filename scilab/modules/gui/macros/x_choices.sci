function [rep] = x_choices(Title,listOfChoices)
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

[lhs,rhs] = argn(0)

// Tests/Demo code
if rhs<=0 then
  s_mat = ["l1=list(''choice 1'',1,[''toggle c1'',''toggle c2'',''toggle c3'']);";
      "l2=list(''choice 2'',2,[''toggle d1'',''toggle d2'',''toggle d3'']);";
      "l3=list(''choice 3'',3,[''toggle e1'',''toggle e2'']);";
      "rep=x_choices(''Toggle Menu'',list(l1,l2,l3));"];
  // Commands display
  for l=1:size(s_mat,1)
    mprintf(s_mat(l) + "\n");
  end
  // Commands execution
  execstr(s_mat);
  return;
end;

// Check number of inputs
if rhs ~=2 then
  error(msprintf(_("%s: Wrong number of input arguments: 2 expected.\n"), "x_choices"));
end

// Type for title
if typeof(Title)~="string" then
  error(msprintf(_("%s: Wrong type for first input argument: String expected.\n"), "x_choices"));
end;
// Type for items
if typeof(listOfChoices)~="list" then
  error(msprintf(_("%s: Wrong type for second input argument: List expected.\n"), "x_choices"));
end;

n = size(listOfChoices);
// Labels & buttons names
items = "void";
// Initial selected buttons
defv = [];
for i = 1:n
  currentList = listOfChoices(i);
  
  // Check that first choose is a list
  if typeof(currentList)~="list" then
    error(_("x_choices(t,x): Wrong type for x("+string(i)+"): List expected."));
  end
  
  // Check that first item (row label in the GUI) is a string
  if typeof(currentList(1))~="string" then
    error(_("x_choices(t,x): Wrong type for x("+string(i)+")(1): String expected."));
  end
  items = [items,currentList(1)];
  
  // Button names must be a row string vector
  if typeof(currentList(3))~="string" then
    error(_("x_choices(t,x): Wrong type for x("+string(i)+")(3): Row string vector expected."));
  end
  [nbLines,nbCols] = size(currentList(3));
  if nbLines~=1 then
    error(_("x_choices(t,x): Wrong type for x("+string(i)+")(3): Row string vector expected."));
  end
  items = [items,currentList(3)];

  // Default button selected
  if typeof(currentList(2))~="constant" then
    error(_("x_choices(t,x): Wrong type for x("+string(i)+")(2): Single double value expected."));
  end
  if size(currentList(2), "*")~=1 then
    error(_("x_choices(t,x): Wrong type for x("+string(i)+")(2): Single double value expected."));
  end
  defv = [defv,currentList(2)];
  
  // Add separator between labels of each line
  if n~=i then 
    items = [items,"[--sep--]"];
  end
end
// Remove "void" from the list of items
items = items(2:prod(size(items)))

// Call the interface to Java
rep = xchoicesi(defv,Title,items)
endfunction

