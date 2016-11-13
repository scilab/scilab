//<-- CLI SHELL MODE -->
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

iext = linspace(1,5,5);
ds = [ones(1:5),zeros(1:5)];
fg = linspace(0,0.5,10);
wt = ones(fg);
y = remez(iext,ds,fg,wt);

if max(abs(y - [0.500 0.852 0.000 0.000])) > 0.001 then pause, end


nc=21;
ngrid=nc*250;
fgrid=.5*(0:(ngrid-1))/(ngrid-1);
mag=ones(1:ngrid/2);
mag(ngrid/2+1:ngrid)=0*ones(1:ngrid/2);
weight=ones(fgrid);
guess=round(1:ngrid/nc:ngrid);
guess(nc+1)=ngrid;
guess(nc+2)=ngrid;
an=remez(guess,mag,fgrid,weight);
