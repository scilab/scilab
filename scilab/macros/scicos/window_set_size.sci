function window_set_size()
  [lhs,rhs]=argn(0)

   r=xget('wpdim');
  rect=dig_bound(scs_m);
  if rect<>[] then
    w=rect(3)-rect(1);
    h=rect(4)-rect(2);
   j=1.5;
   ax=max(r(1)/(j*w),j);
   ay=max(r(2)/(j*h),j);
  else
    w=r(1);h=r(2);rect=[0,0,w,h];
    ax=1.5;ay=ax;
  end


  xbasc()

  xset("wresize",0);
  width=%zoom*w*ax;height=%zoom*h*ay
  xset('wdim',width,height);
  bx=(1-1/ax)/2;
  by=(1-1/ay)/2;
  xsetech([bx,by,1/ax,1/ay],rect)
  
  r=xget('wpdim');
  %XSHIFT=max((width-r(1))/2,0)
  %YSHIFT=max((height-r(2))/2,0)
  xset('viewport',%XSHIFT,%YSHIFT)
endfunction
