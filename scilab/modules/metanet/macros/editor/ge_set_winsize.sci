function ge_set_winsize(a)
//Copyright INRIA
//Author : Serge Steer 2002

  [lhs,rhs]=argn(0)

  rect=ge_dig_bound(GraphList);
  if rect<>[] then
    w=max(rect(3)-rect(1),1);
    h=max(rect(4)-rect(2),1);
    if rhs==0 then j=1.5;a=max(600/(j*w),400/(j*h),j); end
  else
    w=600;h=400;rect=[0,0,w,h];
    if rhs==0 then a=1.5; end
  end


  xbasc()

  xset("wresize",0);
  width=EGdata.Zoom*w*a;height=EGdata.Zoom*h*a
  xset('wdim',width,height);
  b=(1-1/a)/2;
  xsetech([b,b,1/a,1/a],rect)
  
  r=xget('wpdim');
  %XSHIFT=max((width-r(1))/2,0)
  %YSHIFT=max((height-r(2))/2,0)
  xset('viewport',%XSHIFT,%YSHIFT)
endfunction
