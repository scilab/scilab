function xmlfiletohtml(path,xsl)

	// Copyright Enpc (Jean-Philippe Chancelier)
	//given a path on a Scilab help xml file  (assumed to respect 
	//SCI/man/man-rev.dtd ) this function generates the corresponding htm
	//file using the /man/<LANGUAGE>/html-rev.xsl xsl file
	
	generate_cmd='xsltproc';
	
	[lhs,rhs]=argn(0);
	if rhs < 2 then xsl= 'html-rev.xsl';end //the xsl file name;
	global LANGUAGE %helps
	path=pathconvert(path,%f,%t) // convert path to host convention
  
	//proceed if xml file is newest than htm file
	if newest(path,strsubst(path,".xml",".htm"))==1 then
	
		mprintf('  Processing file %s.xml\n',basename(path));

		// build .xml2 file where LINK tags references are solved
		find_links(path,path+"2")
		
		// If the sources we are building are extracted from SVN with
		// the "svn export" command, we can use the "last modification
		// date" of the xml file, if not ( ie sources extracted with the
		// "svn checkout" command ), it doesn't mean anything to use
		// this system.
		
		if ~(isdir(strsubst(path,basename(path)+'.xml','.svn'))) then
			if (basename(xsl) == 'html-rev') then
				xsl = strsubst(xsl,'html-rev','html-bin');
			end
			update_date(path,path+"2")
		end
		
		in=path+"2"
		out=strsubst(path,'.xml','.htm')
		
		// form the html generator command line instruction
		if  MSDOS then 
			// sabcmd does not like c:/.. path replace it by file://c:/..
			xsl='file://'+pathconvert(SCI+'/man/'+LANGUAGE)+xsl;
			generate_cmd='""'+WSCI+'\Win-util\sablotron\sabcmd'+'""'
			if basename(in)+".xml2"<>in then
				in='file://'+in;
			end
			if basename(out)+".xml2"<>out then
				out='file://'+out;
			end
			instr=generate_cmd+' '+xsl+' '+in+' '+out
			RM='del /s '
		else
			xsl=pathconvert(SCI+'/man/'+LANGUAGE)+xsl;
			if generate_cmd=='xsltproc' then 
				instr=generate_cmd+' -o '+out+' '+xsl+' '+in
			else
				instr=generate_cmd+' '+xsl+' '+in+' '+out
			end
			RM='rm -f '
		end

		//run html generator
		if execstr('unix_s('+sci2exp(instr)+')','errcatch')<>0 then 
			write(%io(2),'     Warning '+path+' does not follow the dtd','(a)')
		end
		
		unix_s(RM+path+"2")
	end
endfunction

function update_date(xmlfile,xmlfile2)
	  
	//-------------------------------------
	// Author : Pierre MARECHAL
	// Scilab Team
	// Copyright INRIA
	// Date : 09/05/2005
	//-------------------------------------
	
	//------------------------------------- 
	// Add the date of the last modification of the xml file
	//--------------------------------------
	
	txt=mgetl(xmlfile2);
	d=grep(txt,"<DATE>");
	if d==[] then mputl(txt,xmlfile2); return; end
	[x,ierr]=fileinfo(xmlfile);
	if x(6)<1064550000 then mputl(txt,xmlfile2); return; end
	modification_date = getdate(x(6));
	txt(d)="<DATE>"+msprintf('%02d',modification_date(6))+"/"+msprintf('%02d',modification_date(2))+"/"+msprintf('%04d',modification_date(1))+"</DATE>";
	mputl(txt,xmlfile2);
	
 endfunction
