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
	//   - del_default_language : 
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
	// get man_dirs, il s'agit des directory des répertoires d'origine
	//------------------------------------------------------------------------------------------
	
	initial_help_directory = initial_help_chapters(LANGUAGE);
	
	//------------------------------------------------------------------------------------------
	//patch because scicos is not written in xml
	//------------------------------------------------------------------------------------------
	
	scs=grep(initial_help_directory,'scicos');
	
	if size(scs,'*') == 1 then
		initial_help_directory(scs)=[];
	end
	
	man_dirs = initial_help_directory(:,1);
	man_titles= initial_help_directory(:,2);
	
	if man_dirs == [] then 
		error('I cannot find man dirs');
		return 
	end
	
	[lhs,rhs]=argn(0)
	
	if rhs > 6 then error(39),return; end
	
	if rhs <= 3 then step='all'; end
	if rhs <= 2 then xsl= 'html-rev.xsl'; end
	if rhs <= 1 then titles= H + emptystr(dirs);end
	if rhs <= 0 then dirs = man_dirs ;end
	if titles==[] then titles= H+ emptystr(dirs);end
	
	// default values
	
	if dirs == [] then
		
		dirs = man_dirs;
		titles= man_titles;
		
		for k=1:size(dirs,'*');
			select LANGUAGE
			case 'eng' then
				directory_language = [directory_language;"eng"];
			case 'fr' then
				directory_language = [directory_language;"fr"];
				default_language = [default_language;"eng"];
			end
		end
	end
	
	//------------------------------------------------------------------------------------------
	// Complete the on-line help with the default language
	//------------------------------------------------------------------------------------------
	
	if (step == 'all' | step == 'add_default_language') & ( rhs == 6 ) then
		for k=1:size(dirs,'*');
			mprintf('Completing %s with files transtated into the default language\n',dirs(k));
			complete_with_default_language(dirs(k),directory_language(k),default_language(k));
		end
	end
	
	//------------------------------------------------------------------------------------------
	// build all the whatis 
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
	
// 	//------------------------------------------------------------------------------------------
// 	// now the index
// 	//------------------------------------------------------------------------------------------
// 	
// 	if step=='all' | step == 'index' then 
// 		mprintf('Creating index.htm \n');
// 		if rhs <= 0 then 
// 			gener_index() 
// 		else
// 			gener_index(dirs,titles)
// 		end
// 	end
// 	
// 	//------------------------------------------------------------------------------------------
// 	// now the contents
// 	//------------------------------------------------------------------------------------------
// 	
// 	if step=='all' | step == 'contents' then 
// 		mprintf('Creating contents.htm  \n');
// 		if rhs <= 0 then 
// 			gener_contents() 
// 		else
// 			gener_contents(dirs)
// 		end
// 	end
// 	
// 	//------------------------------------------------------------------------------------------
// 	// now help workshop (Only under Windows and only if
// 	// we build the scilab man (SCI/man/fr and SCI/man/eng )
// 	//------------------------------------------------------------------------------------------
// 	
// 	if MSDOS then
// 		if step=='all' | step == 'hw' then
// 			if (strindex(dirs(1),'\eng\') <> []) | (strindex(dirs(1),'\fr\') <> []) then
// 				mprintf('Creating sciman.hh* \n');
// 				gener_hh(dirs,titles)
// 			end
// 		end
// 	end
	
	//------------------------------------------------------------------------------------------
	// Delete the xml files translated into the default language 
	//------------------------------------------------------------------------------------------
	
	if (step=='all' | step == 'add_default_language') & ( rhs == 6 ) then
		for k=1:size(dirs,'*');
			mprintf('Deleting files transtated into the default language in %s\n',dirs(k));
			del_default_language_xml_files(dirs(k),directory_language(k))
		end
	end
	
endfunction



function gener_whatis(wtitle)

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

    // fix bug 1978
    //  first trim the htm file name, for instance this element of matrix "line":
    //  "<dd><A HREF=""if.htm"">if</A> - conditional execution</dd>"
    //  becomes the following element of matrix "trimmed":
    // "if</A> - conditional execution</dd>"
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

	text = [head;line;"</dl></body></html>"];
	
	mputl(text,"whatis.htm");
	
endfunction



function gener_index(dirs,txt)

	//------------------------------------------------------------------------------------------
	// use %helps to generate an index file 
	//------------------------------------------------------------------------------------------
	
	lines(0);
	
	if MSDOS then
		if ( strindex(%helps(1,1),'\eng\') <> [] ) then
			manpath = pathconvert(getlongpathname(SCI)+'/man/eng/',%t,%f,'w');
		elseif ( strindex(%helps(1,1),'\fr\') <> [] ) then
			manpath = pathconvert(getlongpathname(SCI)+'/man/fr/',%t,%f,'w');
		end
	else
		if ( strindex(%helps(1,1),'/eng/') <> [] ) then
			manpath = pathconvert(getlongpathname(SCI)+'/man/eng/',%t,%f,'u');
		elseif ( strindex(%helps(1,1),'/fr/') <> [] ) then
			manpath = pathconvert(getlongpathname(SCI)+'/man/fr/',%t,%f,'u');
		end
	end
	
	find_links=find_links;
	sep="/";// if MSDOS then sep="\",end
	[lhs,rhs]=argn(0) 
	if rhs <= 0 then 
	        dirs=%helps(:,1)
		txt=%helps(:,2)
	end
  
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
		w=dirs(k)+sep+"whatis.htm";
		if fileinfo(w)==[] then 
			error('file '+w+' not found');
			return 
		end 
		l=l+1;
		w=strsubst(w,'\','/');
		w=strsubst(w,'//','/');
		
		if MSDOS then
			w=pathconvert(getlongpathname(w),%f,%f,'w');
		else
			w=pathconvert(getlongpathname(w),%f,%f,'u');
		end
		
		w = strsubst(w,manpath,'');
		
		line(l)="<dd><A HREF="""+w+""">"+txt(k)+"</A></dd>"		
	end
	line = [line;"		</dl>"
		"	</body>"
		"</html>"]
  
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
		case 'fr' then type_title =  "<H2>Fonctions Scilab</H2>";
		case 'eng' then type_title =  "<H2>Scilab functions</H2>";
	end
  
	full_whatis=["<html>"
		"<head>"
		"	<meta http-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">"
		"	<title>Scilab General Index</title>"
		"</head>"
		"<body bgcolor=""#FFFFFF"">";
		type_title;
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
	directory = pathconvert(directory,%f,%t);
	
	//------------------------------------------------------------------------------------------
	// Nettoyage du répertoire
	// Si il existe un fichier .list_<directory_language> (fichier contenant tous les
	// fichiers traduits dans la langue du répertoire), on supprime dans l'ordre :
	//   1. Tous les fichiers n'appartenant pas à la liste contenue dans .list_<directory_language>
	//   2. Tous les fichiers de la forme .list_<language>
	//------------------------------------------------------------------------------------------
	
	if listfiles(directory+"/.list_"+directory_language) <> [] then
		del_default_language_xml_files(directory,directory_language);
		mdelete(directory+"/.list_*");
	end
	
	//------------------------------------------------------------------------------------------
	// Construction du fichier list_<directory_language> contenant la liste des fichiers traduits
	// dans la langue associée au répertoire
	//------------------------------------------------------------------------------------------
	
	directory_language_xml_files = basename(listfiles(directory+"/*.xml"));
	mputl(directory_language_xml_files,directory+"/.list_"+directory_language);
	
	//------------------------------------------------------------------------------------------
	// Construction du fichier list_<default_language> contenant la liste des fichiers 
	// non traduits dans la langue associée au répertoire qui seront récupérés depuis le 
	// répertoire de la langue par défaut
	//------------------------------------------------------------------------------------------
	
	// Tous les fichiers contenus dans <directory>/../<default_language> pour commencer
	// On afinnera par la suite
	default_language_xml_files = basename(listfiles(directory+"/../"+default_language+"/*.xml"));
	
	// On supprime de "default_language_xml_files" tous les élément contenus dans 
	// "directory_language_xml_files", c'est à dire tous les fichiers déja traduit dans la langue
	// associée au répertoire.
	
	for i=1:size(directory_language_xml_files,'*');
		default_language_xml_files(find(default_language_xml_files==directory_language_xml_files(i)))=[];
	end
	
	// Création du fichier
	mputl(default_language_xml_files,directory+"/.list_"+default_language);
	
	//------------------------------------------------------------------------------------------
	// Copie des fichiers additionnels
	//------------------------------------------------------------------------------------------
	
	for i=1:size(default_language_xml_files,'*');
		tmp_file = mgetl(directory+"/../"+default_language+"/"+default_language_xml_files(i)+".xml");
		mputl(tmp_file,directory+"/"+default_language_xml_files(i)+".xml");
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
	
	if listfiles(directory+"/.list_"+directory_language) <> [] then
		
		// Récupération de la liste des fichiers xml copiés depuis le répertoire de la langue par défaut
		directory_language_xml_files = mgetl(directory+"/.list_"+directory_language);
		
		// Liste de tous les fichiers xml contenu dans le répertoire
		all_files = basename(listfiles(directory+"/*.xml"));
		
		// On retire de "all_files" tous les éléments appartenant à "xml_directory_language_files"
		// Ce sont les aides en ligne traduite dans la langue associée répertoire
		for i=1:size(directory_language_xml_files,'*');
			all_files(find(all_files==directory_language_xml_files(i)))=[];
		end
		
		// Destruction des fichiers restant dans all_files
		for i=1:size(all_files,'*');
			mdelete(directory+"/"+all_files(i)+".xml");
		end
	end
	
endfunction

