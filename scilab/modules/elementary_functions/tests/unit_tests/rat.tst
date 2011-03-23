// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// by reference
[n, d] = rat(%pi);
if n <> 355 then pause, end
if d <> 113 then pause, end

y = rat(%pi);
if y <> n/d then pause, end

[n, d] = rat(%pi, 1.d-12);
if n <> 1146408 then pause, end
if d <> 364913 then pause, end

// by value
[n, d] = rat(3.1415927)
if n <> 355 then pause, end
if d <> 113 then pause, end

y = rat(%pi);
if y <> n/d then pause, end

[n, d] = rat(3.1415927, 1.d-12);
if n <> 1640169 then pause, end
if d <> 522082 then pause, end

