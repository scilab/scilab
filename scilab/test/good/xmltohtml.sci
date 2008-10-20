function xmltohtml(dirs,titles,xsl,step,directory_language,default_language)
	
	// =========================================================================================
	// Authors : Jean-Philippe CHANCELIER, Pierre MARECHAL
	// Copyright INRIA/Enpc
	// 
	// dirs is a set of directories
	// for which html manuals are to be generated 
	// + and index and toc file 
	// titles are associated title strings (optional or [])
	// if dirs is not specified or [] then 
	// standard scilab man are assumed and titles 
	// are searched in %helps 
	// updated by HUYNH Olivier on the 9/03/2004
	// =========================================================================================
	
	// =========================================================================================
	// step :
	//
	//   - add_default_language : Complete the on-line help with the default language. replace the missing
	//                    help files by copying the corresponding files from the default language
	//
	//   - whatis       : A whatis.htm file is generated in each directory using information given 
	//                    in the "SHORT_DESCRIPTION" tag of each xml file and the given title.
	//
	//   - index        : The tags LINK are analysed, and xmltohtml tries to resolve the references
	//                    using the already known help files defined in the %helps variable.
	// 
	//   - html         : It translates the xml file to html using external program xsltproc and the
	//                    given xsl file which should be located in SCIDIR/man/LANGUAGE/ The generated
	//                    html files are located together with the corresponding xml ones.
	//
	//   - contents     : It produces a contents.xml file
	//
	//   - all          : All the previous steps
	// =========================================================================================
	
	global LANGUAGE %helps
	
	//------------------------------------------------------------------------------------------
	// Sauvegarde du chemin courant et de la variable %helps
	//------------------------------------------------------------------------------------------
	
	current_directory = pwd();
	saved_helps = %helps;
	
	//to load the subfunction change_old_man this is required to produce the whatis.htm files
	//associated with old style manuals
	change_old_man();
	
	//------------------------------------------------------------------------------------------
	
	[lhs,rhs]=argn(0);
	
	// Trop de paramètres
	// -----------------------------------------------------------------------------------------
	
	if rhs > 6 then
		error(39);
		%helps = saved_helps;
		chdir(current_directory);
		return;
	end
	
	// Cas par défaut : construction de l'aide en ligne de Scilab
	// -----------------------------------------------------------------------------------------
	
	if (rhs <= 0) | ((rhs == 1) & (dirs == [])) then
		
		dirs_to_build = %helps;
		
		//----------------------------------------------------------------------------------
		// Patch because scicos is not written in xml
		//----------------------------------------------------------------------------------
		scs = grep(dirs_to_build,'scicos');
		if size(scs,'*') == 1 then dirs_to_build(scs,:)=[]; end
		// End of patch --------------------------------------------------------------------
		
		dirs = dirs_to_build(:,1);
		titles = dirs_to_build(:,2);
		directory_language = [];
		default_language = [];
		language_system = [];
		
		select LANGUAGE
		
		case 'eng' then
			xsl = pathconvert(SCI+"/modules/helptools/help_eng.xsl",%f,%f);
			for k=1:size(dirs,'*')
				directory_language = [directory_language;"eng"];
				language_system = [language_system;%F];
			end
		
		case 'fr' then
			xsl = pathconvert(SCI+"/modules/helptools/help_fr.xsl",%f,%f);
			for k=1:size(dirs,'*')
				directory_language = [directory_language;"fr"];
				default_language = [default_language;"eng"];
				language_system = [language_system;%T];
			end
		end
		
		step = "all";
	
	// Cas ou seulement le ou les répertoires sont précisés
	// -----------------------------------------------------------------------------------------
	
	elseif (rhs == 1) & (dirs <> []) then
		
		step = "all";
		language_system = [];
		titles = [];
		
		select LANGUAGE
		
		case 'eng' then
			xsl = pathconvert(SCI+"/modules/helptools/help_eng.xsl",%f,%f);
			for k=1:size(dirs,'*')
				titles = [titles;"Help chapter ("+dirs(k)+")"];
				language_system = [language_system;%F];
			end
		case 'fr' then
			xsl = pathconvert(SCI+"/modules/helptools/help_fr.xsl",%f,%f);
			for k=1:size(dirs,'*')
				titles = [titles;"Chapitre de help ("+dirs(k)+")"];
				language_system = [language_system;%F];
			end
		end
		
	// Cas ou seulement le ou les répertoires ainsi que le ou les titres sont précisés
	// -----------------------------------------------------------------------------------------
	
	elseif rhs == 2 then
		
		step = "all";
		language_system = [];
		
		for k=1:size(dirs,'*')
			language_system = [language_system;%F];
		end
		
		select LANGUAGE
		
		case 'eng' then
			xsl = pathconvert(SCI+"/modules/helptools/help_eng.xsl",%f,%f);
			
		case 'fr' then
			xsl = pathconvert(SCI+"/modules/helptools/help_fr.xsl",%f,%f);
		
		end

	// Cas le ou les répertoires ainsi que le ou les titres sont précisés
	// ainsi que le fichier xsl
	// -----------------------------------------------------------------------------------------
	
	elseif rhs == 3 then
		
		step = "all";
		language_system = [];
		
		for k=1:size(dirs,'*')
			language_system = [language_system;%F];
		end
		
		if ~exists("xsl") | xsl == [] | xsl == "" then
			select LANGUAGE
			
			case 'eng' then
				xsl = pathconvert(SCI+"/modules/helptools/help_eng.xsl",%f,%f);
				
			case 'fr' then
				xsl = pathconvert(SCI+"/modules/helptools/help_fr.xsl",%f,%f);
			
			end
		end
		
	// Cas les répertoires,les titres,le fichier xsl et le step sont précisé
	// -----------------------------------------------------------------------------------------
	
	elseif rhs == 4 then
		
		language_system = [];
		
		for k=1:size(dirs,'*')
			language_system = [language_system;%F];
		end
		
		if ~exists("all") | all == [] | all == "" then step = "all"; end
		
		if ~exists("xsl") | xsl == [] | xsl == "" then
			select LANGUAGE
			
			case 'eng' then
				xsl = pathconvert(SCI+"/modules/helptools/help_eng.xsl",%f,%f);
				
			case 'fr' then
				xsl = pathconvert(SCI+"/modules/helptools/help_fr.xsl",%f,%f);
			
			end
		end
	
	// Cas les répertoires,les titres,le fichier xsl, le step ainsi que la langue du répertoire
	// sont précisées
	// -----------------------------------------------------------------------------------------
	
	elseif rhs == 5 then
		
		language_system = [];
		
		for k=1:size(dirs,'*')
			language_system = [language_system;%F];
		end
		
		if ~exists("all") | all == [] | all == "" then step = "all"; end
		
		if ~exists("xsl") | xsl == [] | xsl == "" then
			select LANGUAGE
			
			case 'eng' then
				xsl = pathconvert(SCI+"/modules/helptools/help_eng.xsl",%f,%f);
				
			case 'fr' then
				xsl = pathconvert(SCI+"/modules/helptools/help_fr.xsl",%f,%f);
			
			end
		end
	
	// Cas où tous est précisée
	// -----------------------------------------------------------------------------------------
	
	elseif rhs == 6 then
		
		language_system = [];
		
		for k=1:size(dirs,'*')
			if isdir(pathconvert(dirs(k)+"/../"+default_language(k),%f,%f)) then
				language_system = [language_system;%T];
			end
		end
		
		if ~exists("all") | all == [] | all == "" then step = "all"; end
		
		if ~exists("xsl") | xsl == [] | xsl == "" then
			select LANGUAGE
			
			case 'eng' then
				xsl = pathconvert(SCI+"/modules/helptools/help_eng.xsl",%f,%f);
				
			case 'fr' then
				xsl = pathconvert(SCI+"/modules/helptools/help_fr.xsl",%f,%f);
			
			end
		end
		
	end
	
	// On transforme le ou les chemins donnés en chemin absolu
	// -----------------------------------------------------------------------------------------
	
	for k=1:size(dirs,'*');
		chdir(dirs(k));
		if MSDOS then
			dirs(k) = getlongpathname(pwd());
		else
			dirs(k) = pwd();
		end
		chdir(current_directory);
	end
	
	//------------------------------------------------------------------------------------------
	// On etablit la liste des répertoires nécéssitants d'être reconstruit
	//------------------------------------------------------------------------------------------
	
	need_to_be_build_tab = [];
	
	for k=1:size(dirs,'*');
		if language_system(k) then
			need_to_be_build_tab = [need_to_be_build_tab;need_to_be_build(dirs(k),directory_language(k),default_language(k))];
		else
			need_to_be_build_tab = [need_to_be_build_tab;need_to_be_build(dirs(k))];
		end
	end
	
	if ~or(need_to_be_build_tab) then
		mprintf("   HTML files are up-to-date\n");
		return;
	end
	
	
	// Nombre de répertoire ayant besoin d'une modification
	// -----------------------------------------------------------------------------------------
	
	nb_dir = size( find(need_to_be_build_tab == %T) , '*' );
	
	//------------------------------------------------------------------------------------------
	// Complete the on-line help with the default language
	//------------------------------------------------------------------------------------------
	
	if step == 'all' | step == 'add_default_language' then
		
		displaydone = 0;
		
		for k=1:size(dirs,'*')
			if need_to_be_build_tab(k) & language_system(k) then
				default_language_path = pathconvert(dirs(k)+"/../"+default_language(k),%f,%f);
				if nb_dir > 1 then
					if displaydone == 0 then
						mprintf("\nCopying missing files copied from\n");
						displaydone = 1;
					end
					mprintf("\t%s\n",default_language_path);
				else
					mprintf("\nCopying missing from %s\n",default_language_path);
				end
				complete_with_default_language(dirs(k),directory_language(k),default_language(k));
			end
		end
	end
	
	//------------------------------------------------------------------------------------------
	// build all the whatis
	//------------------------------------------------------------------------------------------
	
	if step=='all' | step == 'whatis' then 
		displaydone = 0;
		for k=1:size(dirs,'*');
			if need_to_be_build_tab(k) then
				if nb_dir > 1 then
					if displaydone == 0 then
						mprintf("\nCreating whatis.htm\n");
						displaydone = 1;
					end
					mprintf("\t%s\n",dirs(k));
				else
					mprintf("\nCreating whatis.htm in %s\n",dirs(k));
				end
				dirs(k)=pathconvert(dirs(k),%f,%f);
				chdir(dirs(k));
				gener_whatis(titles(k));
			end
		end
	end
	
	//------------------------------------------------------------------------------------------
	// the perform the  html generation
	//------------------------------------------------------------------------------------------
	
	if step=='all' | step == 'html' then 
		
		dirs=stripblanks(dirs)
		
		for k=1:size(dirs,'*');
			if need_to_be_build_tab(k) then
				
				mprintf("\nProcessing chapter %s\n",dirs(k));
				
				chdir(dirs(k));
				
				// On vérifie l'existence du fichier xls
				
				if fileinfo(xsl) == [] then
					error("cannot open xsl file"+xsl);
					chdir(current_directory);
					%helps = saved_helps;
					return;
				end
					
				xml = listfiles('*.xml');
				
				if xml <> [] then 
					for k1=1:size(xml,'*')  // loop on .xml files 
						if language_system(k) then
							ok = xmlfiletohtml(xml(k1),xsl,directory_language(k),default_language(k));
						else
							ok = xmlfiletohtml(xml(k1),xsl);
						end
						
						if ~ok then
							chdir(current_directory);
							%helps = saved_helps;
							return;
						end
					end
				end
			
			end
		end
	end
	
	chdir(current_directory);
	
	//------------------------------------------------------------------------------------------
	// now the index
	//------------------------------------------------------------------------------------------
	
	if step=='all' | step == 'index' then 
		index_file = pathconvert(SCI+"/modules/helptools/index_"+LANGUAGE+".htm",%f,%t);
		mprintf("\nCreating %s\n",index_file);
		gener_index(dirs,titles);
	end
	
	//------------------------------------------------------------------------------------------
	// now the contents
	//------------------------------------------------------------------------------------------
	
	if step=='all' | step == 'contents' then 
		contents_file = pathconvert(SCI+"/modules/helptools/contents_"+LANGUAGE+".htm",%f,%t);
		mprintf("\nCreating %s\n",contents_file);
		if rhs <= 0 then 
			gener_contents() 
		else
			gener_contents(dirs)
		end
	end
	
	//------------------------------------------------------------------------------------------
	// now help workshop (Only under Windows and only if
	// we build the scilab man (SCI/man/fr and SCI/man/eng )
	//------------------------------------------------------------------------------------------
	
	if MSDOS then
		if step=='all' | step == 'hw' then
			if (strindex(dirs(1),'\eng\') <> []) | (strindex(dirs(1),'\fr\') <> []) then
				mprintf("\nCreating sciman.hh* \n");
				gener_hh(dirs,titles)
			end
		end
	end
	
	//------------------------------------------------------------------------------------------
	// Delete the xml files translated into the default language 
	//------------------------------------------------------------------------------------------
	
	if (step=='all' | step == 'add_default_language') then
		displaydone = 0;
		for k=1:size(dirs,'*')
			if need_to_be_build_tab(k) & language_system(k) then
				
				default_language_path = pathconvert(dirs(k)+"/../"+default_language(k),%f,%f);
				
				if nb_dir > 1 then
					if displaydone == 0 then
						mprintf("\nDeleting files copied from\n");
						displaydone = 1;
					end
					mprintf("\t%s\n",default_language_path);
				else
					mprintf("\nDeleting files copied from %s\n",default_language_path);
				end
				del_default_language_xml_files(dirs(k),directory_language(k));
			end
		end
	end
	
	//------------------------------------------------------------------------------------------
	// Création du fichier "directory/.last_successful_build"
	//------------------------------------------------------------------------------------------
	
	if step == 'all' then
		for k=1:size(dirs,'*');
			if need_to_be_build_tab(k) then
				dateToPrint = msprintf("last_successful_build_value = %d",getdate('s'));
				mputl(dateToPrint,pathconvert(dirs(k)+"/.last_successful_build",%f,%f));
			end
		end
	end
	
	//------------------------------------------------------------------------------------------
	// On remet l'environement initial
	//------------------------------------------------------------------------------------------
	
	chdir(current_directory);
	%helps = saved_helps;
	
endfunction



function gener_whatis(wtitle)

	//------------------------------------------------------------------------------------------
	// generate a whatis.htm file and a .list_htm
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
		whatis_title= wtitle 
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
		"	<head>"
		"		<meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
		"		<title>"+whatis_title+"</title>"
		"	</head>"
		""
		"	<body bgcolor=""#FFFFFF"">"
		"		<dl>"];
  
	l=0;
	
	line = [];
	line2 = [];
	
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
				line(l)="			<dd><A HREF="""+fname+""">"+name+"</A> - "+desc+"</dd>";
				line2(l)="- "+name+"==>"+fname;
			end
		end
	end
	
	//------------------------------------------------------------------------------------------
	// Fix bug 1978
	// first trim the htm file name, for instance this element of matrix "line":
	//        "<dd><A HREF=""if.htm"">if</A> - conditional execution</dd>"
	// becomes the following element of matrix "trimmed":
	//        "if</A> - conditional execution</dd>"
	//------------------------------------------------------------------------------------------
	
	trimmed=emptystr(line);
	for i=1:size(line,"r")
		r=strindex(line(i),".htm"">")+6;
		trimmed(i)=part(line(i),r:length(line(i)));
	end
	
	//  now sort the trimmed array, and keep the k indices describing how it was sorted
	[trimmed,k] = gsort(trimmed,'g','i');
	
	//  finally apply the indices k to the original matrix "line"
	line=line(k(:))
	
	// end of fix for bug 1978
	//------------------------------------------------------------------------------------------
	
	text = [head
		line
		"		</dl>"
		"	</body>"
		"</html>"]
	
	mputl(text,"whatis.htm");
	mputl(line2,".list_htm");
	
endfunction


function gener_index(dirs,titles)

	//------------------------------------------------------------------------------------------
	// use %helps to generate an index file 
	//------------------------------------------------------------------------------------------
	
	// On ajoute le ou les répertoire
	
	saved_help = %helps;
	
	[lhs,rhs]=argn(0)
	 
	if rhs == 2 then
		for k=1:size(dirs,'*')
			add_help_chapter(titles(k),dirs(k));
		end
	end
	
	dirs = %helps(:,1);
	titles = %helps(:,2);
	
	lines(0);
	
	line=["<html>"
		"	<head>"
		"		<meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
		"		<title>Index</title>"
		"		<link href=""../sci_styles.css"" rel=""stylesheet"" type=""text/css"">"
		"	</head>"
		"	"
		"	<body bgcolor=""#FFFFFF"">"
		"		<dl>"];
  
	l=size(line,'*')
	
	//------------------------------------------------------------------------------------------
	// check for whatis 
	//------------------------------------------------------------------------------------------

	for k=1:size(dirs,'*')
		
		w = pathconvert(dirs(k)+"/whatis.htm",%f,%f);
		w = strsubst(w,pathconvert(SCI,%f,%t),"../../");
		w = pathconvert(w,%f,%t);
		
// 		if fileinfo(w) ==[] then
// 			error("file "+w+" not found");
// 			return 
// 		end 
		
		l=l+1;
		
		w=pathconvert(getlongpathname(w),%f,%f);
		line(l)="			<dd><A HREF="""+w+""">"+titles(k)+"</A></dd>";
	end
	
	line = [line;
		"		</dl>"
		"	</body>"
		"</html>"]
	
	mputl(line,pathconvert(SCI+"/modules/helptools/index_"+LANGUAGE+".htm",%f,%t));
	
	%helps = saved_help;
	
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
		case 'fr' then type_title =  "		<H2>Fonctions Scilab</H2>";
		case 'eng' then type_title =  "		<H2>Scilab functions</H2>";
	end
  
	full_whatis=["<html>"
		"	<head>"
		"		<meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
		"		<title>Scilab General Index</title>"
		"	</head>"
		"	<body bgcolor=""#FFFFFF"">";
		type_title;
		"		<dl>"
		full_whatis;
		"		<dl>"
		"	</body>"
		"</html>"
		];
	
	mputl(full_whatis,pathconvert(SCI+"/modules/helptools/contents_"+LANGUAGE+".htm",%f,%t));
  
endfunction



function gener_hh(dirs,titles)

	//------------------------------------------------------------------------------------------
	// Modified by Pierre MARECHAL
	// Scilab Team
	// Copyright INRIA
	// Date : 22/02/2006
	//------------------------------------------------------------------------------------------
	
	//------------------------------------------------------------------------------------------
	// first produce a scilab.hhk file 
	// (index file) 
	//------------------------------------------------------------------------------------------
	
	base = dirs 
	
	if ( strindex(base(1),'\eng\') <> [] ) then
		manpath = pathconvert(getlongpathname(SCI)+'/man/eng/',%t,%f,'w');
		doctitle = "Scilab documentation";
	elseif ( strindex(base(1),'\fr\') <> [] ) then
		manpath = pathconvert(getlongpathname(SCI)+'/man/fr/',%t,%f,'w');
		doctitle = "Documentation";
	end
	
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
			name = strsubst(name,manpath,'');
			
			items=[items;"			<LI><OBJECT type=""text/sitemap""><param name=""Local"" value=""" + name + ".htm""><param name=""Name"" value="""+fbase+"""></OBJECT>"];
			names=[names;files];
		end
	end
	
	[sv,sk]=sort(names);
	items=items(sk);
	
	full_index=[
		"<HTML>";
		" 	<HEAD>";
		"	</HEAD>";
		"	<BODY>";
		"		<UL>";
		items;
		"		</UL>"
		"	</BODY>";
		"</HTML>"];
		
	mputl(full_index,'sciman.hhk')

	//------------------------------------------------------------------------------------------
	// produce a scilab.hhc file 
	// (contents) 
	//------------------------------------------------------------------------------------------
    
	items = [];
	
	for k=1:n 
		
		names_II = [];
		items_II = [];
		
		name(k) = strsubst(base(k)+"\","\\","/");
		name(k) = strsubst(name(k),manpath,'');
		
		items=[items;
			"				<LI><OBJECT type=""text/sitemap"">";
			"						<param name=""Local"" value="""+ name(k)+"whatis.htm"">";
			"						<param name=""Name"" value="""+ titles(k)+""">";
			"						<param name=""ImageNumber"" value=""1"">";
			"					</OBJECT>"];
			
		// look for .xml files
		
		files_II = listfiles(base(k)+'/*.xml');
		if files_II<>[] then
			fbase_II = basename(files_II);
			name_II = strsubst(base(k)+"\"+fbase_II,"\\","/");
			name_II = strsubst(name_II,manpath,'');			
			items_II=[items_II;"					<LI><OBJECT type=""text/sitemap""><param name=""Local"" value=""" + name_II + ".htm""><param name=""Name"" value="""+fbase_II+"""><param name=""ImageNumber"" value=""11""></OBJECT>"];
			names_II=[names_II;files_II];
		end
		
		[sv,sk]=sort(names_II);
		items_II=items_II(sk);
		
		items=[items;
		"				<UL>";
		items_II;
		"				</UL>"];
		
	end
	
	contents=[
		"<HTML>";
		"	<HEAD>";
		"	</HEAD>";
		" 	<BODY>";
		"		<OBJECT type=""text/site properties"">";
		"			<param name=""FrameName"" value=""right"">";
		"			<param name=""ImageType"" value=""Folder"">";
		"			<param name=""Window Styles"" value=""0xC01627"">";
		"			<param name=""Foreground"" value=""0xFFD5D5"">";
		"			<param name=""Background"" value=""0xFFD5D5"">";
		"			<param name=""Font"" value=""Palatino Linotype,10,0"">";
		"		</OBJECT>";
		"		<UL>";
		"			<LI><OBJECT type=""text/sitemap"">";
		"					<param name=""Name"" value="""+doctitle+""">";
		"					<param name=""Local"" value=""index.htm"">";
		"					<param name=""ImageNumber"" value=""15"">";
		"				</OBJECT>";
		"			<UL>";
		items;
		"			</UL>";
		"		</UL>";
		"	</BODY>";
		"</HTML>"];
		
	mputl(contents,'sciman.hhc')
	
	//------------------------------------------------------------------------------------------
	// produce a scilab.hhp file 
	//------------------------------------------------------------------------------------------
       
	// Get the index.htm path
	
	head=["[OPTIONS]";
		"Compatibility=1.1 or later";
		"Compiled file=sciman.chm";
		"Contents file=sciman.hhc";
		"Default Window=sciman";
		"Default topic=index.htm";
		"Display compile progress=Yes";
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



function complete_with_default_language(directory,directory_language,default_language)
	
	//------------------------------------------------------------------------------------------
	// Author : Pierre MARECHAL
	// Scilab Team
	// Copyright INRIA
	// Date : 26, july 2006
	//
	// Cette macro complète un répertoire dont certaines aides en ligne sont manquantes
	// Elle le complète avec les aides en ligne de la langue par défaut
	//
	// macro non-visible de l'utilisateur
	//------------------------------------------------------------------------------------------
	
	// Directory traitée
	directory = pathconvert(directory,%f,%f);
	
	//------------------------------------------------------------------------------------------
	// Nettoyage du répertoire
	// Si il existe un fichier .list_<directory_language> (fichier contenant tous les
	// fichiers traduits dans la langue du répertoire), on supprime dans l'ordre :
	//   1. Tous les fichiers n'appartenant pas à la liste contenue dans .list_<directory_language>
	//   2. Tous les fichiers de la forme .list_<language>
	//------------------------------------------------------------------------------------------
	
	if listfiles(pathconvert(directory+"/.list_"+directory_language,%f,%f)) <> [] then
		del_default_language_xml_files(directory,directory_language);
		mdelete(directory+"/.list_*");
	end
	
	//------------------------------------------------------------------------------------------
	// Construction du fichier list_<directory_language> contenant la liste des fichiers traduits
	// dans la langue associée au répertoire
	//------------------------------------------------------------------------------------------
	
	directory_language_xml_files = basename(listfiles(directory+"/*.xml"));
	if directory_language_xml_files <> [] then
		mputl(directory_language_xml_files,directory+"/.list_"+directory_language);
	else
		mputl('',directory+"/.list_"+directory_language);
	end
	
	//------------------------------------------------------------------------------------------
	// Construction du fichier list_<default_language> contenant la liste des fichiers 
	// non traduits dans la langue associée au répertoire qui seront récupérés depuis le 
	// répertoire de la langue par défaut
	//------------------------------------------------------------------------------------------
	
	// Tous les fichiers contenus dans <directory>/../<default_language> pour commencer
	// On afinnera par la suite
	default_language_xml_files = basename(listfiles(pathconvert(directory+"/../"+default_language+"/*.xml",%f,%f)));
	
	// On supprime de "default_language_xml_files" tous les élément contenus dans 
	// "directory_language_xml_files", c'est à dire tous les fichiers déja traduit dans la langue
	// associée au répertoire.
	
	for i=1:size(directory_language_xml_files,'*');
		default_language_xml_files(find(default_language_xml_files==directory_language_xml_files(i)))=[];
	end
	
	// Création du fichier
	if default_language_xml_files <> [] then
		mputl(default_language_xml_files,pathconvert(directory+"/.list_"+default_language,%f,%f));
	else
		mputl('',pathconvert(directory+"/.list_"+default_language,%f,%f));
	end
		
	//------------------------------------------------------------------------------------------
	// Copie des fichiers additionnels
	//------------------------------------------------------------------------------------------
	
	for i=1:size(default_language_xml_files,'*');
		tmp_file = mgetl(pathconvert(directory+"/../"+default_language+"/"+default_language_xml_files(i)+".xml",%f,%f));
		mputl(tmp_file,pathconvert(directory+"/"+default_language_xml_files(i)+".xml",%f,%f));
	end
	
endfunction


function del_default_language_xml_files(directory,directory_language)
	
	//------------------------------------------------------------------------------------------
	// Author : Pierre MARECHAL
	// Scilab Team
	// Copyright INRIA
	// Date : 26, july 2006
	//
	// Cette macro détruit tous les fichiers xml qui ne sont pas traduit dans la langue associée
	// au répertoire
	//
	// macro non-visible de l'utilisateur
	//------------------------------------------------------------------------------------------
	
	// Directory traitée
	directory = pathconvert(directory,%f,%t);
	
	if listfiles(pathconvert(directory+"/.list_"+directory_language,%f,%f)) <> [] then
		
		// Récupération de la liste des fichiers xml copiés depuis le répertoire de la langue par défaut
		directory_language_xml_files = mgetl(pathconvert(directory+"/.list_"+directory_language,%f,%f));
		
		// Liste de tous les fichiers xml contenu dans le répertoire
		all_files = basename(listfiles(directory+"/*.xml"));
		
		// On retire de "all_files" tous les éléments appartenant à "xml_directory_language_files"
		// Ce sont les aides en ligne traduite dans la langue associée répertoire
		for i=1:size(directory_language_xml_files,'*');
			all_files(find(all_files==directory_language_xml_files(i)))=[];
		end
		
		// Destruction des fichiers restant dans all_files
		for i=1:size(all_files,'*');
			mdelete(pathconvert(directory+"/"+all_files(i)+".xml",%f,%f));
		end
	end
	
endfunction


function result = need_to_be_build(directory,directory_language,default_language)
	
	//------------------------------------------------------------------------------------------
	// Author : Pierre MARECHAL
	// Scilab Team
	// Copyright INRIA
	// Date : 27, july 2006
	//
	// Cette fonction a pour but de déterminer si le répertoire a besoin d'être reconstruit ou
	// pas. 
	//
	// On détermine la date de dernière modification la plus récente parmi les date de dernière
	// modification suivantes :
	//     -  date de dernière modification du répertoire "directory".
	//     -  dates de dernière modification des fichiers XML du répertoire "directory".
	//     -  date de dernière modification du répertoire "directory/../<default_language>"
	//        si le système de multilinguisme est utilisé
	//      - dates de dernière modification des fichiers XML du répertoire 
	//        "directory/../<default_language>" si le système de multilinguisme est utilisé
	//
	// Ensuite cette valeur est comparée à la valeur contenue dans le fichier 
	// "directory/.last_successful_build". Si elle est plus grande, need_to_be_build renvoie %T
	//
	// Si le fichier "directory/.last_successful_build" n'existe pas, l'aide n'a jamais été 
	// construite donc need_to_be_build renvoie %T
	// 
	//------------------------------------------------------------------------------------------
	
	[lhs,rhs]=argn(0);
	
	if fileinfo(pathconvert(directory+"/.last_successful_build",%f,%f)) == [] then
		result = %T;
		return;
	else
		exec(pathconvert(directory+"/.last_successful_build",%f,%f),-1);
		
		// ---------------------------------------------------------------------------------
		
		directory_info = fileinfo(directory);
		max_change_date = directory_info(6);
		
		xml_file_list = listfiles(directory+"/*.xml");
		
		for i=1:size(xml_file_list,'*');
			file_date = fileinfo(xml_file_list(i));
			if file_date(6) > max_change_date then
				max_change_date = file_date(6);
			end
		end
		
		if max_change_date > last_successful_build_value then
			result = %T;
			return;
		else
			if rhs == 3 then
				
				default_language_directory_info = fileinfo(pathconvert(directory+"/../"+default_language,%f,%f));
				
				if default_language_directory_info(6) > max_change_date then
					max_change_date = default_language_directory_info(6);
				end
				
				xml_default_language_file_list = listfiles(pathconvert(directory+"/../"+default_language+"/*.xml",%f,%f));
				
				for i=1:size(xml_default_language_file_list,'*');
					file_date = fileinfo(xml_default_language_file_list(i));
					if file_date(6) > max_change_date then
						max_change_date = file_date(6);
					end
				end
			end
			
			if max_change_date > last_successful_build_value then
				result = %T;
				return;
			else
				result = %F;
				return;
			end
		end
	end
	
endfunction
