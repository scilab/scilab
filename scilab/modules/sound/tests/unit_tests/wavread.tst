// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

  a = wavread('SCI/modules/sound/demos/chimes.wav','size');
	if size(a) <> [ 1 2] then pause,end
  if a <> [ 2 13921] then pause,end
  
  [y,Fs,bits]=wavread('SCI/modules/sound/demos/chimes.wav');
  if Fs <> 22050 then pause,end
  if bits <> 16 then pause,end
  if size(y) <> [ 2 13921] then pause,end
  //subplot(2,1,1)
  //plot2d(y(:,1))
  //subplot(2,1,2)
  //plot2d(y(:,2))
  //xdel(winsid());
  
  y=wavread('SCI/modules/sound/demos/chimes.wav',[1 5]) //the first five samples
  if size(y) <> [2 5] then pause,end
