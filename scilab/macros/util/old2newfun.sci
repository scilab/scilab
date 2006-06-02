function old2newfun(fun,oldname,newname)
// This function replaces the variable (named oldname) of a file by a new name (which is newname). It uses the old2newinstr function
// INPUTS:
// -fun: a string, the name (with the whole path) of the modifed file
// -oldname: a string, the old name of the variable
// -newname: a string, the new name of the variable after modification 
//
// Author: F.Belahcene
// Copyright INRIA


if MSDOS
	sep="\";
else
	sep="/";
end

sep_ind=strindex(fun,sep);
dot_ind=strindex(fun,".");

// funname is the function name (without the path and the extension)
funname=part(fun,sep_ind($)+1:dot_ind($)-1);

// Compilation
if ~exists(funname)
	getf(fun,"c");
end

//Get the tree of the function
execstr("tree=macr2tree("+funname+")");

// Change oldname into newname in the tree 
if tree.name==oldname then
	tree.name=newname;
end
for i=1:size(tree.inputs)
	tree.inputs(i)=old2newinstr(tree.inputs(i),oldname,newname);
end
for i=1:size(tree.outputs)
	tree.outputs(i)=old2newinstr(tree.outputs(i),oldname,newname);
end

for i=1:size(tree.statements)
	tree.statements(i)=old2newinstr(tree.statements(i),oldname,newname);
end

// Get the matching code of the tree after modification
txt=tree2code(tree,%T);

// Replace the old code by the new code (which is txt) in the file  
//mputl(txt,fun);

endfunction