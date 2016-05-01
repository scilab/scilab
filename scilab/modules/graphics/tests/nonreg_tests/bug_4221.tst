// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4221 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4221
//
// <-- Short Description -->
// graypolarplot uses to much memory.
//

rho=1:256;
theta=(1:360)./180.*%pi;
z=30+round(theta'*(1+rho.^2));
f=gcf();
f.color_map= hotcolormap(128);
clf();
graypolarplot(theta,rho,z);
xdel(winsid());

// It's just a 256x360 matrix, it should not complain about memory
