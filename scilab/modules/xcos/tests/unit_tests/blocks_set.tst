// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- NO CHECK ERROR OUTPUT -->
//
// <-- Short Description -->
// Blocks must have valid dimensions for their settings.
// Some dimensions were not coherents between theirs "set" and "define" method.

loadScicosLibs();

defaultlibs = ["Branching",
	"Electrical",
	"Hydraulics",
	"Linear",
	"Misc",
	"Sources",
	"Events",
	"Integerop",
	"Matrixop",
	"Nonlinear",
	"Sinks",
	"Threshold"];

defaultlibs = defaultlibs + 'lib';

wrong_blocks = [];


funcprot(0);

// Stubbing the x_mdialog method
// checking it's arguments size only
function [result]=x_mdialog(title,labels,default_inputs_vector)
	result = x_dialog(labels,default_inputs_vector);
endfunction

// Stubbing the x_dialog method
// checking it's arguments size only
function [result]=x_dialog(labels,default_inputs_vector)
	if(and(size(labels) <> size(default_inputs_vector))) then pause, end;
	
	result = default_inputs_vector;
endfunction

// Stubbing the edit_curv method
function [xx, yy, ok, gc] = edit_curv(xx, yy,  axis, args, gc)
	ok = %t;
endfunction

// Stubbing the messagebox method
function [btn] = messagebox(msg, msgboxtitle, msgboxicon, buttons, ismodal)
	btn=1;
endfunction

for i = 1:size(defaultlibs,'*')
	[macros, path] = libraryinfo(defaultlibs(i));
	for j = 1:size(macros,'*')
		interfunction = macros(j);

		ierr = execstr("scs_m=" + interfunction + "(""define"", [], []);", "errcatch");
		ierr = execstr("scs_m=" + interfunction + "(""set"", scs_m, []);", "errcatch");
	end
end

