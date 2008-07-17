// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3241 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3241
//
// <-- Short Description -->
// length function does not work anymore with polynomials, boolean, integers.
// in scilab 4 length(x) was similar to size(x) for these data types


if length(%s) <> size(%s,'*') then pause,end
if length(%t) <> size(%s,'*') then pause,end
if length(int32(1)) <> size(int32(1),'*') then pause,end
if length(int16(2)) <> size(int16(2),'*') then pause,end
if length(int8(3)) <> size(int8(3),'*') then pause,end

if length(%s) <> 1 then pause,end
if length(%t) <> 1 then pause,end
if length(int32(1)) <> 1 then pause,end
if length(int16(2)) <> 1 then pause,end
if length(int8(3)) <> 1 then pause,end

if length([%s,%s,%s;%s,%s,%s]) <> size([%s,%s,%s;%s,%s,%s],'*') then pause,end
if length([%t,%f,%t;%t,%f,%t]) <> size([%t,%f,%t;%t,%f,%t],'*') then pause,end
if length([int32(1),int32(2),int32(3);int32(1),int32(2),int32(3)]) <> size([int32(1),int32(2),int32(3);int32(1),int32(2),int32(3)],'*') then pause,end
if length([int16(1),int16(2),int16(3);int16(1),int16(2),int16(3)]) <> size([int16(1),int16(2),int16(3);int16(1),int16(2),int16(3)],'*') then pause,end
if length([int8(1),int8(2),int8(3);int8(1),int8(2),int8(3)]) <> size([int8(1),int8(2),int8(3);int8(1),int8(2),int8(3)],'*')  then pause,end

if length([%s,%s,%s;%s,%s,%s]) <> 6 then pause,end
if length([%t,%f,%t;%t,%f,%t]) <> 6 then pause,end
if length([int32(1),int32(2),int32(3);int32(1),int32(2),int32(3)]) <> 6 then pause,end
if length([int16(1),int16(2),int16(3);int16(1),int16(2),int16(3)]) <> 6 then pause,end
if length([int8(1),int8(2),int8(3);int8(1),int8(2),int8(3)]) <> 6 then pause,end

s = scf();
handle_Matrix = [s,s,s;s,s,s];
if length(s) <> 1 then pause,end
if length(s) <> size(s,'*') then pause,end

if length(handle_Matrix) <> size(handle_Matrix,'*') then pause,end
if length(handle_Matrix) <> 6 then pause,end
delete(s);
