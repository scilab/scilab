

// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

exec(SCI+"/modules/dynamic_link/examples/gateway/builder.sce");
exec(SCI+"/modules/dynamic_link/examples/gateway/loader.sce");

A=ones(2,2);B=ones(2,2);
C=matmul(A,B);
if norm(A*B-matmul(A,B)) > %eps then pause,end

D="test string";
view(A,C,D);