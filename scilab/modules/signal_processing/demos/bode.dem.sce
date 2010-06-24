// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released into the public domain


                  //////////////////
                  //BODE PLOTS//////
                  //////////////////
mode(1)
//First plot using state space representation
//
//   xd=-2*%pi*x+u
//    y=18*%pi*x+u
//
// h(s)=18*%pi/(s+2*%pi)
//
   a=-2*%pi;
   b=1;
   c=18*%pi;
   d=1;
   sl=syslin('c',a,b,c,d);ssprint(sl)

   my_handle = scf(100001);
   clf(my_handle,"reset");
   bode(sl,.1,100),
   halt()

//Second plot using poly
   s=poly(0,'s');
   h1=1/real((s+2*%pi*(15+100*%i))*(s+2*%pi*(15-100*%i)));
   h1=syslin('c',h1)
   clf(my_handle,"reset");
   bode(h1,10,1000,.01),
   halt();

//Third plot which combines the first two plots
   clf(my_handle,"reset");
   bode([h1;sl],.1,1000,.01);
   halt();


// series connection
   clf(my_handle,"reset");
   bode(h1*sl,.1,1000,.01);
   halt();

   delete(my_handle);
