// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [fontId,fontSize]=getfont(S,v1)
  
//returns the clicked font id and font size. 
  fontId=[];fontSize=[];
  
  if exists('S','local')==0 then S="a";end
  if type(S)<>10 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"), "getfont", 1));
  end
  S=part(S(1),1)
  
  win=max(winsid()+1);
  f=scf(win);
  
  //fnts=xlfont();nf=size(find(fnts<>''),'*')-1	
  fontnames=['Courrier';'Symbol';'Times';'Times Italic';
	     'Times Bold';'Times Bold Italic';
	     'Helvetica';'Helvetica Italic';
	     'Helvetica Bold';'Helvetica Bold Italic'];

  nf=size(fontnames,'*')

  font_sz=[0.5 1:6];
  nz=size(font_sz,'*'); //number of font sizes
//Disabling standard menus

  addmenu(win,'Ok',list(2,'menu_ok'));
  addmenu(win,'Cancel',list(2,'menu_cancel'));
  delmenu(win,"&?")
  delmenu(win,_("&Edit"))
  delmenu(win,_("&Tools"))  
  delmenu(win,_("&File"))
  toolbar(win,'off')
  x=2;
  red=addcolor([1 0 0]);
    W1=6 //first column width
  H1=2 // first row height
  H=3.5;W=2.5;

  Hdl=drawchart();
  
  done=%f;
  
  deff('menu_ok(k,gwin)','done=%t;k=[-fontId,fontSize];')
  deff('menu_cancel(k,gwin)','done=%t;k=[];')
    
  if exists('font','local') then
    fontId=max(0,min(nf,font(1)))
    fontSize=max(0,min(5,font(2)))
    selected=fontId+nf*fontSize+1;
    t=Hdl(selected); t.foreground=red;
  else
    selected=0
  end
  
  while %t
    
    [c_i,cx,cy,cw,str]=xclick();
    
    if c_i==-2 then
      if strindex(str,"menu_ok")==1 then xdel(win);break,end
      if strindex(str,"menu_cancel")==1 then xdel(win);break,end
      
    elseif c_i==-1000 then
      fontId=[];
      fontSize=[];
      break,
      
    elseif c_i>=0 & c_i<=12 then
      if selected<>0 then 
	t=Hdl(selected)
	t.foreground=-1;
      end
      
      fontId=max(0,min(nf,floor((cy)/H)));
      fontSize=max(0,min(nz,floor((cx-W1)/W)));
      selected=fontId+nf*fontSize+1;
      t=Hdl(selected);t.foreground=red;

      xinfo('You have chosen fontId = '+string(fontId)+' , fontSize =  '+string(fontSize))
      
    elseif c_i>=32 & c_i<1032 then
      S=ascii(c_i)
      drawlater();Hdl.text=S;drawnow()
    elseif c_i>=1032
      S=ascii(c_i-1000)
      drawlater();Hdl.text=S;;drawnow()
    end
  end
  
  if strindex(str,"menu_cancel")==1 then
    fontId=[];
    fontSize=[];
  end
  
  if argn(1)<2 then
    fontId=[fontId,fontSize];
  end

endfunction

function Hdl=drawchart()
  f=gcf();
  a=gca();
  drawlater()
  a.margins([1:2 4])=0.1
  a.data_bounds=[0 0;W1+nz*W H1+(nf)*H]
  a.title.text=["Click to select font Id  and font size";
		"or press a key to select a character"];
  
  // Drawing first column
  for k=0:nf-1
    xstringb(0,H*k,fontnames(k+1)+' ('+string(k)+')',W1,H)
    xrect(0, H*(k+1), W1, H);
  end
  
  // Drawing first row
  for l=0:nz-1
    xstringb(W1+l*W,(nf)*H ,string(font_sz(l+1)),W,H1)
    xrect(W1+l*W, H1+(nf)*H, W, H1);
  end
  
  // drawing top left corner
  xrect(0,H1+(nf)*H,W1,H1);
  xstringb(0, (nf)*H,"Id (index)\Size",W1,H1)
  
  Hdl=[]
  for k=-(0:nf-1)
    Hdll=[]
    for x=1:nz
      xstringb(W1+(x-1)*W,-H*k,S,W,H)
      t=gce();t.font_size=font_sz(x);t.font_style=-k
      Hdll=[Hdll t]
    end
    Hdl=[Hdl;Hdll]
  end

  glue(Hdl)
  drawnow()
  
endfunction
