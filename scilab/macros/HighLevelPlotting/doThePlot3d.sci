function doThePlot3d(x,y,z,az,el,lab,ms,ax)

if length(az)>1
   driver('X11')
   xset('pixmap',1);
   
   for i=1:length(az)-1;
      xset('wwpc');
      plot3d(x,y,z,az(i),el(i),lab,ms,ax);
      xset('wshow');
   end
   xset('pixmap',0)
   driver('Rec');
   xset('pixmap',1)
   xset('wwpc')
   plot3d(x,y,z,az($),el($),lab,ms,ax);
   xset('wshow')
   xset('pixmap',0)
else
   plot3d(x,y,z,az($),el($),lab,ms,ax);
end
