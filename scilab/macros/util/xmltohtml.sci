function xmltohtml(dirs,titles,xsl,step)

	//------------------------------------------------------------------------------------------
	// Copyright Enpc (Jean-Philippe Chancelier)
	//------------------------------------------------------------------------------------------
	// dirs is a set of directories 
	// for which html manuals are to be generated 
	// + and index and toc file 
	// titles are associated title strings (optional or [])
	// if dirs is not specified or [] then 
	// standard scilab man are assumed and titles 
	// are searched in %helps 
	// updated by HUYNH Olivier on the 9/03/2004
	//------------------------------------------------------------------------------------------

	curdir=pwd()
	
	help=utillib.help;
	
	//to load the subfunction change_old_man
	//this is required to produce the whatis.htm files
	//associated with old style manuals
	
	change_old_man()
	
	global LANGUAGE %helps
	select LANGUAGE 
		case 'eng' then H= 'Help chapter' 
		case 'fr'  then H= 'Chapitre de help'
	end
	
	//------------------------------------------------------------------------------------------
	// get man_dirs
	//------------------------------------------------------------------------------------------
	
	std= grep(%helps(:,1),SCI)
	
	//------------------------------------------------------------------------------------------
	//patch because scicos is not written in xml
	//------------------------------------------------------------------------------------------
	
	scs=grep(%helps(std,1),'scicos')
	if size(scs,'*')==1 then std(scs)=[],end
	
	man_dirs = basename(%helps(std,1));
	man_titles= %helps(std,2);
	if man_dirs == [] then 
		error('I cannot find man dirs');
		return 
	end
  
	[lhs,rhs]=argn(0) 
	
	if rhs > 4 then error(39),return; end
	if rhs <= 3 then step='all';end 
	if rhs <= 2 then xsl= 'html-rev.xsl'; end 
	if rhs <= 1 then titles= H + emptystr(dirs);end 
	if rhs <= 0 then dirs = man_dirs ;end 
	if titles==[] then titles= H+ emptystr(dirs);end 
	if dirs == [] then 
		dirs = 'SCI/man/'+LANGUAGE+'/'+man_dirs ;
		titles= man_titles;
	end
 
	//------------------------------------------------------------------------------------------
	// first build all the whatis 
	//------------------------------------------------------------------------------------------
	
	if step=='all' | step == 'whatis' then 
		for k=1:size(dirs,'*');
			mprintf('Creating whatis.htm in %s\n',dirs(k));
			dirs(k)=pathconvert(dirs(k),%f,%t)
			chdir(dirs(k));
			if titles(k) == H then 
				titles(k) = titles(k)+' ('+dirs(k)+')';
			end
			gener_whatis(titles(k))
			chdir('../')
		end
	end
  
	//------------------------------------------------------------------------------------------
	// the perform the  html generation
	//------------------------------------------------------------------------------------------
	
	if step=='all' | step == 'html' then 
		dirs=stripblanks(dirs)
		for k=1:size(dirs,'*');
			mprintf('Processing chapter %s\n',dirs(k));
			chdir(dirs(k));
			global %helps;
			nk=length(dirs(k))
			if or(part(dirs(k),nk)==['/','\']) then nk=nk-1,end
			if and(%helps<>part(dirs(k),1:nk)) then
				saved_helps=%helps;
				%helps=[%helps;'./',"Temp"];
				%helps=saved_helps;
			end
	
			xml = listfiles('*.xml');
			if xml <> [] then 
				for k1=1:size(xml,'*')  // loop on .xml files 
				xmlfiletohtml(xml(k1),xsl)
				end 
			end
			chdir('../')
		end
	end
	
	chdir(curdir)
	
	//------------------------------------------------------------------------------------------
	// now the index
	//------------------------------------------------------------------------------------------
	
	if step=='all' | step == 'index' then 
		mprintf('Creating index.htm \n');
		if rhs <= 0 then 
			gener_index() 
		else
			gener_index(dirs,titles)
		end
	end
	
	//------------------------------------------------------------------------------------------
	// now the contents
	//------------------------------------------------------------------------------------------
	
	if step=='all' | step == 'contents' then 
		mprintf('Creating contents.htm  \n');
		if rhs <= 0 then 
			gener_contents() 
		else
			gener_contents(dirs)
		end
	end
	
	//------------------------------------------------------------------------------------------
	// now help workshop (Only under Windows)
	//------------------------------------------------------------------------------------------
	
	if MSDOS then
		if step=='all' | step == 'hw' then 
			mprintf('Creating sciman.hh* \n');
			gener_hh(dirs,titles)
		end
	end

endfunction



function gener_whatis(title)

	//------------------------------------------------------------------------------------------
	// generate a whatis.htm file 
	// using *.xml man files
	//------------------------------------------------------------------------------------------
	
	[lhs,rhs]=argn(0); 
	
	lines(0);
	// look for .xml files
	xml = listfiles('*.xml');
	
	if MSDOS& xml<>[] then 
		// on MSDOS listfiles *.xml also 
		// returns *.xml* !!!!!
		ind = grep(xml,'xml2');
		xml(ind)=[];
	end
	
	if rhs == 1 then 
		whatis_title= title 
	else
		// find the title 
		ind=grep(%helps(:,1),getcwd());
		if ind<>[] then 
			whatis_title= %helps(ind(1),2)
		else
			whatis_title='Help chapter'
		end
	end
  
	head=["<html>"
		"<head>"
		"	<meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
		"	<title>"+whatis_title+"</title>"
		"</head>"
		"<body bgcolor=""#FFFFFF"">"
		"<dl>"];
  
	l=0;
	line=[]
	for k1=1:size(xml,'*')  // loop on .xml files
		path=xml(k1);
		txt=mgetl(path);
		d=grep(txt,"<SHORT_DESCRIPTION");
		f=grep(txt,"</SHORT_DESCRIPTION");
		if d<>[] then 
			
			// Remove <VERB> and </VERB> tags
			
			txt = strsubst(txt,'<VERB>','');
			txt = strsubst(txt,'</VERB>','');
			
			// Loop on the lines of short description if necessary
			for k2=1:size(d,"*")
				tt="";
				for k3=d(k2):f(k2)
					tt=tt+txt(k3);
				end
				i=strindex(tt,"""");
				name=part(tt,i(1)+1:i(2)-1);
				i1=strindex(tt,">");
				i2=strindex(tt,"<");
				desc=stripblanks(part(tt,i1(1)+1:i2(2)-1));
				l=l+1; fname=part(path,[1:length(path)-4])+".htm";
				fname=strsubst(fname,'//','/');
				line(l)="<dd><A HREF="""+fname+""">"+name+"</A> - "+desc+"</dd>";
			end
		end
	end
	text = [head;gsort(line,'g','i');"</dl></body></html>"];
	
	mputl(text,"whatis.htm");
	
endfunction



function gener_index(dirs,txt)

	//------------------------------------------------------------------------------------------
	// use %helps to generate an index file 
	//------------------------------------------------------------------------------------------
	lines(0);
	find_links=find_links;
	sep="/";// if MSDOS then sep="\",end
	[lhs,rhs]=argn(0) 
	if rhs <= 0 then 
	        dirs=%helps(:,1)
		txt=%helps(:,2)
	end
  
	line=["<html>"
		"<head>"
		"	<meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
		"	<title>Index</title>"
		"</head>"
		"<body bgcolor=""#FFFFFF"">"
		"<dl>"];
  
	l=size(line,'*')
	
	//------------------------------------------------------------------------------------------
	// check for whatis 
	//------------------------------------------------------------------------------------------

	for k=1:size(dirs,'*')
		w=dirs(k)+sep+"whatis.htm";
		if fileinfo(w)==[] then 
			error('file '+w+' not found');
			return 
		end 
		l=l+1;
		w=strsubst(w,'\','/');
		w=strsubst(w,'//','/');
		line(l)="<dd><A HREF="""+w+""">"+txt(k)+"</A></dd>"
	end
	line = [line;"</dl></body></html>"]
  
	mputl(line,"index.htm")
	
endfunction



function flag = gener_links()

	//------------------------------------------------------------------------------------------	
	// returns %t if new files were created
	//------------------------------------------------------------------------------------------
	
	lines(0);
	flag=%f
	
	// look for .xml files
	// look for .xml files
	
	xml = listfiles('*.xml');
	for k1=1:size(xml,'*')  // loop on .xml files
		path=xml(k1)
		//write(%io(2),path)
		if newest(path,strsubst(path,".xml",".htm"))==1 then
			find_links(path,path+"2")
			flag = %t
		end
	end
	
endfunction 



function gener_contents(dirs1)

	//------------------------------------------------------------------------------------------
	// contents.htm 
	//------------------------------------------------------------------------------------------
  
	[lhs,rhs]=argn(0) 
	if rhs <= 0 then 
		dirs = dirname(%helps(:,1));
		base = basename(%helps(:,1));
	else
		dirs = dirname(dirs1)
		base = basename(dirs1)
	end

	//------------------------------------------------------------------------------------------
	// help in the std man directory
	//------------------------------------------------------------------------------------------
	std= grep(dirs,SCI)
	n=size(dirs,'*')

	//------------------------------------------------------------------------------------------	
	// help in the non std man directory
	//------------------------------------------------------------------------------------------
	
	nstd=1:n;
	nstd(std)=[];
	
	full_whatis=[];
	full_whatis_name=[];

	if std<>[] then 
		for k=std
			d = dirs(k)+'/'+base(k);
			[fd,ierr]=mopen(d+"/whatis.htm","r");
			if ierr<>0 then
				warning(" whatis file missing in "+d+". Directory ignored")
			else
				whatis=mgetl(fd);mclose(fd);
				ind=grep(whatis,'</A>');
				whatis=whatis(ind);
				for k1=1:size(whatis,'*')
					lwhatis = whatis(k1);
					i=strindex(lwhatis,">"); j=strindex(lwhatis,"</A>");
					lkey=part(lwhatis,i(2)+1:j-1);
					full_whatis_name=[full_whatis_name;lkey];
				end
				whatis=strsubst(whatis,"HREF=""","HREF="""+base(k)+"/");
				whatis=strsubst(whatis,'//','/');
				full_whatis=[full_whatis;whatis];
			end 
		end
	end
  
	if nstd<>[] then 
		for k=nstd
			d = dirs(k)+'/'+base(k);
			[fd,ierr]=mopen(d+"/whatis.htm","r");
			if ierr<>0 then
				warning(" whatis file missing in "+d+". Directory ignored")
			else
				whatis=mgetl(fd);mclose(fd);
				ind=grep(whatis,'</A>');
				whatis=whatis(ind);
				for k1=1:size(whatis,'*')
					lwhatis = whatis(k1);
					i=strindex(lwhatis,">"); j=strindex(lwhatis,"</A>");
					lkey=part(lwhatis,i(2)+1:j-1);
					full_whatis_name=[full_whatis_name;lkey];
				end
				if dirs(k)<>'.' then 
					d=dirs(k)+'/'+base(k)+"/";
				else 
					d=base(k)+"/";
				end
				whatis=strsubst(whatis,"HREF=""","HREF="""+d);
				whatis=strsubst(whatis,'//','/');
				full_whatis=[full_whatis;whatis];
			end
		end
	end
	
	[sv,sk]=sort(full_whatis_name);
	full_whatis=full_whatis(sk);
	
	select LANGUAGE 
		case 'fr' then title =  "<H2>Fonctions Scilab</H2>";
		case 'eng' then title =  "<H2>Scilab functions</H2>";
	end
  
	full_whatis=["<html>"
		"<head>"
		"	<meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
		"	<title>Scilab General Index</title>"
		"</head>"
		"<body bgcolor=""#FFFFFF"">";
		title;
		full_whatis;
		"</body></html>"
		];
	
	mputl(full_whatis,'contents.htm')
  
endfunction 



function gener_hh(dirs,titles)

	//------------------------------------------------------------------------------------------
	// Modified by Pierre MARECHAL
	// Scilab Team
	// Copyright INRIA
	// Date : 31/03/2005
	//------------------------------------------------------------------------------------------
	
	//------------------------------------------------------------------------------------------
	// first produce a scilab.hhk file 
	// (index file) 
	//------------------------------------------------------------------------------------------
	
	base = dirs 

	// help in the std man directory 
	n=size(dirs,'*')

	items=[];
	names=[];

	// just for the std directories 

	for k=1:n 
		// look for .xml files
		files = listfiles(base(k)+'/*.xml');
		if files<>[] then
			fbase = basename(files);
			name = strsubst(base(k)+"\"+fbase,"\\","/");
			items=[items;
				"            <LI><OBJECT type=""text/sitemap""><param name=""Local"" value=""" ...
				+ name +  ...
				".htm""><param name=""Name"" value="""+ fbase+"""></OBJECT>"];
			names=[names;files];
		end
	end
	
	[sv,sk]=sort(names);
	items=items(sk);
	
	full_index=[
		"<HTML>";
		"    <HEAD>";
		"    </HEAD>";
		"    <BODY>";
		"        <UL>";
		"            <LI><OBJECT type=""text/sitemap"">";
		"                     <param name=""Name"" value=""Scilab documentation"">";
		"                     </OBJECT>";
		"            <UL>";
		items;
		"            </UL>";
		"        </UL>"
		"    </BODY>";
		"</HTML>"];
		
	mputl(full_index,'sciman.hhk')

	//------------------------------------------------------------------------------------------
	// produce a scilab.hhc file 
	// (contents) 
	//------------------------------------------------------------------------------------------
    
	items = [];
	for k=1:n 
		name(k) = strsubst(base(k)+"\","\\","/");
		items=[items;
			"                <LI><OBJECT type=""text/sitemap"">";
			"                    <param name=""Local"" value="""+ name(k)+"whatis.htm"">";
			"                    <param name=""Name"" value="""+ titles(k)+""">";
			"                    <param name=""ImageNumber"" value=""11"">";
			"                    </OBJECT>"];
	end
	
	contents=[
		"<HTML>";
		"    <HEAD>";
		"    </HEAD>";
		"    <BODY>";
		"        <UL>";
		"            <LI><OBJECT type=""text/sitemap"">";
		"                     <param name=""Name"" value=""Scilab documentation"">";
		"                     </OBJECT>";
		"            <UL>";
		items;
		"            </UL>";
		"        </UL>"
		"    </BODY>";
		"</HTML>"];
		
	mputl(contents,'sciman.hhc')
	
	//------------------------------------------------------------------------------------------
	// produce a scilab.hhp file 
	//------------------------------------------------------------------------------------------
       
	head=["[OPTIONS]";
		"Compatibility=1.1";
		"Compiled file=sciman.chm";
		"Contents file=sciman.hhc";
		"Default Window=sciman";
		"Default topic=index.htm";
		"Display compile progress=No";
		"Full-text search=Yes";
		"Index file=sciman.hhk";
		"Language=0x409"; 
		"Title=Scilab Documentation";
		"";
		"[WINDOWS]";
		"sciman=""Scilab Documentation"",""sciman.hhc"",""sciman.hhk"",""index.htm"",""index.htm"",,,,,0x2520,220,0x384e,[84,16,784,504],,,,0,,,0";
		"";
		"[FILES]"];

	items=base+"*.htm";
       
	mputl([head;items],'sciman.hhp')  

endfunction

