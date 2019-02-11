// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// Unit test for tree2code()
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

fp = funcprot(0);

// Tests the output prototype:
// -------------------------
protos = [
"function test()"
"function test(x)"
"function test(x,y)"
"function test(x,y,varargin)"
"function [z] = test()"
"function [z] = test(x)"
"function [z] = test(x,y)"
"function [z] = test(x,y,varargin)"
"function [u,v] = test()"
"function [u,v] = test(x)"
"function [u,v] = test(x,y)"
"function [u,v] = test(x,y,varargin)"
"function [u,v,varargout] = test()"
"function [u,v,varargout] = test(x)"
"function [u,v,varargout] = test(x,y)"
"function [u,v,varargout] = test(x,y,varargin)"
];

for p = protos'
    execstr(p+", endfunction");
    txt = tree2code(macr2tree(test));
    assert_checkequal(txt(1), p);
end

funcprot(fp);

// Tests the indentation width
// ---------------------------
function test()
 disp(2)
endfunction
txt = tree2code(macr2tree(test), %t);
path = "//interface/scinotes/display/body/scinotes-display";
tmp = evstr(xmlGetValues(path, "indent-size", "SCIHOME/XConfiguration.xml"));
assert_checkequal(txt(2), blanks(tmp)+"disp(2)");
