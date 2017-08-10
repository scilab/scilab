//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for median function
// =============================================================================

if or( median([1 2 3; 4 5 6],'c') <> [2;5])              then pause, end
if or( median([1 2 3; 4 5 6],'r') <> [2.5 3.5 4.5])      then pause, end

if or( median([1, 2, 3]','r')     <> 2 )                 then pause, end
if or( median([1, 2, 3],'r')      <> [1 2 3])            then pause, end


if or( median([1, 2, 3]','c')     <> [1 ; 2 ; 3])        then pause, end
if or( median([1, 2, 3],'c')      <> 2)                  then pause, end


if or( median([1, 2, 3, 4]','c')  <> [1 ; 2 ; 3 ; 4])    then pause, end
if or( median([1, 2, 3, 4],'c')   <> 2.5)                then pause, end


if or( median([1, 2, 3, 4],'r')  <> [1 2 3 4])           then pause, end
if or( median([1, 2, 3, 4]','r')   <> 2.5)               then pause, end
