//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

////////////////////////
//WINDOWED FIR FILTERS//
////////////////////////

//Low pass filter (length 33, Kaiser window, cut-off .2)
   [wft,wfm,fr]=wfir('lp',33,[.2 0],'kr',[5.6 0]);

   my_handle = scf(100001);
   clf(my_handle,"reset");
   plot2d(fr',log(wfm)')
   xtitle(' ','frequency','magnitude');
   xtitle(['Windowed FIR Low pass Filter';...
          'Kaiser window,cut-off:0.2';...
          'length 33'])
   halt();

//Stop band filter (length 127, Hamming window, cut-offs .2 and .3)
   [wft,wfm,fr]=wfir('sb',127,[.2 .3],'hm',[0 0]);
   clf(my_handle,"reset");
   plot2d(fr',log(wfm)')
   xtitle(' ','frequency','magnitude')
   xtitle(['Windowed FIR Low pass Filter';...
          'Hamming window,cut-off:0.2,0.3';...
          'length 127'])
   halt();

//Band pass filter (length 55, Chebyshev window, cut-offs .15 and .35)
   [wft,wfm,fr]=wfir('bp',55,[.15 .35],'ch',[.001 -1]);
   clf(my_handle,"reset");
   plot2d(fr',log(wfm)')
   xtitle(' ','frequency','magnitude')
   xtitle(['Windowed FIR Band pass Filter';...
          'Chebyshev window,cut-off:0.15,0.35';...
          'length 55'])
   halt();

   delete(my_handle);
