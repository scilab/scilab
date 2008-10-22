//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

////////////////////////
//MINIMAX FIR FILTERS///
////////////////////////
   mode(1);

//Low pass filter (length 33, cut-off in [.23,.27], weight 1/.1)

   hn=eqfir(33,[0 .23;.27 .5],[1 0],[1 .1]);
   [hm,fr]=frmag(hn,256);

   my_handle = scf(100001);
   clf(my_handle,"reset");

   plot2d(fr',hm'),
   title_c=["Minimax FIR filter";
            "low pass  cut-off in [.23,.27]";
            "length 33"];
   ax=gca();a.margins(3)=0.2;
   title(title_c);
   halt();
 
//Triangular filter created using remezb
   nc=21;
   ngrid=nc*16;
   fg=.5*(0:(ngrid-1))/(ngrid-1);
   ds(1:ngrid/2)=(0:-1+ngrid/2)*2/(ngrid-2);
   ds(ngrid/2+1:ngrid)=ds(ngrid/2:-1:1);
   wt=ones(fg);
   [an]=remezb(nc,fg,ds,wt);
   hn(1:nc-1)=an(nc:-1:2)/2;
   hn(nc)=an(1);
   hn(nc+1:2*nc-1)=an(2:nc)/2;
   [hm,fr]=frmag(hn,maxi(size(ds)));
   clf(my_handle,"reset");
   plot2d(fr',hm'),
   xtitle('Remez Triangular filter','frequency','magnitude');
   halt();
   clf(my_handle,"reset");
   plot2d(fr',(hm-ds)'),
   xtitle(' ','frequency','magnitude error');
   halt();

   delete(my_handle);
