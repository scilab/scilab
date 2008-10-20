function [k1,k2]=getmark()
	
	//returns the clicked markid (with a minus sign) 
	//and mark size. 
	[lhs,rhs]=argn(0)
	k1=[];k2=[];
	win=max(winsid()+1);
	xset('window',win);
	
	if ~MSDOS then
		delmenu(win,'3D Rot.')
		delmenu(win,'UnZoom')
		delmenu(win,'Zoom')
		delmenu(win,"Edit")
		delmenu(win,'File')
		delmenu(win,'Insert')
	else
		hidetoolbar(win)
		// French
		delmenu(win,'&Fichier')
		delmenu(win,'&Editer')
		delmenu(win,'&Outils')
		delmenu(win,'&Inserer')
		// English
		delmenu(win,'&File')
		delmenu(win,'&Edit')
		delmenu(win,'&Tools')
		delmenu(win,'&Insert')
	end
	
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
	
	set('figure_style','new')
	f=gcf();
	f.figure_size = [674,841]
	f.axes_size = [610,780]
	cur_drawing_mode = f.immediate_drawing;
	f.immediate_drawing ='off';
	f.auto_resize='off'
	
	a=gca();
	
	a.axes_visible='off';
	a.data_bounds=[0 0;10 30];
	a.font_size=2;
	a.mark_size_unit="tabulated"
	a.margins=[0.04 0.06 0.1 0.05];
	
	xtitle(" Select mark style k and mark size l")
	for k=0:14
		xstringb(0,2*k-1,"k = "+msprintf("%2d",k),1.5,2)
		xrect(0, 2*k+1, 1.5, 2);
	end
	
	for l=1:6
		xstringb(l*1.5, 29,"l = "+msprintf("%2d",l-1),1.5,2)
		xrect(l*1.5, 31, 1.5, 2);
	end
	
	H=[]
	for x=1:6
		for k=(0:19)
			xpoly(1+x*1.5,2*k,'marks');
			p=gce();p.mark_size=x-1;p.mark_style=k;
			H=[H;p];
		end
	end
	
	f.immediate_drawing = cur_drawing_mode;
	f.pixmap='on';
	
	seteventhandler('evh')
	
	addmenu(win,'Ok',list(2,'menu_ok'));
	addmenu(win,'Cancel',list(2,'menu_cancel'));
	
	global pos done;done=-1;
	ksel=9;xsel=1;
	while %t
		select done
			case 0 then //click somehere
				cx=pos(1);cy=pos(2)
				k1=round(cy/2);k1=min(k1,14);k1=max(0,k1);
				k2=round(cx/1.5);k2=k2-2;k2=min(k2,5);k2=max(0,k2);
				H(ksel+1+xsel*10).foreground=-1;
				H(k1+1+k2*10).foreground=5;
				ksel=k1;xsel=k2;
				show_pixmap()
				done=-1;
				xinfo('You have chosen mark_style = '+string(k1)+' , mark_size = '+string(k2))
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
