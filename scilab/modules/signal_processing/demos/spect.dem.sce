// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released into the public domain

////////////////////////
//SPECTRAL ESTIMATION///
////////////////////////
   mode(1);
   lines(0);

//generate white data
   rand('normal');
   rand('seed',0);
   x=rand(1:1024-33+1);

//make low-pass filter with eqfir
   nf=33;
   bedge=[0 .1;.125 .5];
   des=[1 0];
   wate=[1 1];
   h=eqfir(nf,bedge,des,wate);

//filter white data to obtain colored data
   h1=[h 0*ones(1:max(size(x))-1)];
   x1=[x 0*ones(1:max(size(h))-1)];
   hf=fft(h1,-1);
   xf=fft(x1,-1);
   yf=hf.*xf;
   y=real(fft(yf,1));
 
//plot magnitude of filter
   h2=[h 0*ones(1:167)];
   hf2=fft(h2,-1);
   hf2=real(hf2.*conj(hf2));
   hsize=max(size(hf2));
   fr=(1:hsize)/hsize;

   my_handle = scf(100001);
   clf(my_handle,"reset");

   plot2d(fr',log(hf2)');
   xtitle('Data spectrum','frequency','magnitude');
   halt();

//pspect example
   [sm1]=pspect(100,200,'tr',y);
   smsize=max(size(sm1));
   fr=(1:smsize)/smsize;
   clf(my_handle,"reset");
   plot2d(fr',log(sm1)')
   xtitle('Spectral estimation','frequency','spectral power');
   halt();

//cspect example
   [sm2]=cspect(100,200,'tr',y);
   smsize=max(size(sm2));
   fr=(1:smsize)/smsize;
   clf(my_handle,"reset");
   plot2d(fr',log(sm2)');
   xtitle(['Spectral estimation ; periodogram method'],' ' ,' ' )
   halt();

   delete(my_handle);
