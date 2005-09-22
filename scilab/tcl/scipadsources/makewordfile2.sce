//////this from dynamickeywords.sce
function cset=lineform(keywordlist)
   keywordlist=sort(keywordlist)
   initial=gsort(unique(part(keywordlist,1)),"r","i")
   cset=""
   for i=1:size(initial,1)
     cset(i)=strcat(keywordlist(part(keywordlist,1)==initial(i))," ")
   end
endfunction
//////////////////////////

whatlines=unix_g("scilab -nw -ns -e ""what;quit""");

allfun=[]; allcomm=[];
for i=1:size(whatlines,1)
  a=whatlines(i);
  if a==" Commands:" then break; end
  if ~(a=="" | a=="Internal functions: ")
    b=tokens(a," ");
    allfun=[allfun;b];
  end
end
for j=(i+1):size(whatlines,1)
  a=whatlines(j);
  if a~=""
    b=tokens(a," ");
    allcomm=[allcomm;b];
  end
end

//scicos functions (derived from the listing od SCI/macro/scicos,
// assuming 1:1 filename correspondance
curdir=pwd();
chdir("SCI/macros/scicos/");
scicosfun=strsubst(listfiles("*.bin"),".bin","");
chdir("SCI/macros/scicos_blocks/");
scicosblocks=basename(listfiles("*/*.bin"));
scicosblocks=strsubst(scicosblocks,".bin","");
chdir(curdir);


allfun=lineform(allfun);
allcom=lineform(allcomm);
allscicos=lineform([scicosfun;scicosblocks]);

allwords=["#MODE scilab";"#TAG intfun";allfun;..
                         "#TAG comm";allcom;
                         "#TAG scicos";allscicos];

mdelete words;
fd=mopen("words","w");
for i=1:size(allwords,1)
  mfprintf(fd,"%s\n",allwords(i));
end
mclose(fd);

