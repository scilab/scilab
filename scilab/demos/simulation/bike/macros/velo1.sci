function []=velo1()
// "full wheel" version
// Copyright INRIA
  ct=-cos(t);cp=cos(p);st=-sin(t);sp=sin(p);
  xe=[xmin;xmax;xmax;xmin;xmin]
  ye=[ymin;ymin;ymax;ymax;ymin]
  ze=[zmin;zmin;zmin;zmin;zmin];
  xer=ct*xe-st*ye;
  yer=cp*(st*xe+ct*ye)+sp*ze;

  [n1,n2]=size(xfrontar);
  deff('[]=velod(i)',['xnr=ct*xfrontar(:,i)-st*yfrontar(:,i);';
		      'ynr=cp*(st*xfrontar(:,i)+ct*yfrontar(:,i))+sp*zfrontar(:,i);';
		      'xnt=ct*xf(:,i)-st*yf(:,i);';
		      'ynt=cp*(st*xf(:,i)+ct*yf(:,i))+sp*zf(:,i);';
		      'xnf=ct*xrearar(:,i)-st*yrearar(:,i),';
		      'ynf=cp*(st*xrearar(:,i)+ct*yrearar(:,i))+sp*zrearar(:,i);';
		      'xpoly(xnt,ynt,''lines'')';
		      'xfpoly(xnr,ynr)';
		      'xfpoly(xnf,ynf)']);

  xset('thickness',1); 
  if driver()<>'Pos' then
    isoview(mini(xer),maxi(xer),mini(yer),maxi(yer));
    if ~isdef('velo_rti') then   velo_rti=0.03;end 
    realtimeinit(velo_rti);
    realtime(0)
    xset("alufunction",6)
    xpoly(xer,yer,'lines')
    for i=1:n2-1,
      velod(i);
      ww=i:i+1;
      plot2d((ct*xprear(1,ww)-st*xprear(2,ww))',...
	     (cp*(st*xprear(1,ww)+ct*xprear(2,ww))+sp*xprear(3,ww))',...
	     [1,-1],"000");
      realtime(i);
      velod(i);
    end
    velod(n2-1);
    xset("alufunction",3);
    xset('thickness',1);
  else
    pix=xget('pixmap')
    xset('pixmap',1)
    if ~isdef('velo_rti') then   velo_rti=0.03;end 
    realtimeinit(velo_rti);
    realtime(0)
    for i=1:4:n2-1,
      xset('wwpc')
      ww=1:i+1;
      xpoly(xer,yer,'lines')
      plot2d((ct*xprear(1,ww)-st*xprear(2,ww))',...
	     (cp*(st*xprear(1,ww)+ct*xprear(2,ww))+sp*xprear(3,ww))',...
	     [1,-1],"000");
      velod(i);
      xset('wshow')
      pause
      realtime(i);
    end
    xset('pixmap',pix)
  end
endfunction


