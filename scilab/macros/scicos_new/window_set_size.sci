function window_set_size()
  [lhs,rhs]=argn(0)

  r=xget('wpdim');
  rect=dig_bound(scs_m);
  if rect==[] then rect=[0,0,r(1),r(2)];end
  w=rect(3)-rect(1);
  h=rect(4)-rect(2);
  j=min(1.5,max(1,1600/(%zoom*w)),max(1,1200/(%zoom*h)))  ;
  ax=max(r(1)/(%zoom*w),j);
  ay=max(r(2)/(%zoom*h),j);

  xbasc()

  xset("wresize",0);
  width=%zoom*w*ax;height=%zoom*h*ay
  xset('wdim',width,height);
  bx=(1-1/ax)/2;
  by=(1-1/ay)/2;
  wrect=[bx,by,1/ax,1/ay]
  arect=[0.02,0.02,0.02,0.02]

  xsetech(wrect=wrect,frect=rect,arect=arect)

  %XSHIFT=max((width-r(1))/2,0)
  %YSHIFT=max((height-r(2))/2,0)
  if ~MSDOS then %YSHIFT=%YSHIFT+30,end
  xselect()
  xset('viewport',%XSHIFT,%YSHIFT)
endfunction
