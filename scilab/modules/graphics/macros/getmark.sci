// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [k1,k2]=getmark()
  
//returns the clicked markid (with a minus sign) 
//and mark size. 
  [lhs,rhs]=argn(0)
  k1=[];k2=[];
  win=max(winsid()+1);
  f=scf(win)

  nbMarks = 14;
  nbSizes = 6;
  
  delmenu(win,_("&File")); 
  delmenu(win,_("&Edit"));
  delmenu(win,_("&Tools"));
  delmenu(win,_("&?")); 

  toolbar(win, "off");
  
  //Event handler and menu definition
  
  deff('evh(gwin,x,y,ibut)',..
       ['global pos done'
	'if or(ibut==(0:5)) then '
	'   [x,y]=xchange(x,y,''i2f'')'
	'   pos=[x,y],done=0,'
	'end'
	'if ibut==-1000 then ,done=3,end'
       ])
  
  deff('menu_ok(k,gwin)','global done;done=1')
  deff('menu_cancel(k,gwin)','global done;done=2')
  drawlater()	
  f.axes_size = [610,610]
  f.auto_resize='off'
  
  a=gca();
  
  a.axes_visible='off';
  a.data_bounds=[0 0;10 30];
  a.font_size=2;
  a.mark_size_unit="tabulated"
  a.margins=[0.04 0.06 0.05 0.1];
  
  a.title.text = " Select mark style k and mark size l";
  a.title.font_size = 3;
  for k=0:nbMarks
    xstringb(0,2*k - 2,"k = "+msprintf("%2d",k),1.5,2)
    xrect(0, 2*k, 1.5, 2);
  end
  
  for l=1:nbSizes
    xstringb(l*1.5, 28,"l = "+msprintf("%2d",l-1),1.5,2)
    xrect(l*1.5, 30, 1.5, 2);
  end
  
  H=[]
  for x=1:nbSizes
    Hc=[]
    for k=(0:nbMarks)
      xpoly(0.75+x*1.5,2*k - 1,'marks');
      p=gce();p.mark_size=x-1;p.mark_style=k;
      Hc=[Hc;p];
    end
    H=[H Hc]
  end
  drawnow()
  seteventhandler('evh')
  
  addmenu(win,'Ok',list(2,'menu_ok'));
  addmenu(win,'Cancel',list(2,'menu_cancel'));
  
  global pos done;done=-1;
  ksel=9;xsel=1;
  //H(k1+1,(k2+1)).mark_foreground=5;
  while %t
    select done
    case 0 then //click somehere
      seteventhandler('');drawlater()
      cx=pos(1);cy=pos(2)
      k1=round((cy + 1)/2);k1=min(k1,nbMarks);k1=max(0,k1);
      k2=round((cx + 0.75)/1.5);k2=k2-2;k2=min(k2,nbSizes - 1);k2=max(0,k2);
      H(ksel+1,xsel+1).mark_foreground=-1;
      H(k1+1,k2+1).mark_foreground=5;
      ksel=k1;xsel=k2;
      done=-1;
      xinfo('You have chosen mark_style = '+string(k1)+', mark_size = '+string(k2))
      drawnow();seteventhandler('evh')
    case 1 then  // ok button clicked
      k1=k1;k=[k1,k2];break,
    case 2 then   // cancel button clicked
      k=[];break,
    case 3 then //the window has been blosed
      k=[],if argn(1)==1 then k1=k,end
      clearglobal pos done
      return
    end
    xpause(100)
  end
  if argn(1)==1 then k1=k,end
  seteventhandler('')
  xdel(win)
  clearglobal pos done
endfunction
