// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test is_handle_valid function

// check that current objects are valid
test = is_handle_valid([gcf(), gca(), gce()]);
if (test <> [%t, %t, %t]) then pause; end;

// create 11 polylines
plot([0:10; 0:10; 0:10], [0:10; 0:0.5:5; 0:2:20]);
    
// check polylines validity
axes = gca();
polylines = axes.children(1).children
test = is_handle_valid(polylines);
if (test <> [%t; %t; %t; %t; %t; %t; %t; %t; %t; %t; %t]) then pause; end
    
// delete some polylines
delete(polylines(3:7));
// print validity
test = is_handle_valid(polylines)
if (test <> [%t; %t; %f; %f; %f; %f; %f; %t; %t; %t; %t]) then pause; end

