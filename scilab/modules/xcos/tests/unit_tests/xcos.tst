// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- NO CHECK ERROR OUTPUT -->

defaultlibs = ['Branching',
'Branching',
'Events',
'Misc',
'Sinks',
'Threshold',
'Linear',
'Matrixop',
'Nonlinear',
'Sources',
'Electrical',
'Hydraulics',
'PDE'];

defaultlibs = defaultlibs + 'lib';
for i = size(defaultlibs,'*')
 if isdef(defaultlibs(i)) then pause,end
end

// check that macros are loaded @ scilab startup
if whereis('shape') <> [] then pause,end

// we launch xcos then libs should be loaded
xcos();
if whereis('shape') <> "PDElib" then pause,end

for i = size(defaultlibs,'*')
 if ~isdef(defaultlibs(i)) then pause,end
end

