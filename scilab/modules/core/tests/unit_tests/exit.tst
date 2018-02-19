// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Try to find the path of Scilab executable
if (getos() <> "Windows") & (isempty(fileinfo(SCI + "/bin/scilab"))) then
  // Linux binary version or Linux packaged version
  SCIPATH = SCI + "/../../";
else
  // Linux source version
  // Windows source or binary version
  SCIPATH = SCI;
end

for i = 1:10
	exitcode = unix(SCIPATH + "/bin/scilab -nwni -e ""exit(" + string(i) + ")""");
    assert_checkequal(exitcode, i);
end
