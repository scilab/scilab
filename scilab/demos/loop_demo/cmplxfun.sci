function demo_cmplxfun()
  demo_help demo_cmplxfun
  getf("SCI/demos/cmplxfunc/MacCmplx.sci")
  set figure_style old;xset("wpos",600,16);xset("wdim",600*0.9,400*0.9)
  R = 4;  e = 0.001; theta = 30; alpha = 60;
  PlotCmplxFunc(R,e,"Disk","Ox",[40 20],"log",theta,alpha,[e,R])
  realtimeinit(0.1);for k=1:60,realtime(k),end
  R = 2 ; theta = -110; alpha = 75;
  PlotCmplxFunc(R,0.001,"Square","Oy",41,"atan",theta,alpha,[-R,R])
  realtimeinit(0.1);for k=1:60,realtime(k),end
endfunction
