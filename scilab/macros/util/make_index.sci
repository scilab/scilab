function make_index()
  global INDEX
  sep="/";if MSDOS then sep="\",end
  dirs=%helps(:,1)
  txt=%helps(:,2)
  l=0
  for k=1:size(dirs,'*')
    d=dirs(k)
    w=d+sep+"whatis.htm"
    l=l+1;
    line(l)="<BR><A HREF="""+w+""">"+txt(k)+"</A>"
  end
  INDEX=TMPDIR+sep+"index.htm"
  mputl(line,INDEX)
endfunction
