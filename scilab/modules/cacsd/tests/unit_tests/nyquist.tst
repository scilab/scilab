// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->

function r=checknyquist(Args,leg)
  if size(Args)==1 then Args($+1)=1d-3; Args($+1)=1d+3;end
  [frq,rf,splitf]=repfreq(Args(:));repi=imag(rf);repf=real(rf);
  [mn,n]=size(rf);
  splitf($+1)=n+1;

  ksplit=1;sel=splitf(ksplit):splitf(ksplit+1)-1;
  R=[repf(:,sel)];  I=[repi(:,sel)];
  for ksplit=2:size(splitf,'*')-1
    sel=splitf(ksplit):splitf(ksplit+1)-1
    R=[R %nan(ones(mn,1)) repf(:,sel)];
    I=[I %nan(ones(mn,1)) repi(:,sel)];
  end
  //use symetry
  R=[R R(:,$:-1:1)];
  I=[I -I(:,$:-1:1)];

  l=find(~isnan(R(1,:)))
  fig=gcf();
  r=0
  a=fig.children;

  if a.log_flags<>'nnn' then r=1;return,end
  if or(a.axes_visible<>["on","on","on"]) then r=2;return,end

  n=size(Args(1),1)
  n1=size(a.children,'*')
  if (argn(2)==1&(n<>n1))|(argn(2)==2&(n+1<>n1)) then r=3;return,end
  for k=1:n;
    if a.children($+1-k).type<>'Compound' then r=4;return,end
    if size(a.children($+1-k).children,'*')<>2  then r=5;return,end
    if a.children($+1-k).children(1).type<>'Compound'  then r=6;return, end
    if a.children($+1-k).children(2).type<>'Polyline'  then r=7;return, end
    if %f then //temporarily removed for 5.0
      if a.children($+1-k).children(1).children(1).type<>'Segs' then r=8;return, end
      ns=size(a.children($+1-k).children(1).children(1).data,1);
      if size(a.children($+1-k).children(1).children,'*')-2<>(ns/2) then r=9;return, end
      if or(a.children($+1-k).children(1).children(2:$).type<>'Text') then r=10;return, end
    end
    if or(l<>find(~isnan(a.children($+1-k).children(2).data(:,1)))) then r=11;return, end
    if norm(a.children($+1-k).children(2).data(l,:)-[R(k,l)' I(k,l)'])> 1d-14 then r=12;return, end
    s= a.children($+1-k).children(1).children(1);
  end
  if argn(2)==2 then //test the legend
    if size(a.children,'*')<=n then r=13;return,end
    if a.children(1).type<>'Legend' then r=14;return,end
    if size(a.children(1).links,'*')<>n then r=15;return,end
    if or(a.children(1).text<>leg(:)) then r=16;return,end
  end
endfunction


s=poly(0,'s');
n=1+s;d=1+2*s;
h=syslin('c',n,d);
sl=tf2ss(h);
sld=dscr(sl,0.01);
hd=ss2tf(sld);

[w,rf]=repfreq(h,0.01,100);

//continuous time
clf();nyquist(h);
if checknyquist(list(h)) then pause,end
clf();nyquist(h,'h');
if checknyquist(list(h),'h') then pause,end

clf();nyquist(h,0.01,100);
if checknyquist(list(h,0.01,100)) then pause,end
clf();nyquist(h,0.01,100,'h');
if checknyquist(list(h,0.01,100),'h') then pause,end

clf();nyquist(h,0.01,100,0.01)
if checknyquist(list(h,0.01,100,0.01)) then pause,end
clf();nyquist(sl,0.01,100);
if checknyquist(list(sl,0.01,100)) then pause,end
clf();nyquist(sl,0.01,100,0.01)
if checknyquist(list(sl,0.01,100,0.01)) then pause,end

//continuous time SIMO
h=[h;h+1];sl=[sl;sl+1];
clf();nyquist(h);
if checknyquist(list(h)) then pause,end
clf();nyquist(h,['h';'h+1']);
if checknyquist(list(h),['h';'h+1']) then pause,end

clf();nyquist(h,0.01,100);
if checknyquist(list(h,0.01,100)) then pause,end
clf();nyquist(h,0.01,100,['h';'h+1']);
if checknyquist(list(h,0.01,100),['h';'h+1']) then pause,end

clf();nyquist(h,0.01,100,0.01)
if checknyquist(list(h,0.01,100,0.01)) then pause,end
clf();nyquist(sl,0.01,100);
if checknyquist(list(sl,0.01,100)) then pause,end
clf();nyquist(sl,0.01,100,0.01)
if checknyquist(list(sl,0.01,100,0.01)) then pause,end

//discrete case
clf();nyquist(hd);
if checknyquist(list(hd)) then pause,end
clf();nyquist(hd,'h');
if checknyquist(list(hd),'h') then pause,end

clf();nyquist(hd,0.01,100);
if checknyquist(list(hd,0.01,100)) then pause,end
clf();nyquist(hd,0.01,100,'h');
if checknyquist(list(hd,0.01,100),'h') then pause,end

clf();nyquist(hd,0.01,100,0.01)
if checknyquist(list(hd,0.01,100,0.01)) then pause,end
clf();nyquist(sld,0.01,100);
if checknyquist(list(sld,0.01,100)) then pause,end
clf();nyquist(sld,0.01,100,0.01)
if checknyquist(list(sld,0.01,100,0.01)) then pause,end

//discrete case SIMO
hd=[hd;hd+1];sld=[sld;sld+1];
clf();nyquist(hd);
if checknyquist(list(hd)) then pause,end
clf();nyquist(hd,['h';'h+1']);
if checknyquist(list(hd),['h';'h+1']) then pause,end

clf();nyquist(hd,0.01,100);
if checknyquist(list(hd,0.01,100)) then pause,end
clf();nyquist(hd,0.01,100,['h';'h+1']);
if checknyquist(list(hd,0.01,100),['h';'h+1']) then pause,end

clf();nyquist(hd,0.01,100,0.01)
if checknyquist(list(hd,0.01,100,0.01)) then pause,end
clf();nyquist(sld,0.01,100);
if checknyquist(list(sld,0.01,100)) then pause,end
clf();nyquist(sld,0.01,100,0.01)
if checknyquist(list(sld,0.01,100,0.01)) then pause,end

//nyquist given by precmputed freqency response

clf(); nyquist(w,rf)
clf(); nyquist(w,20*log(abs(rf))/log(10),(180/%pi)*atan(imag(rf),real(rf)))

// check legend color
clf();
F1=syslin('c',(2.5*0.8)/((0.25*%s+1)*(0.8*%s+1)));
F2=syslin('c',(0.88*2.5*0.8)/((0.25*%s+1)*(0.8*%s+1)));
F3=syslin('c',(100*2.5*0.8)/((0.25*%s+1)*(0.8*%s+1)));
nyquist ([F1;F2;F3],0.0 ,1000,["F1(Kr=1)";"F2(Kr=0,88)";"F3(Kr=10)"],%f)
a=gca();
assert_checkequal(a.children(1).links.foreground, [3, 2, 1]); // [green blue black]
