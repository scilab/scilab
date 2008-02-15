function [xh]=hilb(n,wtype,par)
//<xh>=hilb(n[,wtype][,par])
//returns the first n points of the
//Hilbert transform centered around the origin.
//That is, xh=(2/(n*pi))*(sin(n*pi/2))**2.
//Window type and window parameters are optional.
// n     :Number of points in filter (must be odd)
// wtype :window type ('re','tr','hn','hm','kr','ch')
//       :     default wtype='re'
// par   :window parameter for wtype='kr' or 'ch'
//       :     default par=<0 0>
//       :see the macro window for more help
// xh    :Hilbert transform
//
//!
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - C. Bunks
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

   [lhs,rhs]=argn(0);
   if rhs==1 then,
      wtype='re';
      par=[0 0];
   else if rhs==2 then,
      par=[0 0];
   end,
   end,
   if int(n/2)==n/2 then,
      error('Error---Filter length must be odd')
   end,
   no2=(n-1)/2;
   th=0*ones(1,no2);
   th(1:2:no2)=ones(1:2:no2)./(1:2:no2);
   xh=[-th(no2:-1:1),0,th];
   xh=2*xh/%pi;
 
   [win_l,cwp]=window(wtype,n,par);
   xh=xh.*win_l;
endfunction
