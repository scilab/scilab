// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// non regression bug for graphic memory leak

plot3d();
a = gca();

beginFreeMemory = getmemory();

// rotate for a long time and find if there are memory leaks
for i = 1:3600,
  a.rotation_angles(2) = i;
end;

endFreeMemory = getmemory();

// not much should have been allocated.
memoryIncrease = beginFreeMemory - endFreeMemory;

// let say that the rotation should not use more than 10 Meg
if (memoryIncrease > 10000) then pause; end


