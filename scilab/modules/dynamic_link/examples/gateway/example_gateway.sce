

// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

exec(SCI+"/modules/dynamic_link/examples/gateway/builder.sce");
exec(SCI+"/modules/dynamic_link/examples/gateway/loader.sce");

A=ones(2,2);B=ones(2,2);
C=matmul(A,B);
if norm(A*B-matmul(A,B)) > %eps then pause,end

D="test string";
view(A,C,D);