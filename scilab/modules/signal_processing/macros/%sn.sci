// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [y]=%sn(x,m)
//Jacobi 's elliptic function with parameter m
//which computes the inverse of the elliptic
//integral for the parameter m.
//x may be a vector.
//The amplitude is computed in fortran and apply
//the addition formulas for elliptic functions
//  x :A point inside the fundamental rectangle
//    :defined by the elliptic integral
//  m :Parameter of the elliptic integral (0<m<1)
//  y :Result
//
//!

[n1,n2]=size(x);
n=n1*n2;
a=amell(real(x),sqrt(m));
s=sin(a);
c=cos(a);
d=sqrt(ones(n1,n2)-m*s.*s);
m1=1-m;
a1=amell(imag(x),sqrt(m1));
s1=sin(a1);
c1=cos(a1);
d1=sqrt(ones(n1,n2)-m1*s1.*s1);
y=(s.*d1+%i*c.*d.*s1.*c1)./(c1.*c1+m*s.*s.*s1.*s1);
endfunction
