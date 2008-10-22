mode(-1);
// Copyright INRIA

// Contents of this file
// - function []=xml2htmlpdf(XMLFILE,dic)
// - function [MATNAME,DESCR,SCINAME,NOTE,MATEX,SCIEX]=get_func_info(data,index)
// - function []=write_html_data(matname,descr,sciname,note,matex,sciex,funnb,dic)
// - function []=write_latex_data(matname,descr,sciname,note,matex,sciex,funnb,dic)
// - function [matname,descr,sciname,note,matex,sciex]=datatohtml(matname,descr,sciname,note,matex,sciex)
// - function [matname,descr,sciname,note,matex,sciex]=datatolatex(matname,descr,sciname,note,matex,sciex)
// - function []=create_html_index(indexfile,names,dic)
// - function [out]=special_name(in)
// - function [entry]=index_entry(in)
// - Some batch instructions

//****************************************************************************************
// function []=xml2htmlpdf(XMLFILE,dic)
// Copyright INRIA
// XMLFILE : XML file containing data to convert
// dic : dictionary we want to build (M2SCI or SCI2M)
//****************************************************************************************
function []=xml2htmlpdf(XMLFILE,dic)
LATEXFILE=dic+".tex"
HTMLFILE="./html/"+dic+".htm"
HTMLINDEXFILE="./html/"+dic+"_index.htm"

// Verify that XML file is well formed
//ierr=execstr("unix_s(""xmllint --noout --valid "+XMLFILE+""")","errcatch")
//if ierr<>0 then
  //mprintf("File : "+XMLFILE+" does not follow DTD");
  //return
//end

// Read data in XML file
[xmlfile,ierr]=mopen(XMLFILE,"r")
if ierr<>0 then
  mprintf("Could not open file : "+XMLFILE);
  return
end
xmldata=mgetl(xmlfile);
mclose(xmlfile);

// Create function table
funtable=[] // line matname sciname
k=1
while k<size(xmldata,1)
  while strindex(xmldata(k),"<MATSCIFUN>")==[] & k<size(xmldata,1)
    k=k+1
  end
  tmp=string(k)
  while strindex(xmldata(k),"<MATNAME>")==[] & k<size(xmldata,1)
    k=k+1
  end
  tmp=[tmp,stripblanks(strsubst(strsubst(xmldata(k),"<MATNAME>",""),"</MATNAME>",""))]
  while strindex(xmldata(k),"<SCINAME>")==[] & k<size(xmldata,1)
    k=k+1
  end
  tmp=[tmp,stripblanks(strsubst(strsubst(xmldata(k),"<SCINAME>",""),"</SCINAME>",""))]
  if k<size(xmldata,1) then
    funtable=[funtable;tmp]
  end
end

// Parameter initialization according to dic 
if dic=="M2SCI" then
  title="Matlab-Scilab equivalent functions"
  leftimage="<B>Matlab</B>"
  rightimage="<IMG SRC=scilab.gif>"
  lefthead="Matlab function"
  righthead="Scilab function"
  
  // Del No equivalent funnames
  k=1
  while k<=size(funtable,1)
    if funtable(k,2)=="No equivalent" then
      functable(k,:)=[]
    else
      funtable(k,4)=special_name(funtable(k,2)) // Name in index
      funtable(k,5)=index_entry(funtable(k,2)) // Entry in index
      k=k+1
    end
  end

  [tmp,indexes]=sort(convstr(funtable(:,2)))
  
  funlines=[];funnames=[];indexnames=[];indexentries=[]
  oplines=[];opnames=[];
  varlines=[];varnames=[];
  for k=1:size(indexes,1)
    if length(funtable(indexes(k),5))==1 then
      funlines=[funlines;funtable(indexes(k),1)] // Line in XML file
      funnames=[funnames;funtable(indexes(k),2)] // Name of function
      indexnames=[indexnames;funtable(indexes(k),4)]  // Name in index
      indexentries=[indexentries;funtable(indexes(k),5)]  // Entry in index
    elseif funtable(indexes(k),5)=="Operators" then
      oplines=[oplines;funtable(indexes(k),1)];
      opnames=[opnames;funtable(indexes(k),2)];
    elseif funtable(indexes(k),5)=="Variables" then
      varlines=[varlines;funtable(indexes(k),1)];
      varnames=[varnames;funtable(indexes(k),2)];
    else
      error("Wrong index entry : "+funtable(indexes(k),5));
    end
  end
  
  // Add operators
  for k=1:size(opnames,1) 
    funlines=[funlines;oplines(k)]
    funnames=[funnames;opnames(k)]
    indexnames=[indexnames;special_name(opnames(k))]
    indexentries=[indexentries;"Operators"]
  end
  
  // Add Variables
  for k=1:size(varnames,1) 
    funlines=[funlines;varlines(k)]
    funnames=[funnames;varnames(k)]
    indexnames=[indexnames;special_name(varnames(k))]
    indexentries=[indexentries;"Variables"]
  end
else
  title="Scilab-Matlab equivalent functions"
  leftimage="<IMG SRC=scilab.gif>"
  rightimage="<B>Matlab</B>"
  lefthead="Scilab function"
  righthead="Matlab function"
  
  // Del No equivalent funnames
  k=1
  while k<=size(funtable,1)
    write(%io(2),funtable(k,3))
    if funtable(k,3)=="No equivalent" then
      funtable(k,:)=[]
    elseif part(stripblanks(funtable(k,3)),1)=="1" then
      funtable(k,:)=[]
    else
      funtable(k,4)=special_name(funtable(k,3)) // Name in index
      funtable(k,5)=index_entry(funtable(k,3)) // Entry in index
      k=k+1
    end
  end

  [tmp,indexes]=sort(convstr(funtable(:,3)))
  
  funlines=[];funnames=[];indexnames=[];indexentries=[]
  oplines=[];opnames=[];
  varlines=[];varnames=[];
  for k=1:size(indexes,1)
    if length(funtable(indexes(k),5))==1 then
      funlines=[funlines;funtable(indexes(k),1)] // Line in XML file
      funnames=[funnames;funtable(indexes(k),3)] // Name of function
      indexnames=[indexnames;funtable(indexes(k),4)]  // Name in index
      indexentries=[indexentries;funtable(indexes(k),5)]  // Entry in index
    elseif funtable(indexes(k),5)=="Operators" then
      oplines=[oplines;funtable(indexes(k),1)];
      opnames=[opnames;funtable(indexes(k),3)];
    elseif funtable(indexes(k),5)=="Variables" then
      varlines=[varlines;funtable(indexes(k),1)];
      varnames=[varnames;funtable(indexes(k),3)];
    else
      error("Wrong index entry : "+funtable(indexes(k),5));
    end
  end
  
  // Add operators
  for k=1:size(opnames,1) 
    funlines=[funlines;oplines(k)]
    funnames=[funnames;opnames(k)]
    indexnames=[indexnames;special_name(opnames(k))]
    indexentries=[indexentries;"Operators"]
  end
  
  // Add Variables
  for k=1:size(varnames,1) 
    funlines=[funlines;varlines(k)]
    funnames=[funnames;varnames(k)]
    indexnames=[indexnames;special_name(varnames(k))]
    indexentries=[indexentries;"Variables"]
  end
end

if dic=="M2SCI" then
  ref="Matlab"
  eq="Scilab"
else
  eq="Matlab"
  ref="Scilab"
end

LATEX_FORWORD=[
    "This document contains a list of some "+ref+" basis functions and compare them ";
    "with the "+eq+" equivalent functions if there are. It gives the main differences ";
    "and gives examples of particular cases.\newline";
    "\newline"
    "However not all "+ref+" and "+eq+" functions are described here.\newline";
    "\newline"
    "Equivalents for operators and variables can be found at the end of this document.\newline"
    "\newline"
    "Caution:\newline"
    "This document is not dedicated to explain how functions work. ";
    "Users can used online manuals of "+ref+" and "+eq+" for it.\newline"
    "\newline"
    ]
HTML_FOREWORD=[
    LATEX_FORWORD;
    "How to use this HTML guide:\newline";
    "To have more details about functions and examples, clic on the ""triangles"" just ";
    "at the left of "+ref+" function name.\newline";
    "To have a more detailed index, clic on the letters and keywords.\newline"
    ]

// Write HTML file beginning
HTML_FILE_BEG=[
    "<!-- FILE GENERATED BY SCILAB - PLEASE DO NOT EDIT -->"
    "<HTML>"
    "<HEAD>"
    "  <TITLE>"+title+"</title>"
    "  <META HTTP-EQUIV=""Content-Type"" CONTENT=""text/html; charset=iso-8859-1"">"
    "  <SCRIPT SRC=""matsci.js""></SCRIPT>"
    "</HEAD>"
    "<BODY BACKGROUND=""back.jpg"">"
    "<P ALIGN=""center"">"
    "<FONT FACE=""arial"">"
    ]
[htmlfile,ierr]=mopen(HTMLFILE,"w")
if ierr<>0 then
  mprintf("Could not open file : "+HTMLFILE);
  return
end
mputl([HTML_FILE_BEG;
    "<A NAME=""Foreword""></A>"
    "<TABLE WIDTH=""80%"">";   
    "  <TR WIDTH=""100%"">"
    "    <TD COLSPAN=""4"" ALIGN=""center"">"
    "      <FONT SIZE=""+2"" COLOR=""red"">Foreword</FONT>"
    "      <HR WIDTH=""100%"">"
    "    </TD>"
    "  </TR>"
    "  <TR WIDTH=""100%"">"
    "    <TD COLSPAN=""4"">"
    strsubst(HTML_FOREWORD,"\newline","<BR>")
    "    </TD>"
    "  </TR>"
    "</TABLE>"
    ],htmlfile)
mclose(htmlfile)

// Write LATEX file beginning
LATEX_FILE_BEG=[
    "% FILE GENERATED BY SCILAB - PLEASE DO NOT EDIT %"
    "\documentclass[12pt]{report}"
    "\usepackage[a4paper,text={170mm,250mm},pdftex,twoside]{geometry}"
    "\usepackage{fancyheadings}"
    "\lhead{"+lefthead+"}"
    "\rhead{"+righthead+"}"
    "\cfoot{DRAFT - Page {\thepage}}"
    "\addtolength{\parskip}{5mm}"
    "\usepackage{makeidx}"
    "\makeindex"
    "\usepackage{amssymb}"
    "\usepackage{hyperref}"
    "\begin{document}"
    "\pagestyle{empty}"
    "\begin{center}"
    "\vspace*{9cm}"
    "\begin{minipage}[c]{\linewidth}"
    "\begin{center}"
    "\textbf{\Large{"+dic+":\\"+title+"\\}}"
    "Version 1.0"
    "\end{center}"
    "\begin{center}"
    "Scilab Project\\"
    "INRIA Rocquencourt - BP 105\\"
    "78153 Le Chesnay Cedex -  France\\"
    "Email: \texttt{scilab@inria.fr}\\"
    "2004"
    "\end{center}"
    "\end{minipage}"
    "\end{center}"
    "\cleardoublepage"
    "\newpage"
    "\pdfbookmark[0]{Foreword}{Forword}"
    "\begin{center}"
    "\textbf{Forword}"
    "\end{center}"
    LATEX_FORWORD
    "\cleardoublepage"
    "\printindex"
    "\cleardoublepage"
    "\pagestyle{fancy}"
    ]
[latexfile,ierr]=mopen(LATEXFILE,"w")
if ierr<>0 then
  mprintf("Could not open file : "+LATEXFILE);
  return
end
mputl(LATEX_FILE_BEG,latexfile)
mclose(latexfile)

// Write all function informations
indentry_sav=""
for l=1:size(funnames,1)
  
  // Get infos from xml data
  [matname,descr,sciname,note,matex,sciex]=get_func_info(xmldata,evstr(funlines(l)))
  
  // Add indexes to files (HTML and TEX)
  indentry=indexentries(l)
  if indentry<>indentry_sav then

    // Index HTML file (A, B, ...)
    HTML_DATA=[
	"<A NAME="""+indentry+"""></A>";
	"<TABLE WIDTH=""80%"">";
	"  <TR WIDTH=""100%"">"
	"    <TD COLSPAN=""4"">"
	"    <HR WIDTH=""100%"">"
	"    </TD>"
	"  </TR>"
	"  <TR WIDTH=""100%"">";
	"    <TD HEIGHT=""20"" WIDTH=""100%"" COLSPAN=""4"">";
	"    </TD>";
	"  </TR>";
	"  <TR WIDTH=""100%"">";
	"    <TD WIDTH=""5%"" ALIGN=""center""></TD>";
	"    <TD WIDTH=""10%"" ALIGN=""center"">"+leftimage+"</TD>";
	"    <TD WIDTH=""70%"" ALIGN=""center""><FONT SIZE=""+2"" COLOR=""red"">"+indentry+"</FONT></TD>";
	"    <TD WIDTH=""10%"" ALIGN=""center"">"+rightimage+"</TD>";
	"  </TR>";
	"</TABLE>"]
    
    [htmlfile,ierr]=mopen(HTMLFILE,"a+")
    if ierr<>0 then
      mprintf("Could not open file : "+HTMLFILE);
      return
    end
    mputl(HTML_DATA,htmlfile)
    mclose(htmlfile)
    
    // Index LATEX file (A, B, ...)
    LATEX_DATA=[
	"\newpage"
	"\pdfbookmark[0]{"+indentry+"}{"+indentry+"}"
	"\begin{center}\textbf{"+indentry+"}\end{center}"
	]
    [latexfile,ierr]=mopen(LATEXFILE,"a+")
    if ierr<>0 then
      mprintf("Could not open file : "+LATEXFILE);
      return
    end
    mputl(LATEX_DATA,latexfile)
    mclose(latexfile)
    
    indentry_sav=indentry
  end
  
  // Write data according to data read
  write_html_data(matname,descr,sciname,note,matex,sciex,l,dic);
  write_latex_data(matname,descr,sciname,note,matex,sciex,l,dic,indexnames(l),indexentries(l));
end

// Write HTML file end
HTML_FILE_END=[
    "</P>"
    "</FONT>"
    "</BODY>"
    "</HTML>"
    ]
[htmlfile,ierr]=mopen(HTMLFILE,"a+")
if ierr<>0 then
  mprintf("Could not open file : "+HTMLFILE);
  return
end
mputl(["<HR WIDTH=""80%"">";HTML_FILE_END],htmlfile)
mclose(htmlfile)

// Write LATEX file end
LATEX_FILE_END=[
    "\end{document}"
    ]
[latexfile,ierr]=mopen(LATEXFILE,"a+")
if ierr<>0 then
  mprintf("Could not open file : "+LATEXFILE);
  return
end
mputl(LATEX_FILE_END,latexfile)
mclose(latexfile)

// Create HTML index file (done automatically for TEX)
create_html_index(HTMLINDEXFILE,funnames,dic,indexentries)
endfunction


//****************************************************************************************
//function [MATNAME,DESCR,SCINAME,NOTE,MATEX,SCIEX]=get_func_info(data,index)
// Copyright INRIA
// data : XML data
// index : position of function in data
//****************************************************************************************
function [MATNAME,DESCR,SCINAME,NOTE,MATEX,SCIEX]=get_func_info(data,index)
index=index+1
MATNAME=stripblanks(strsubst(strsubst(data(index),"<MATNAME>",""),"</MATNAME>",""))
index=index+1
SCINAME=stripblanks(strsubst(strsubst(data(index),"<SCINAME>",""),"</SCINAME>",""))
index=index+1
DESCR=stripblanks(strsubst(strsubst(data(index),"<DESCR>",""),"</DESCR>",""))
index=index+1
disp(data(index-3))
if strindex(data(index),"</MATSCIFUN>")==[] then
  index=index+1
  NOTE=""
  while strindex(data(index),"</NOTE>")==[]
    NOTE=NOTE+stripblanks(data(index))
    index=index+1
  end
  index=index+1
  if strindex(data(index),"</MATSCIFUN>")==[] then
    
    if strindex(data(index),"<MATEX>")<>[] then 
      index=index+1
      MATEX=[]
      while strindex(data(index),"</MATEX>")==[] then
	MATEX=[MATEX;stripblanks(data(index))]
	index=index+1
      end
      index=index+1
    end
    
    if strindex(data(index),"<SCIEX>")<>[] then 
      index=index+1
      SCIEX=[]
      while strindex(data(index),"</SCIEX>")==[] then
	SCIEX=[SCIEX;stripblanks(data(index))]
	index=index+1
      end
    end      
  else
    MATEX=""
    SCIEX=""
  end
else
  NOTE=""
  MATEX=""
  SCIEX=""
end
endfunction

//****************************************************************************************
//function []=write_html_data(matname,descr,sciname,note,matex,sciex,funnb,dic)
// Copyright INRIA
//****************************************************************************************
function []=write_html_data(matname,descr,sciname,note,matex,sciex,funnb,dic)
[matname,descr,sciname,note,matex,sciex]=datatohtml(matname,descr,sciname,note,matex,sciex)

if dic=="M2SCI" then
  leftname=matname
  rightname=sciname
  leftex=matex
  rightex=sciex
else
  leftname=sciname
  rightname=matname
  leftex=sciex
  rightex=matex
end  

[htmlfile,ierr]=mopen(HTMLFILE,"a+")
if ierr<>0 then
  mprintf("Could not open file : "+HTMLFILE);
  return
end

// Prepare data to write in HTML file
ind_name=strsubst(special_name(leftname)," ","-")
ind_name=strsubst(ind_name,"&#8596;","")

HTML_DATA = [
    "<!---------- "+special_name(leftname)+" ---------->"
    "<A NAME="""+ind_name+"""></A>";
    "<TABLE CELLPADING=""0"" CELLSPACING=""0"" WIDTH=""80%"">";
    "  <TR>"
    "    <TD>"
    "    <HR WIDTH=""100%"">"
    "    </TD>"
    "  </TR>"
    "  <TR WIDTH=""100%""> "
    "    <TD>"
    "      <TABLE WIDTH=""100%"" CELLSPACING=""0"">"
    "        <TR>"
    "          <TD WIDTH=""5%"" ALIGN=""right"" VALIGN=""top"">"
    ]
if or(leftex<>"") | or(rightex<>"") | or(note<>"") then
  HTML_DATA=[
      HTML_DATA;
      "            <A HREF=""javascript:swap_couche(''"+string(funnb)+"'');"">"
      "              <IMG NAME=""arrow"+string(funnb)+""" SRC=""rightarrow.gif?1"" BORDER=""0"">"
      "            </A>"
      ]
end
HTML_DATA=[
    HTML_DATA;
    "          </TD>"
    "  	       <TD WIDTH=""10%"" ALIGN=""center""><B>"+leftname+"</B></TD>"
    "          <TD WIDTH=""75%"" ALIGN=""center""><I>"+descr+"</I></TD>"
    "	       <TD WIDTH=""10%"" ALIGN=""center""><B>"+rightname+"</B></TD>"
    "        </TR>"
    "      </TABLE>"
    "    </TD>"
    "  </TR>"]
if or(leftex<>"") | or(rightex<>"") | or(note<>"") then
  HTML_DATA=[
      HTML_DATA;
      "  <SCRIPT LANGUAGE=''JavaScript''>"
      "    <!--	"
      "      vis[''"+string(funnb)+"''] = ''hide'';"
      "    //-->"
      "  </SCRIPT>"
      "  <TR WIDTH=""100%"">"
      "    <TD>"
      "      <DIV ID=''Layer"+string(funnb)+"'' STYLE=''display: none;''>"
      "        <TABLE WIDTH=""100%"" CELLSPACING=""0"" BORDER=""1"" BORDERCOLOR=""silver"">"
      ]
  if or(note<>"") then
    HTML_DATA=[
	HTML_DATA
	"          <TR>"
	"            <TD COLSPAN=""2"">"+note+"</TD>"
	"          </TR>"
	]
  end
  if or(rightex<>"") | or(leftex<>"") then
    if leftex=="" then leftex="None";end
    if rightex=="" then rightex="None";end
    HTML_DATA=[
	HTML_DATA
	"          <TR>"
	"            <TD VALIGN=""top"" WIDTH=""50%""><TT>"+leftex+"</TT></td>"
	"            <TD VALIGN=""top"" WIDTH=""50%""><TT>"+rightex+"</TT></td>"
	"          </TR>"
	]
    
  end
  HTML_DATA=[
      HTML_DATA
      "        </TABLE>"
      "        <BR>"
      "      </DIV>"
      "    </TD>"
      "  </TR>"
      ]
end

HTML_DATA=[
    HTML_DATA
    "</TABLE>"
    ]

mputl(HTML_DATA,htmlfile)
mclose(htmlfile)
endfunction

//****************************************************************************************
// function [matname,descr,sciname,note,matex,sciex]=datatohtml(matname,descr,sciname,note,matex,sciex)
// Copyright INRIA
// Convert XML data to HTML
//****************************************************************************************
function [matname,descr,sciname,note,matex,sciex]=datatohtml(matname,descr,sciname,note,matex,sciex)

// Syntax modification
matname=strsubst(matname,"<MATH>","")
matname=strsubst(matname,"</MATH>","")
matname=strsubst(matname,"|-|","&#8596;")

sciname=strsubst(sciname,"<MATH>","")
sciname=strsubst(sciname,"</MATH>","")
sciname=strsubst(sciname,"|-|","&#8596;")

note=strsubst(note,"<P>","")
note=strsubst(note,"</P>","<BR>")
note=strsubst(note,"<B>","<B>")
note=strsubst(note,"<MATH>","")
note=strsubst(note,"</MATH>","")
note=strsubst(note,"|-|","&#8596;")

note=strsubst(note,"</NOTE_ITEM>","")
kitem=strindex(note,"<NOTE_ITEM")
kitem=[kitem,length(note)+1]
klabel=strindex(note,"LABEL")
kend=strindex(note,""">")
if kitem<>[] then
  tmp=part(note,1:kitem(1)-1)
  for m=1:size(kitem,"*")-1
    label=strsubst(part(note,klabel(m)+7:kend(m)-1),"|-|","&#8596;")
    tmp=tmp+"&#9657; "+label+":<BR>"+part(note,kend(m)+2:kitem(m+1)-1)+"<BR>"
  end
  note=stripblanks(tmp)
end

// Remove useless newline
if max(strindex(note,"<BR>"))==length(note)-3 then
  note=part(note,1:(length(note)-4))
end

if matex<>"" then
  tmp="<TT>"
  for m=1:size(matex,1)
    matex(m)=strsubst(matex(m),"<MATH>","")
    matex(m)=strsubst(matex(m),"</MATH>","")
    tmp=tmp+matex(m)+"<BR>"
  end
  tmp=tmp+"</TT>"
  matex=tmp
end

if sciex<>"" then
  tmp="<TT>"
  for m=1:size(sciex,1)
    tmp=tmp+sciex(m)+"<BR>"
  end
  tmp=tmp+"</TT>"
  sciex=tmp
end

rhs=argn(2)
if rhs<3 then
  note=""
end
if rhs<4 then
  matex=""
end
if rhs<5 then
  sciex=""
end

endfunction

//****************************************************************************************
//function []=write_latex_data(matname,descr,sciname,note,matex,sciex,funnb,dic)
// Copyright INRIA
//****************************************************************************************
function []=write_latex_data(matname,descr,sciname,note,matex,sciex,funnb,dic,index_name,index_entry)
[matname,descr,sciname,note,matex,sciex]=datatolatex(matname,descr,sciname,note,matex,sciex)

if dic=="M2SCI" then
  leftname=matname
  rightname=sciname
  leftex=matex
  rightex=sciex
else
  leftname=sciname
  rightname=matname
  leftex=sciex
  rightex=matex
end  

[latexfile,ierr]=mopen(LATEXFILE,"a+")
if ierr<>0 then
  mprintf("Could not open file : "+LATEXFILE);
  return
end

// Prepare data to write in LATEX file
ind_name=strsubst(special_name(leftname)," ","-")
ind_name=strsubst(ind_name,"$\Leftrightarrow$","")

LATEX_DATA = [
    "%---------- "+ind_name+" ----------"
    "\pdfbookmark[1]{"+leftname+"}{"+index_name+"}"
    ]

if index_entry=="Operators" then
  LATEX_DATA = [
      LATEX_DATA;
      "\index{"+index_entry+"!"+index_name+"\textsf{("+leftname+")}}"
      ]
elseif  index_entry=="Variables" & index_name=="Last index" then
  LATEX_DATA = [
      LATEX_DATA;
      "\index{"+index_entry+"!"+index_name+"\textsf{(end)}}"
      ]
else
  LATEX_DATA = [
      LATEX_DATA;
      "\index{"+index_entry+"!"+index_name+"}"
      ]
end

LATEX_DATA = [
    LATEX_DATA;
    "\begin{center}"
    "\begin{tabular}{|p{4.5cm}|p{3.25cm}p{3.25cm}|p{4.5cm}|}"
    "\hline\multicolumn{1}{|p{4.5cm}}{\centering\textbf{"+leftname+"}} & "
    "\multicolumn{2}{|p{6.5cm}|}{\centering\textit{"+descr+"}} & "
    "\multicolumn{1}{p{4.5cm}|}{\centering\textbf{"+rightname+"}} \\ \hline"
    ]
if note<>"" then
  LATEX_DATA=[
      LATEX_DATA
      "\multicolumn{4}{|p{16.5cm}|}{"+note+"} \\ \hline"
      ]
end

if or(rightex<>"") | or(leftex<>"") then
  if leftex=="" then leftex="None";end
  if rightex=="" then rightex="None";end
  LATEX_DATA=[
      LATEX_DATA
      "\multicolumn{2}{|p{7.75cm}|}{"+leftex+"} & "
      "\multicolumn{2}{p{7.75cm}|}{"+rightex+"} \\ \hline"
      ]
end

LATEX_DATA=[
    LATEX_DATA
    "\end{tabular}"
    "\end{center}"
    ]

mputl(LATEX_DATA,latexfile)
mclose(latexfile)
endfunction

//****************************************************************************************
// function [matname,descr,sciname,note,matex,sciex]=datatolatex(matname,descr,sciname,note,matex,sciex)
// Copyright INRIA
// Convert XML data to LATEX
//****************************************************************************************
function [matname,descr,sciname,note,matex,sciex]=datatolatex(matname,descr,sciname,note,matex,sciex)

matname=strsubst(matname,"\","\textbackslash");
matname=strsubst(matname,"<MATH>^</MATH>","\textasciicircum")
matname=strsubst(matname,"%","\%")
matname=strsubst(matname,"$","\$")
matname=strsubst(matname,"&amp;","\&")
matname=strsubst(matname,"&lt;","<")
matname=strsubst(matname,"&gt;",">")
matname=strsubst(matname,"<MATH>","$")
matname=strsubst(matname,"</MATH>","$")
matname=strsubst(matname,"|-|","$\Leftrightarrow$")
matname=strsubst(matname,"_","\_")

sciname=strsubst(sciname,"\","\textbackslash");
sciname=strsubst(sciname,"<MATH>^</MATH>","\textasciicircum")
sciname=strsubst(sciname,"%","\%")
sciname=strsubst(sciname,"$","\$")
sciname=strsubst(sciname,"&amp;","\&")
sciname=strsubst(sciname,"&lt;","<")
sciname=strsubst(sciname,"&gt;",">")
sciname=strsubst(sciname,"<MATH>","$")
sciname=strsubst(sciname,"</MATH>","$")
sciname=strsubst(sciname,"|-|","$\Leftrightarrow$")
sciname=strsubst(sciname,"_","\_")

note=strsubst(note,"\","\textbackslash")
note=strsubst(note,"$","\$")
note=strsubst(note,"<P>","")
note=strsubst(note,"</P>","\newline ")
note=strsubst(note,"<B>","\textbf{")
note=strsubst(note,"</B>","}")
note=strsubst(note,"<MATH>^</MATH>","\textasciicircum")
note=strsubst(note,"<MATH>","$")
note=strsubst(note,"</MATH>","$")
note=strsubst(note,"</NOTE_ITEM>","")
note=strsubst(note,"&lt;","<")
note=strsubst(note,"&gt;",">")
note=strsubst(note,"%","\%")
note=strsubst(note,"|-|","$\Leftrightarrow$")

kitem=strindex(note,"<NOTE_ITEM")
kitem=[kitem,length(note)+1]
klabel=strindex(note,"LABEL")
kend=strindex(note,""">")
if kitem<>[] then
  tmp=part(note,1:kitem(1)-1)
  for m=1:size(kitem,"*")-1
    label=strsubst(part(note,klabel(m)+7:kend(m)-1),"|-|","$\Leftrightarrow$")
    tmp=tmp+"$\triangleright$ "+label+":\newline "+part(note,kend(m)+2:kitem(m+1)-1)+"\newline "
  end
  note=tmp
  note=stripblanks(strsubst(note,"_","\_"))
end

// Remove useless newline
if max(strindex(note,"\newline"))==length(note)-7 then
  note=part(note,1:(length(note)-8))
end

if or(matex<>"") then
  tmp="\texttt{"
  for m=1:size(matex,1)
    matex(m)=strsubst(matex(m),"&amp;","\&")
    matex(m)=strsubst(matex(m),"&lt;","<")
    matex(m)=strsubst(matex(m),"&gt;",">")
    matex(m)=strsubst(matex(m),"^","\textasciicircum")
    matex(m)=strsubst(matex(m),"_","\_")
    matex(m)=strsubst(matex(m),"$","\$")
    matex(m)=strsubst(matex(m),"<MATH>","$")
    matex(m)=strsubst(matex(m),"</MATH>","$")
    tmp=tmp+matex(m)
    if m<>size(matex,1) then
      tmp=tmp+"\newline "
    end
  end
  tmp=tmp+"}"
  matex=tmp
end

if or(sciex<>"") then
  tmp="\texttt{"
  for m=1:size(sciex,1)
    sciex(m)=strsubst(sciex(m),"&amp;","\&")
    sciex(m)=strsubst(sciex(m),"&lt;","<")
    sciex(m)=strsubst(sciex(m),"&gt;",">")
    sciex(m)=strsubst(sciex(m),"^","\textasciicircum")
    sciex(m)=strsubst(sciex(m),"_","\_")
    sciex(m)=strsubst(sciex(m),"%","\%")
    sciex(m)=strsubst(sciex(m),"$","\$")
    sciex(m)=strsubst(sciex(m),"<MATH>","$")
    sciex(m)=strsubst(sciex(m),"</MATH>","$")
    tmp=tmp+sciex(m)
    if m<>size(sciex,1) then
      tmp=tmp+"\newline "
    end
  end
  tmp=tmp+"}"
  sciex=tmp
end

rhs=argn(2)
if rhs<3 then
  note=""
end
if rhs<4 then
  matex=""
end
if rhs<5 then
  sciex=""
end

endfunction

//****************************************************************************************
// function []=create_html_index(indexfile,names,dic)
// Copyright INRIA
// - indexfile: name of file to create
// - names: function names
// - dic: dic to create
//****************************************************************************************
function []=create_html_index(indexfile,names,dic,indentries)

htmlfile=mopen(indexfile,"w")

mputl(HTML_FILE_BEG,htmlfile)

firstlettersav=""
for kname=1:size(names,"*")
  
  firstletter=indentries(kname)
  if length(firstletter)==1 then
    couche=string(ascii(firstletter))
  elseif firstletter=="Operators" then
    couche=string(1000)
  else
    couche=string(1001)
  end
  if firstletter<>firstlettersav then
    firstlettersav=firstletter
    if kname<>1 then
      HTML_DATA=[
	  "      </DIV>"
	  "    </TD>"
	  "  </TR>"
	  "</TABLE>"
	  ]
    else
      if dic=="M2SCI" then
	HTML_DATA="<A HREF=""SCI2M_doc.htm"" TARGET=""_top"">Go to SCI2M</A><BR>"
      else
	HTML_DATA="<A HREF=""M2SCI_doc.htm"" TARGET=""_top"">Go to M2SCI</A><BR>"
      end
      HTML_DATA=[
	  HTML_DATA
	  "<BR><A HREF="""+dic+".htm#Foreword"" TARGET=""dic"">Foreword</A><BR><BR>"
	  ]
    end
    HTML_DATA = [
	HTML_DATA;
	"<TABLE WIDTH=""70"">"
	"  <TR>"
	"    <TD>"
	"      <A HREF=""javascript:void(0)"" onClick=""javascript:swap_couche(''"+couche+"''); parent.dic.document.location.href = ''"+dic+".htm#"+firstletter+"''"">"+firstletter+"</A><BR>"
	"    </TD>"
	"  </TR>"
	"  <SCRIPT language=''JavaScript''>"
	"    <!--	"
	"      vis[''"+couche+"''] = ''hide'';"
	"    //-->"
	"  </SCRIPT>"
	"  <TR>"
	"    <TD>"
	"      <DIV ID=''Layer"+couche+"'' STYLE=''display: none;''>"
	]
  mputl(HTML_DATA,htmlfile)
  end

  ind_name=strsubst(special_name(names(kname))," ","-")
  ind_name=strsubst(ind_name,"|-|","")

  HTML_DATA=[
      "<A HREF="""+dic+".htm#"+ind_name+""" TARGET=""dic"">"+strsubst(names(kname)," |-| ","--")+"</A><BR>"]
  mputl(HTML_DATA,htmlfile)
  
end
HTML_DATA=[
    "      </DIV>"
    "    </TD>"
    "  </TR>"
    "</TABLE>"
    HTML_FILE_END]

mputl(HTML_DATA,htmlfile)
mclose(htmlfile);
endfunction

//****************************************************************************************
// function [out]=special_name(in)
// Copyright INRIA
// Deals with operator names
//****************************************************************************************
function [out]=special_name(in)
if in=="+" then
  out="plus"
elseif in=="-" then
  out="minus"
elseif in==".*." then
  out="kronecker product"
elseif in=="./." then
  out="kronecker right division"
elseif in==".\." then
  out="kronecker letf division"
elseif in=="/" then
  out="right division"
elseif in=="./" then
  out="elementwise right division"
elseif in==".\" | in==".\textbackslash" then
  out="elementwise left division"
elseif in=="<MATH>^</MATH>" | in=="^" | in=="\textasciicircum" then
  out="exponent"
elseif in==".<MATH>^</MATH>" | in==".^" | in==".\textasciicircum" then
  out="elementwise exponent"
elseif in=="*" then
  out="multiplication"
elseif in==".*" then
  out="elementwise multiplication"
elseif in=="<MATH>&gt;</MATH>" | in=="$>$" | in=="&gt;" then
  out="greater than"
elseif in=="<MATH>&lt;</MATH>" | in=="$<$" | in=="&lt;"then
  out="smaller than"
elseif in=="<MATH>&gt;</MATH>=" | in=="$>$=" | in=="&gt;=" then
  out="greater or equal to"
elseif in=="<MATH>&lt;</MATH>=" | in=="$<$=" | in=="&lt;=" then
  out="smaller or equal to"
elseif in=="==" then
  out="equal to"
elseif in=="~=" then
  out="not equal to"
elseif in=="''" then
  out="transpose"
elseif in==".''" then
  out="elementwise transpose"
elseif in=="&amp;" | in=="\&" then
  out="logical AND"
elseif in=="<MATH>|</MATH>" | in=="|" | in=="$|$" then
  out="logical OR"
elseif in=="\" | in=="\textbackslash" then
  out="left division"
elseif in==":" then
  out="colon"
elseif in=="~" then
  out="negation"
elseif in=="\$" | in=="$" | in=="end (index)" then
  out="Last index"
elseif in=="~MSDOS" then
  out="isunix"
else
  out=strsubst(in,"\_","");
  out=strsubst(out,"_","");
  out=strsubst(out,"%","");
  out=strsubst(out,"$","");
end
endfunction

//****************************************************************************************
// function [entry]=index_entry(in)
// Copyright INRIA
// Finds index entries
//****************************************************************************************
function [entry]=index_entry(in)
if dic=="M2SCI" & or(in==["i","j","eps","ans","pi","end (index)"]) then
  entry="Variables"
elseif part(in,1)=="%" | part(in,2)=="%" | in=="$" | in=="\$" | ~isempty(strindex(in,"MSDOS")) then
  entry="Variables"
elseif special_name(in)<>in & isempty(strindex(in,"_")) then
  entry="Operators"
else
  entry=convstr(part(in,1),"u")
end
endfunction

//****************************************************************************************
// Batch instructions
//****************************************************************************************
disp("M2SCI");
xml2htmlpdf("matscifuns.xml","M2SCI")
disp("SCI2M");
xml2htmlpdf("matscifuns.xml","SCI2M")
disp("Doc done !")
