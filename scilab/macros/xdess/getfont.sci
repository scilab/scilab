function [fontId,fontSize]=getfont(S,v1)
	
	//returns the clicked font id and font size. 
	fontId=[];fontSize=[];
	
	if exists('S','local')==0 then S="a";end
	if type(S)<>10 then error('Argument must be a character string'),end
	S=part(S(1),1)
	
	win=max(winsid()+1);
	xset('window',win);
	
	xset('window',win);
	set figure_style new;
	
	fnts=xlfont();
	nf=size(find(fnts<>''),'*')-1
	
	//Disabling standard menus
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
	
	x=2;
	red=addcolor([1 0 0]);
	W1=1 //first column width
	H1=1 // first row height
	H=1.5;W=1.5;
	Hdl=drawchart();
	
	done=%f;
	
	deff('menu_ok(k,gwin)','done=%t;k=[-fontId,fontSize];')
	deff('menu_cancel(k,gwin)','done=%t;k=[];')
	
	addmenu(win,'Ok',list(2,'menu_ok'));
	addmenu(win,'Cancel',list(2,'menu_cancel'));
	
	if exists('font','local') then
		fontId=max(0,min(nf,font(1)))
		fontSize=max(0,min(5,font(2)))
		selected=fontId+(nf+1)*fontSize+1;
		t=Hdl(selected); t.foreground=red;
		show_pixmap()
	else
		selected=0
	end
	
	while %t
		
		[c_i,cx,cy,cw,str]=xclick();
		
		if c_i==-2 then
			if strindex(str,"menu_ok")==1 then xdel(win);break,end
			if strindex(str,"menu_cancel")==1 then xdel(win);break,end
		
		elseif c_i==-100 then
			fontId=[];
			fontSize=[];
			break,
		
		elseif c_i>=0 & c_i<=12 then
			if selected<>0 then 
				t=Hdl(selected)
				t.foreground=-1;
			end
		
			fontId=max(0,min(nf,floor((cy)/H)));
			fontSize=max(0,min(5,floor((cx-W1)/W)));
			selected=fontId+(nf+1)*fontSize+1;
			t=Hdl(selected);t.foreground=red;
			show_pixmap()
			xinfo('You have chosen fontId = '+string(fontId)+' , fontSize =  '+string(fontSize))
		
		elseif c_i>=32 & c_i<1032 then
			S=ascii(c_i)
			a=gca();
			a.visible='off';
			Hdl.text=S;
			a.visible='on';
			show_pixmap()
			
		elseif c_i>=1032
			S=ascii(c_i-1000)
			a=gca();
			a.visible='off';
			Hdl.text=S;
			a.visible='on';
			show_pixmap()
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
	f.pixmap='on';
	a=gca()
	drawlater()
	a.data_bounds=[0 0;W1+6*W H1+(nf+1)*H]
	xtitle(["Click to select font Id  and font size";"or press a key to select a character"])
	
	// Drawing first column
	for k=0:nf
		xstringb(0,H*k,string(k),W1,H)
		xrect(0, H*(k+1), W1, H);
	end
	
	// Drawing first row
	for l=0:5
		xstringb(W1+l*W,(nf+1)*H ,string(l),W,H1)
		xrect(W1+l*W, H1+(nf+1)*H, W, H1);
	end
	
	// drawing top left corner
	xrect(0,H1+(nf+1)*H,W1,H1);
	xstringb(0, (nf+1)*H,"Id\Sz",H1,W1)
	
	Hdl=[]
	for k=-(0:nf)
		Hdll=[]
		for x=1:6
			xstringb(W1+(x-1)*W,-H*k,S,W,H)
			t=gce();t.font_size=x-1;t.font_style=-k
			Hdll=[Hdll t]
		end
		Hdl=[Hdl;Hdll]
	end
	
	glue(Hdl)
	drawnow()
	show_pixmap()
	
endfunction
