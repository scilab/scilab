 // Non reg test on string format in 2D Axes

   a=gca();
   a.tight_limits='on';
   x=[0:0.1:5]';
   y=cos(x);
   plot2d(x,y); // 3 numbers after comma // CHECK HERE
   plot2d([0,5],[0,0]); // the zero is well placed  // CHECK HERE

   // pretty display test:
   a.tight_limits="off"  // CHECK HERE