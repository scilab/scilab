/////////////////////////////////////////////////////////////
// utility script for Scipad translators:
//  Compares two SCI/tcl/scipadsources/msg_files/*.msg files,
//   generally, a master one like fr.msg or it.msg, updated by
//   the mantainers of Scipad, and a derived msg file, mantained by
//   one of the contributing translators;
//  reports the missing and additional entries in the second one;
//  generates a new *.msg.rev file, which includes placeholders
//   for the new entries to be translated "???--master translation",
//   all nicely reformatted.
//
// The syntax is assumed to be very rigid here: each line is one of 5:
//  1) a blank line
//  2) a comment beginning with # 
//  3) a line with ::msgcat::mcset xx, followed by either two double-quoted
//       strings, or one unquoted keyword and one quoted translation string.
//       xx has to match the filename, and there is a single space between
//       mcset and xx.
//  4) the beginning of the above, ending in \
//  5) a continuation of 3)
//
//  Use (from scilab):
//   msgdiff(master_msgfile_path,derived_msgfile_path)
// or
//   report=msgdiff(master_msgfile_path,derived_msgfile_path);
//   mprintf("%s\n",report)
//
//   msgdiff(master_msgfile_path)  just reports syntax warnings and
//         generates a reformatted rev file
//
//  known bugs: 
//   -unquoted label names are wrapped by quotes in the rev file (apparently
//    harmless
//   -Quoted strings ending by \ (there is one such occurrence in fr.msg)
//    confuse the parser, the result has a " too much
//
/////////////////////////////////////////////////////////////

function report=msgdiff(msgfile1,msgfile2)
  [M1_1,M2_1,after1,lastcomment1]=msglist(msgfile1);
  if exists("msgfile2","local") then
    [M1_2,M2_2,after2,lastcomment2]=msglist(msgfile2);
  else
    msgfile2=msgfile1
    M1_2=M1_1; M2_2=M2_1; after2=after1; lastcomment2=lastcomment1;
  end
  lang2=basename(msgfile2)
//find missing entries and write the result file
  disp("Finding missing entries in "+msgfile2+" and writing the result...")
  report(1)="Translations missing in file "+msgfile2+":";
  report(2)=""; j=2; section1=""; k=1
  revfile=msgfile2+".rev"
  mdelete(revfile)
  fd=mopen(revfile,"w")
// we write the header of the SECOND file to the result
  mfprintf(fd,"%s\n",lastcomment2(1:after2(1))); k=after2(1)
  for i=1:size(M1_1,1)
    if after1(i)>k then
      mfprintf(fd,"%s\n",lastcomment1((k+1):after1(i)))
      k=after1(i)
    end
    l=find(M1_2==M1_1(i)); l=l(1) 
    if l<>[] then
      mfprintf(fd,"%s\n",lineformat(lang2,M1_2(l),M2_2(l)))
    else
      mfprintf(fd,"%s\n",lineformat(lang2,M1_1(i),"???--"+M2_1(i)))
      section=lastcomment1(after1(i));
      if section<>section1 then
        j=j+1; report(j)=""; j=j+1; report(j)=section
      end
      j=j+1;
      report(j)="::msgcat::mcset "+lang2+" """+M1_1(i)+""" ""???"""
      section1=section;
    end
  end
  mclose(fd)
  
//unused strings
  disp("Finding unused entries in " +msgfile2+"...")
  j=j+1; report(j)=""; 
  j=j+1; report(j)="Unused strings in file "+msgfile2+":"; 
  section1=""
  for i=1:size(M1_2,1)
    if ~or(M1_1==M1_2(i)) then
      section=lastcomment2(after2(i));
      if section<>section1 then
        j=j+1; report(j)="";j=j+1; report(j)=section
      end
      j=j+1;
      report(j)="::msgcat::mcset "+lang2+" """+M1_2(i)+""" """+M2_2(i)+""""
      section1=section;
    end
  end
endfunction


///////////////////////////////////////////////


function [M1,M2,after,lastcomment]=msglist(msgfile)
  disp("Parsing file "+msgfile+", be patient...")
  lang=basename(msgfile)
//read the file
  M=""; i=1
  fd=mopen(msgfile)
  while ~meof(fd)
    a=stripblanks(mgetl(fd,1))
    n=max(length(M(i)),1)
    if part(M(i),n)=="\" then
      M(i)=part(M(i),1:n-1)+" "+a
    else
      i=i+1; M(i)=a;
    end
  end
  mclose(fd)
//parse the entries: orig and translated string, remove comments
  lastcomment=[];after=0; M1=[];a1=""; j=1; k=1;
  for i=2:size(M,1)   //M(1)="" because of the above
    a=M(i)
    if part(a,1)=="#" | a=="" then
      lastcomment(j)=a
      j=j+1
    elseif grep(a,"::msgcat::mcset "+lang)==1 then
      a=strsubst(a,"\""","''''")
      t=tokenpos(a,"""");
    // normally the line contains 4 quotes, enclosing two quoted strings 
      if size(t,1)==2 then
    // this accounts for lines where the first string is an unquoted tcl variable
        a1=part(a,t($-1,1):t($-1,2));
        a1=tokens(a1," "); a1=a1(3)
      else
        a1=part(a,t($-2,1):t($-2,2));          
      end
      a2=part(a,t($,1):t($,2)); 
      M1(k)=strsubst(a1,"''''","\""");
      M2(k)=strsubst(a2,"''''","\""");
      after(k)=j-1; k=k+1
    else
      write(%io(2),"WARNING: THE FOLLOWING LINE SEEMS WRONG HERE AND WILL BE IGNORED")
      write(%io(2),a)
    end
  a1=a;
  end
endfunction

///////////////////////////////////////////////
// auxiliary functions for wrapping a long line over several lines

function l=lineformat(lang,orig,transl)
  prefix="::msgcat::mcset "+lang+" "
  lindent="                   "
  n1=length(prefix)
  n2=length(orig)
  n3=length(transl)
  n4=length(lindent)
  maxline=80
  
  if n1+n2+3+n3+2<maxline then
//all on a single line, ok
    l=prefix+""""+orig+""" """+transl+""""
  else
  //orig on a line and transl on the next
    if n1+n2+3<maxline & n4+n3+2<maxline then
      l(1)=prefix+""""+orig+"""\"
      l(2)=lindent+""""+transl+""""
    end
  //orig on one line and transl on several
    if n1+n2+3<maxline & n4+n3+2>=maxline then
      l(1)=prefix+""""+orig+"""\"
      s=linesplit(lindent+""""+transl+"""",lindent)
      l(2:size(s,1)+1)=s
    end
  //orig on several lines and transl on one
    if n1+n2+3>=maxline & n4+n3+2<maxline then
      s=linesplit(prefix+""""+orig+"""\")
      l(1:size(s,1))=s
      l(size(s,1)+1)=lindent+""""+transl+""""
    end
  //orig on several lines and transl on several
    if n1+n2+3>=maxline & n4+n3+2>=maxline then
      s=linesplit(prefix+""""+orig+"""\")
      l(1:size(s,1))=s
      s=linesplit(lindent+""""+transl+"""",lindent)
      l($+(1:size(s,1)))=s
    end
  end
endfunction

function s=linesplit(longstring,lindent)
  n=length(longstring)
  d=length(lindent)
  maxline=80
  if n+2<maxline then 
    s=longstring;
    return;
  else
    p=tokenpos(longstring," ");
    i=1;j=0;m=1
    while j<n
      if j==0 then 
        k=find((p(:,2)+2-j+1)<maxline & p(:,1)>j)
      else
        k=find((p(:,2)+d+2-j+1)<maxline & p(:,1)>j)
      end
      if k==[] then
        m=m+1
        if m<size(p,1) then q=p(m+1,1)-2; else q=p(m,2); end
        s(i)=part(longstring,j:q-1) 
      else
        if k($)<size(p,1) then q=p(k($)+1,1)-2; else q=p(k($),2); end
        s(i)=part(longstring,(j+1):q)
        m=k($)
      end
      if i>1 then s(i)=lindent+s(i); end
      j=q
      if j<n then s(i)=s(i)+"\"; end
      i=i+1;
    end
  end
endfunction
