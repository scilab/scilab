// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released into the public domain

lines(0);

////////////////////////
//DISCRETE IIR FILTERS//
////////////////////////

//Low pass filter (length 5, type cheb1, cut-off .2)
   hz=iir(5,'lp','cheb1',[.2 0],[.05 .05]);
   [hzm,fr]=frmag(hz,256);

   my_handle = scf(100001);
   clf(my_handle,"reset");

   plot2d(fr',hzm');
   xtitle('Discrete IIR filter : low pass  fr <0.2',' ',' ');
   halt();

//Band pass filter (length 3, type ellip, cut-offs .15 and .25)
   hz=iir(3,'bp','ellip',[.15 .25],[.08 .03]);
   [hzm,fr]=frmag(hz,256);
   clf(my_handle,"reset");
   plot2d(fr',hzm')
   xtitle('Discrete IIR filter band pass  0.15<fr<0.25 ',' ',' ');
