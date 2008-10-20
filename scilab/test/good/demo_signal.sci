function demo_signal()
  demo_help demo_signal
  SetPosition();
  
  set figure_style old; //xselect();xbasc();
    //xset("wpos",500,16);xset("wdim",500*0.9,350*0.9);
  //xset('wpos',620,16);xset('wdim',600,540);
  subplot(221)
  hz=iir(3,'bp','ellip',[0.15 0.25],[0.08 0.035]);
  [hzm,fr]=frmag(hz,256);
  plot2d(fr',hzm')
  xset("font size",3) ;
  xtitle('Discrete  elliptic IIR filter',' ',' ');

  subplot(222)
  hn=eqfir(33,[0 .23;.27 .5],[1 0],[1 .1]);
   [hm,fr]=frmag(hn,256);
   plot2d(fr',hm'),
   title_c=['Minimax FIR filter';
 	  'low pass  cut-off in [.23,.27]';
          'length 33'];
   xtitle(title_c);
   
  subplot(223)
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
   plot2d(fr',hm'),
   xtitle('Remez Triangular filter','frequency','magnitude');
  subplot(224)

   [wft,wfm,fr]=wfir('lp',33,[.2 0],'kr',[5.6 0]);
   plot2d(fr',log(wfm)')
   xtitle(' ','frequency','magnitude');
   xtitle(['Windowed FIR Low pass Filter';...
          'Kaiser window,cut-off:0.2';...
          'length 33'])
realtimeinit(0.1),for k=1:60,realtime(k),end
xdel()
endfunction
