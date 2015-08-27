//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


if inttype([]) <> 0 then pause,end
if inttype(%nan) <> 0 then pause,end
if inttype(%inf) <> 0 then pause, end

if inttype(int8(0)) <> 1 then pause,end
if inttype(uint8(0)) <> 11 then pause,end
if inttype(int16(0)) <> 2 then pause,end
if inttype(uint16(0)) <> 12 then pause,end
if inttype(int32(0)) <> 4 then pause,end
if inttype(uint32(0)) <> 14 then pause,end

// Introduced in Scilab 6.0
if inttype(int64(0)) <> 8 then pause,end
if inttype(uint64(0)) <> 18 then pause,end
