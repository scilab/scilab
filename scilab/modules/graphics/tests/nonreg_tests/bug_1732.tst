// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1732 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1732
//
// <-- Short Description -->
//    The field Axes.sub_tics does not correctly works under Linux, but correctly
//    works un Windows.
//
//    Jacques-Deric

ref                = [1,0,5] ;
ha                 = gca () ;
ha.axes_visible    = "on" ;
ha.view            = "3d" ;
ha.sub_ticks       = ref ;
res1               = ha.sub_ticks ;

// mask Z axis
ha.rotation_angles = [1,30];
res2               = ha.sub_ticks ;

// check the sub_ticks were not modified

if or(res1<>ref) then pause,end
if or(res2<>ref) then pause,end
