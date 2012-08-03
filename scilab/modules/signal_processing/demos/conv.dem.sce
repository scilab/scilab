// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge STEER
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function demo_conv()

//create a signal
  dt=0.02;
  t=0:dt:1;
  f=1;// frequency in Hertz
  y=sin(2*%pi*f*t)+0.1*rand(t);
  scf(1000);clf;
  demo_viewCode('conv.dem.sce');
  drawlater()
  
  subplot(211)
  plot(t,y) //given signal
  //low pass 8 points wfir filter
  smooth=[0.1197912,0.1242239,0.1272323,0.1287527,0.1287527,0.1272323,0.1242239,0.1197912];
  ys=conv(y,smooth,'same');
  plot(t,ys,'r')// filtered signal
  legend([_("given noisy signal"),_("filtered signal")])
  title(_("Signal"))
  
  subplot(212)
  //2 points derivative using convolution
  plot(t(1:$-1),conv(ys,[1,-1],'valid')/dt,'r')
  //3 points derivative  using convolution
  plot(t(1:$-2),conv(ys,[1, 0,-1]/2,'valid')/dt,'g')
  legend([_("2 points convolution"),_("3 points convolution")])
  title(_("Signal derivative estimate"))
  drawnow()
endfunction
demo_conv()
clear demo_conv
