// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Simple test for xpoly using the MVC implementation -->
//
// <-- Short Description -->
// Tests the Polyline MVC object via the xpoly function:
// -whether properties corresponding to the function parameters are correctly initialized
// -whether the coordinates array is correctly initialized and can be resized
//

x = sin(2*%pi*(0:4)/5);
y = cos(2*%pi*(0:4)/5);

xpoly(x,y,"lines",1);

e = gce();

if (e.line_mode <> "on") then pause, end;
if (e.closed <> "on") then pause, end;
if (e.mark_mode <> "off") then pause, end;

s = size(e.data);

if (s(1) <> size(x, '*')) then pause, end;
if (s(1) <> size(y, '*')) then pause, end;

// 2 columns expected [x, y]
if (s(2) <> 2) then pause, end;


// Coordinates array resized

x = sin(2*%pi*(0:1999)/2000);
y = sin(2*%pi*(0:1999)/2000);

e.data = [x', y'];

s = size(e.data);

if (s(1) <> size(x, '*')) then pause, end;
if (s(1) <> size(y, '*')) then pause, end;

// Z coordinates added
z = linspace(0, 1, 2000);

e.data = [x', y', z'];

s = size(e.data);

// 3 columns expected [x, y, z]
if (s(2) <> 3) then pause, end;

if (s(1) <> size(x, '*')) then pause, end;
if (s(1) <> size(y, '*')) then pause, end;
if (s(1) <> size(z, '*')) then pause, end;

// Deletes the data
e.data = [];

if (e.data <> []) then pause, end;

