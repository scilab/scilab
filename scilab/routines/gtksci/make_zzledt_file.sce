//creates the file zzledt.txt (used by the gtk shell for tab-autocompletion)
// derived from SCI/tcl/scipadsources/dynamickeywords.sce
// Author: Enrico Segre

//predefined variables
names=who("get"); pdef=names(($-predef())+1:$);

//library functions
libfun=[]; libvar=[];
for i=1:size(names,1)
  if type(eval(names(i)))==14 then
     libvar=[libvar;names(i)];
     libstring=string(eval(names(i)));
     libfun=[libfun;libstring(2:$)];
  end
end

//commands and internal functions
[intfun,allcomm]=what();

//scicos functions (derived from the listing od SCI/macro/scicos,
// assuming 1:1 filename correspondance
curdir=pwd();
chdir("SCI/macros/scicos/");
scicosfun=strsubst(listfiles("*.bin"),".bin","");
chdir("SCI/macros/scicos_blocks/");
scicosblocks=basename(listfiles("*/*.bin"));
scicosblocks=strsubst(scicosblocks,".bin","");
chdir(curdir);

allwords=unique([intfun;allcomm;pdef;libfun;scicosfun;scicosblocks]);

mdelete zzledt.txt;
fd=mopen("zzledt.txt","w");
for i=size(allwords,1):-1:1
  mfprintf(fd,"""%s"",\n",allwords(i));
end
mclose(fd);

