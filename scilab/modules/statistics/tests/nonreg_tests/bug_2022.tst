//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA -Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2022 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2022
//
// <-- Short Description -->
//    percentile function (perctl) doesn't always calculate percentile value, instead
//    choosing to error out:
//
//    Example that causes error:
//
//    perctl([23.4,364.34,1929.3,4234.3,293.3,928.3],[10])

val1 = perctl([500 480 460 453 452 452 451 450 450 431 421 420 420 417 416 414 410 410 405 397 380 360 332],90);
if or( val1 <> [472 3]  ) then pause,end

val2 = perctl([500 480 460 453 452 452 451 450 450 431 421 420 420 417 416 414 410 410 405 397 380 360 332],75);
if (or(val2 <> [452 5]) &  or(val2 <> [452 6]) ) then pause,end

val3 = perctl([500 480 460 453 452 452 451 450 450 431 421 420 420 417 416 414 410 410 405 397 380 360 332],10);
if or( val3 <> [368 22] ) then pause,end

val4 = perctl([500 480 460 453 452 452 451 450 450 431 421 420 420 417 416 414 410 410 405 397 380 360 332],1);
if or( val4  <> [332 23] ) then pause,end

val5 = perctl([500 480 460 453 452 452 451 450 450 431 421 420 420 417 416 414 410 410 405 397 380 360 332],2);
if or( val5  <> [332 23] ) then pause,end

val6 = perctl([500 480 460 453 452 452 451 450 450 431 421 420 420 417 416 414 410 410 405 397 380 360 332],99);
if or( val6 <> [500 1]  ) then pause,end

val = perctl([23.4,364.34,1929.3,4234.3,293.3,928.3],[10]);
if or( val <> [23.4 1] ) then pause,end
