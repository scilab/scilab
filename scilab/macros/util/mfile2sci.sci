function res=mfile2sci(fil,res_path,Recmode,only_double,verbose_mode,prettyprint)
// Copyright INRIA
// This function performs translation of a single M-file
// - fil: file name
// - res_path: path to write translated file in (default value is fil path)
// - Recmode: recursive mode (default value is false)
// Authors: S.S. and V.C.

// Get default arguments
[lhs,rhs]=argn(0)
if rhs<6 then prettyprint=%F,end
if rhs<5 then verbose_mode=3,end
if rhs<4 then only_double=%F,end
if rhs<3 then Recmode=%F,end
if rhs<2 then res_path="./",end
if rhs<1 then res=m2sci_gui();return;end
if MSDOS then
  fil=strsubst(fil,"\","/")
  res_path=strsubst(res_path,"\","/")
end
if part(res_path,length(res_path))<>"/" then 
  res_path=res_path+"/"
end
// Loads libraries related to m2sci
if exists("m2skernellib")==0 then load("SCI/macros/m2sci/kernel/lib"),end
if exists("m2spercentlib")==0 then load("SCI/macros/m2sci/percent/lib"),end
if exists("m2ssci_fileslib")==0 then load("SCI/macros/m2sci/sci_files/lib"),end

if multi_fun_file(fil,res_path,Recmode,only_double,verbose_mode,prettyprint) then
  res=1
  return
end

// Get context
global("m2sci_infos")
global("mtlbref_fun")
global("mtlbtool_fun")
global("not_mtlb_fun")
[l,mac]=where()
Reclevel=size(find(mac=="mfile2sci"),"*")
tpcallpos=min(find(mac=="translatepaths"));
guicallpos=min(find(mac=="m2sci_gui"));
if size(find(mac=="m2sci_gui"),"*")==1 & tpcallpos<guicallpos then // Bug 679
  Reclevel=Reclevel-1
end
if size(find(mac=="multi_fun_file"),"*")==1 then
  Reclevel=Reclevel-1
end

if Reclevel==1 then
  nametbl=[]
else
  m2sci_infos_save=m2sci_infos
end
m2sci_infos=[%f %f]

margin=part(" ",ones(1,3*(Reclevel-1)))
margin="  "
rec="OFF"
dble="NO"
pretty="NO"
if prettyprint then pretty="YES";end
if Recmode then rec="ON";end
if only_double then dble="YES";end

res=[]

// Handle file path
// File name
k=strindex(fil,".")
if k<>[]
  ke=k($)-1
  base_name=part(fil,1:ke)
else
  ke=length(fil)
  base_name=fil
end
// File path
k=strindex(fil,"/")
if k==[] then
  file_path="./"
else
  file_path=part(fil,1:k($))
end
// Others M-files in directory
if exists("Paths")==0 then
  Paths=file_path,
  if MSDOS then
    Paths=strsubst(Paths,"/","\")
    mfiles=listfiles(Paths+"*.m")
    sep="\"
  else
    mfiles=listfiles(Paths+"*.m")
    sep="/"
  end
end

// Function name
fnam=part(base_name,k($)+1:ke) // File name without extension

// logfile initialisation
if exists("logfile")==0 then
  [tempfd1,ierr1]=file('open',pathconvert(TMPDIR)+"logfile.dat","old")
  if ierr1==0 then
    load(pathconvert(TMPDIR)+"logfile.dat")
    file('close',tempfd1)
    file('close',logfile)
    mdelete(pathconvert(TMPDIR)+"logfile.dat")
  end
  logfile=file('open',res_path+"m2sci_"+fnam+'.log','unknown')
  save(pathconvert(TMPDIR)+"logfile.dat",logfile)
end

// Output beginning message
mss=["****** Beginning of mfile2sci() session ******";
    "File to convert: "+fil;
    "Result file path: "+res_path;
    "Recursive mode: "+rec;
    "Only double values used in M-file: "+dble;
    "Verbose mode: "+string(verbose_mode);
    "Generate formated code: "+pretty]
m2sci_info(mss,-1);

// Read in the file as text
m2sci_info("M-file reading...",-1);
txt=mgetl(fil);
m2sci_info("M-file reading: Done",-1);

//Replace TAB by SPACE
txt=strsubst(txt,code2str(-40),"")

if txt==[] then
  m2sci_infos("File "+fil+"is an empty file ! Nothing done...",-1);
  return
end
tmptxt=txt

// Make minor changes on syntax
m2sci_info("Syntax modification...",-1);
ierr=execstr("load(''"+pathconvert(TMPDIR)+fnam+ ".tree'',''txt'',''helppart'',''batch'')",'errcatch','n')
if ierr<>0 | exists('txt')==0 | exists('batch')==0 & strindex(res_path,TMPDIR)==[] then
  [helppart,txt,batch]=m2sci_syntax(txt)
elseif ierr==0 & newest(fil,pathconvert(TMPDIR)+fnam+ ".tree")==1 then
    [helppart,txt,batch]=m2sci_syntax(tmptxt)
end

m2sci_info("Syntax modification: Done",-1);

// Write .cat file and update whatis
if helppart<>[] then
  catfil=res_path+fnam+".cat"
  whsfil=res_path+"whatis"
  mputl(helppart,catfil)
  if exists("whsfil_unit")==1 then
    write(whsfil_unit,stripblanks(helppart(1))+" |"+fnam,"(a)")
  end
end

if txt~=[] then
  quote="''";
  dquote="""";
  kc=strindex(txt(1),"function");
  kc=kc(1);

  // Define Scilab function
  fprot=funcprot();
  funcprot(0);
  
  // Blanks in file name are replaced by _ for batch
  // kc+9 because 'function '
  ksc=min(strindex(txt(1),";")) // searching for a comment on first line after function prototype
  if isempty(ksc) then 
    ksc=length(txt(1))+1;
    firstline=[]
  else
    firstline=part(txt(1),ksc+1:length(txt(1)));
  end
  
  func_proto=part(txt(1),kc+9:ksc-1)
  keq=min(strindex(func_proto,"="))
  kpar=min(strindex(func_proto,"("))
  if isempty(keq) then
    keq=1
  end
  if isempty(kpar) then
    kpar=length(func_proto)+1
  end
  func_proto=part(func_proto,1:keq)+..
      strsubst(stripblanks(part(func_proto,keq+1:kpar-1))," ","_")+..
      part(func_proto,kpar:length(func_proto))

  deff(func_proto,[firstline;txt(2:$)],"n")
  w=who("get");
  mname=w(1);
  nametbl=[nametbl;mname]
  if fnam<>mname & ~batch then // warning is not displayed for a batch file
    mss=["Warning: file "+fil+" defines function "+mname+" instead of "+fnam;
	"         "+mname+".sci, "+mname+".cat and sci_"+mname+".sci will be generated !"]
    m2sci_info(mss,-1);
  end

  // Compilation
  execstr("comp("+mname+",1)")
  funcprot(fprot)

  // Get Scilab pseudo code of the function
  m2sci_info("Macro to tree conversion...",-1); 
  macr=evstr(mname)
  mtlbtree=macr2tree(macr);
  if ~batch then
    mtlbtree.name=mname;
  else
    mtlbtree.name="";
  end
  
  //Transfom a equal instructions(if lhs are multi_operation insert and expression is a funcall) in the matlab tree to sup_equal instructions
  global("tmpvarnb")
  tmpvarnb=0;
  level=[0,0];
  ninstr=1;
  while ninstr<=size(mtlbtree.statements)-3
    mtlbtree.statements(ninstr)=transformtree(mtlbtree.statements(ninstr))
    ninstr=ninstr+1
  end

  // Perform the translation
  [scitree,trad,hdr,crp]=m2sci(mtlbtree,w(1),Recmode,prettyprint)

  //Creation of fname_resume.log file
 // if mtlbref_fun<>[]|not_mtlb_fun<>[]|mtlbtool_fun<>[] then
    //resume_logfile initialisation
    if exists("resume_logfile")==0 then
      [tempfd2,ierr2]=file('open',pathconvert(TMPDIR)+"resumelogfile.dat","old")
      if ierr2==0 then
        load(pathconvert(TMPDIR)+"resumelogfile.dat")
        file('close',tempfd2)
        file('close',resume_logfile)
        mdelete(pathconvert(TMPDIR)+"resumelogfile.dat")
      end
      resume_logfile=file('open',res_path+"resume_m2sci_"+fnam+".log",'unknown')
      save(pathconvert(TMPDIR)+"resumelogfile.dat",resume_logfile)
    end
       
    //number of matlab reference functions, matlab toolboxes functions, not matlab functions
    size1=size(mtlbref_fun,1)
    size2=size(mtlbtool_fun,1)
    size3=size(not_mtlb_fun,1)
    
    if size(mtlbref_fun,"*")<>0 then
      mtlbref_fun(:,1)=""""+mtlbref_fun(:,1)+""""
    end
    if size(mtlbtool_fun,"*")<>0 then
      mtlbtool_fun(:,1)=""""+mtlbtool_fun(:,1)+""""
    end
    if size(not_mtlb_fun,"*")<>0 then
      not_mtlb_fun(:,1)=""""+not_mtlb_fun(:,1)+""""
    end

    info_resume=["****** " + fnam + ": Functions of mfile2sci() session ******";
	"*";
	string(size1)+" Matlab Function(s) not yet converted, original calling sequence used : ";
	mtlbref_fun(:,1)+mtlbref_fun(:,2);
	"*";
	string(size2)+" Matlab Toolbox(es) Functions, original calling sequence used :";
	mtlbtool_fun(:,1)+mtlbtool_fun(:,2);
	"*";
	string(size3)+" Unknown Function(s), original calling sequence used :";
	not_mtlb_fun(:,1)+not_mtlb_fun(:,2);
	"*"]

    write(resume_logfile,margin+info_resume)
    file("close",resume_logfile)
    mdelete(pathconvert(TMPDIR)+"resumelogfile.dat")
  //end
  
  m2sci_info("Macro to tree conversion: Done",-1);

  crp(1)=""; // Delete function prototype
  if isempty(firstline) then
    res=[hdr;crp]
  else
    hdr(1)=hdr(1)+" "+crp(2);
    crp(2)=[];
    res=[hdr;crp];
  end

  // Strip last return and blank lines
  n=size(res,1)
  while res(n)==part(" ",1:length(res(n))) then 
    n=n-1
  end
  res=res(1:n)

  // Write sci-file
  ext=".sci"
  scifil=res_path+fnam+ext
  mputl(res,scifil)

  // Write sci_<mname>.sci translation file
  if trad<>[] then
    sci_fil=res_path+"sci_"+mname+".sci"
    mputl(trad,sci_fil)
    res=1
  else
    res=0
  end

  // Output summary information
  infos=[]
  if m2sci_infos(1) then
    infos=["Translation may be improved: see the //! comments and for all mtlb_<funname> function call"
	"  Type help mtlb_<funname> in Scilab command window to get information about improvements"]
  end
  if m2sci_infos(2) then
    infos=[infos;"Translation may be wrong (see the //!! comments)"]
  end

  nametbl($)=[]

else
  infos="File contain no instruction, no translation made..."
end

// End of translation messages
mss="****** End of mfile2sci() session ******"

m2sci_info([infos;mss],-1);

if Reclevel>1 then
  m2sci_infos=m2sci_infos_save
end

file("close",logfile)
clearglobal m2sci_infos
clearglobal mtlbref_fun
clearglobal mtlbtool_fun
clearglobal not_mtlb_fun
// For execution called by translatepaths()
//nametbl=resume(nametbl)
mdelete(pathconvert(TMPDIR)+fnam+ ".tree")
mdelete(pathconvert(TMPDIR)+"logfile.dat")

endfunction
