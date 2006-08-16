function [sm,cwp]=pspect(sec_step,sec_leng,wtype,x,y,wpar)
//<sm,cwp>=pspect(sec_step,sec_leng,wtype,x,y,wpar)
//Cross-spectral estimate between x and y if both are given
//and auto-spectral estimate of x otherwise.
//Spectral estimate obtained using the modified periodogram method
// x        :data if vector, amount of input data if scalar
// y        :data if vector, amount of input data if scalar
// sec_step :offset of each data window
// sec_leng :length of each data window
// wtype    :window type (re,tr,hm,hn,kr,ch)
// wpar     :optional window parameters
//          :         for wtype='kr', wpar>0
//          :         for wtype='ch', 0<wpar(1)<.5, wpar(2)>0
// sm       :power spectral estimate in the interval [0,1]
// cwp      :unspecified Chebyshev window parameter
//!
//author: C. Bunks  date: 14 Sept 1988
// Copyright INRIA

   [lhs,rhs]=argn(0);
   cross=0;
   if sec_step<sec_leng then,
 
//get number of sections to be used
 
   xsize=maxi(size(x));
   if xsize==1 then,
      xsize=x;
   end,
   nsecs=int((xsize-sec_leng)/sec_step);
 
//construct window
 
   if rhs==4 then,
      w=window(wtype,sec_leng);
   else if rhs==5 then,
      if wtype=='kr' then,
         wpar=y;
         w=window(wtype,sec_leng,wpar);
      else if wtype=='ch' then,
         wpar=y;
         [w,cwp]=window(wtype,sec_leng,wpar);
      else,
         cross=1;
         w=window(wtype,sec_leng);
      end,
      end,
   else,
      [w,cwp]=window(wtype,sec_leng,wpar);
      cross=1;
   end,
   end,
   wpower=w*w';
 
//average periodograms
 
   sm=0*w;
   if maxi(size(x))==1 then,
      ovrlp=sec_leng-sec_step;
      xd=[0*ones(1,sec_step) getx(ovrlp,1)];
      if cross==1 then,
         yd=[0*ones(1,sec_step) gety(ovrlp,1)];
      end,
      for k=1:nsecs,
         xd(1:ovrlp)=xd(sec_step+1:sec_leng);
         xd(ovrlp+1:sec_leng)=...
                 getx(sec_step,sec_leng+(k-1)*sec_step+1);
         xw=w.*(xd-(sum(xd)/sec_leng)*ones(1:sec_leng));
         fx=fft(xw,-1);
         if cross==1 then,
            yd(1:ovrlp)=yd(sec_step+1:sec_leng);
            yd(ovrlp+1:sec_leng)=...
                 gety(sec_step,sec_leng+(k-1)*sec_step+1);
            yw=w.*(yd-(sum(yd)/sec_leng)*ones(1:sec_leng));
            fy=fft(yw,-1);
            sm=sm+fx.*conj(fy);
         else,
            sm=sm+real(fx.*conj(fx));
         end,
      end,
   else,
      ind=(1:sec_leng);
      for k=1:nsecs,
         indd=ind+(k-1)*sec_step*ones(1:sec_leng);
         xd=x(indd);
         xe=w.*(xd-(sum(xd)/sec_leng)*ones(1:sec_leng));
         fx=fft(xe,-1);
         if cross==1 then,
            yd=y(indd);
            ye=w.*(yd-(sum(yd)/sec_leng)*ones(1:sec_leng));
            fy=fft(ye,-1);
            sm=sm+fx.*conj(fy);
         else,
            sm=sm+real(fx.*conj(fx));
         end,
      end,
   end,
 
   sm=sm/(nsecs*wpower);
 
//input error
 
   else,
      error('section step is bigger then section length');
   end
endfunction
