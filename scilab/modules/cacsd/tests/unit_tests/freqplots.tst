// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->

plots=['bode','black','nyquist','gainplot'];
s=poly(0,'s');
h=syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01));
h1=h*syslin('c',(s^2+2*0.1*15.1*s+228.01)/(s^2+2*0.9*15*s+225));
hd=dscr(h,0.01);
hd1=dscr(h1,0.01);
[frq,repf]=repfreq([h;h1],0.01,100);
[phi,db]=phasemag(repf);

Args=list(..
	  list(h),..
	  list(h,0.01,100),..
	  list(h,0.01,100,'h'),..
	  list([h;h1]),..
	  list([h;h1],0.01,100),..
	  list([h;h1],0.01,100,['h';'h1']),..
	  list(hd),..
	  list(hd,0.01,100),..
	  list(hd,0.01,100,'hd'),..
	  list([hd;hd1]),..
	  list([hd;hd1],0.01,100),..
	  list([hd;hd1],0.01,100,['hd';'hd1']),..
	  list(frq,repf(1,:)),..
	  list(frq,repf(1,:),'h'),..
	  list(frq,repf),..
	  list(frq,repf,['h';'h1']),..
	  list([frq;frq],repf,['h';'h1']),..
	  list(frq,db(1,:),phi(1,:)),..
	  list(frq,db(1,:),phi(1,:),'h'),..
	  list(frq,db,phi),..
	  list(frq,db,phi,['h';'h1']),..
	  list([frq;frq],db,phi,['h';'h1']));

warning('off');	  
for p=plots
  mprintf("-------- %s ---------------\n",p)
  for args=Args
    clf()   
    if execstr(p+'(args(:))','errcatch')<>0 then pause,end
  end
end
warning('on');	  