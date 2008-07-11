// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -  Author: Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function show_margins(h,typ)
  if argn(2)<2 then typ='bode',end
  if and(typ<>['nyquist','bode']) then
    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
		   "show_margins",2,'""nyquist"", ""bode""'))
  end
  drawlater();clf();
  if typ=="bode" then
    bode(h)
    f=gcf();
    axg=f.children(2);
    axp=f.children(1);
    fmin=min(axg.x_ticks.locations);
    fmax=max(axg.x_ticks.locations);
    gmin=min(axg.y_ticks.locations);
    gmax=max(axg.y_ticks.locations);
    pmin=min(axp.y_ticks.locations);
    pmax=max(axp.y_ticks.locations);

    [gm,fr]=g_margin(h)
    sca(axp);
    xpoly([fmin;fmax],[-180;-180])
    e=gce();e.foreground=color('red');e.line_style=4;
    if fr<>[] then
      xpoly([fr;fr],[pmin;pmax])
      e=gce();e.foreground=color('red');e.line_style=4;
      sca(axg);
      xpoly([fr;fr],[gmin;gmax])
      e=gce();e.foreground=color('red');e.line_style=4;
      xpoly([fr;fr],[-gm;0])
      e=gce();e.foreground=color('red');e.thickness=2;
    end

    [phm,fr]=p_margin(h)
    sca(axg);
    xpoly([fmin;fmax],[0;0])
    e=gce();e.foreground=color('blue');e.line_style=4;
    if fr<>[] then
      xpoly([fr;fr],[gmin;gmax])
      e=gce();e.foreground=color('blue');e.line_style=4;
      sca(axp);
      xpoly([fr;fr],[pmin;pmax])
      e=gce();e.foreground=color('blue');e.line_style=4;
      xpoly([fr;fr],[-180;phm-180])
      e=gce();e.foreground=color('blue');e.thickness=2;
    end
  else
    nyquist(h)
    ax=gca();
    [gm,fr]=g_margin(h)
    xpoly([min(ax.x_ticks.locations);0],[0;0]);
    e=gce();e.foreground=color('blue');e.line_style=4;
    if fr<>[] then
      if h.dt=='c' then
	f=horner(h,2*%i*%pi*fr)
      else
	dt=h.dt;if dt=='d' then dt=1,end
	f=horner(h,exp(2*%i*%pi*fr*dt))
      end
      xpoly([real(f);0],[0;0]);
      e=gce();e.foreground=color('blue');e.thickness=2;
    end
    [phm,fr]=p_margin(h)
    //unit circle
    t=linspace(0,2*%pi,100);
    xpoly(sin(t),cos(t))
    e=gce();e.foreground=color('red');e.line_style=4;
    if fr<>[] then
      t=phm*%pi/180+%pi;
      xpoly([cos(t);0],[sin(t);0])
      e=gce();e.foreground=color('red');e.thickness=2;
    end
  end
  
  
  drawnow()
endfunction
