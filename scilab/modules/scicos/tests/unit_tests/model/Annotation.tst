// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

loadXcosLibs();

p = funcprot();
funcprot(0);
// Overload scicos_getvalue and xstringl to customize parameters
function [ok,txt,font,siz,exprs] = scicos_getvalue(title, fields, types, exprs)
    ok = %t;
    txt = "newText";
    font = 3;
    siz = 4;
    exprs = [txt; string(font); string(siz)];
endfunction
function r = xstringl(a, b, c, d, e)
    r = [0 0 3 4];
endfunction
funcprot(p);

// Allocate an Annotation
o = TEXT_f("define")
o.graphics
o.model
o.void
o.gui

// Modify it
o = TEXT_f("set", o)
o.graphics
o.model
o.void
o.gui
