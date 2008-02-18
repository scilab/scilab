// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Francois DELBECQUE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [Ns,d]=coffg(Fs)
// [Ns,d]=coffg(Fs) computes Fs^-1 where Fs is a polynomial
// or rational matrix by co-factors method.
// d = common denominator; Ns =  numerator (matrix polynomial)
// Fs inverse = Ns/d.
// (Be patient...results are generally reliable)
// See also det, detr, invr, penlaur, glever, leverrier
//!
//
if type(Fs)==2 then
   [n,np]=size(Fs);
   if n<>np then error('First argument to coffg must be square!');end
   d=det(Fs) // common denominator
   n1=n;
   for kk=1:n1,for l=1:n1,
     signe=(-1)^(kk+l);
     col=[1:kk-1,kk+1:n1];row=[1:l-1,l+1:n1];
     Ns(kk,l)=-signe*det(Fs(row,col))
    end;end
    Ns=-Ns;
elseif type(Fs)==16|type(Fs)==15
   [n,np]=size(Fs);
   if n<>np then error('First argument to coffg must be square!');end
   d=det(Fs) // common denominator
   n1=n;
   for kk=1:n1,for l=1:n1,
     signe=(-1)^(kk+l);
     col=[1:kk-1,kk+1:n1];row=[1:l-1,l+1:n1];
     Ns(kk,l)=-signe*det(Fs(row,col))
   end;end
   Ns=-Ns;
else
error('First argument to coffg is invalid');
end
endfunction
