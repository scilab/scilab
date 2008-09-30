// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [xk]=mfft(x,flag,dim)
//<xk>=mfft(x,flag,dim)
//macro which calculates the fft for a multi-dimensional signal
// x    :x(i,j,k,...) input signal in the form of a row vector
//      :whose values are arranged so that the i index runs the
//      :quickest, followed by the j index, etc.
// flag :(-1) fft or (1) inverse fft
// dim  :dimension vector which gives the number of
//      :values of x for each of its indices
// xk   :output of multidimensional fft in same format as for x
//
//Example: For a three dimensional vector which has three points
//along its first dimension, two points along its second
//dimension and three points along its third dimension the row
//vector is arranged as follows
//
//      x=<x(1,1,1),x(2,1,1),x(3,1,1),
//         x(1,2,1),x(2,2,1),x(3,2,1),
//               x(1,1,2),x(2,1,2),x(3,1,2),
//               x(1,2,2),x(2,2,2),x(3,2,2),
//                     x(1,1,3),x(2,1,3),x(3,1,3),
//                     x(1,2,3),x(2,2,3),x(3,2,3)>
//
//and the dim vector is as follows
//
//      dim=<3,2,3>
//
//!

   xk=x;
   dims=[1 matrix(dim,1,maxi(size(dim)))];
   for k=1:maxi(size(dim)),
      xk=fft(xk,flag,dim(k),prod(dims(1:k)));
   end,
endfunction
