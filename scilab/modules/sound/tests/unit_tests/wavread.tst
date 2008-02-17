// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

  wavread('SCI/demos/signal/sound/chimes.wav','size')

  [y,Fs,bits]=wavread('SCI/demos/signal/sound/chimes.wav');Fs,bits
  subplot(2,1,1)
  plot2d(y(:,1))
  subplot(2,1,2)
  plot2d(y(:,2))
  
  y=wavread('SCI/demos/signal/sound/chimes.wav',[1 5]) //the first five samples
