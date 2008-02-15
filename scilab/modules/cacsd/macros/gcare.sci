// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [X,F]=gcare(Sl)
//[X,F]=gcare(Sl)
//Generalized Control Algebraic Riccati Equation
//X = solution , F = gain
//!

[A,B,C,D]=Sl(2:5);
S=eye()+D'*D;R=eye()+D*D';
Si=inv(S);
Ar=A-B*Si*D'*C;
H=[Ar,-B*Si*B';
   -C'*inv(R)*C,-Ar'];
X=ric_desc(H);
F=-Si*(D'*C+B'*X)
endfunction
