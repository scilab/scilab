lines(0);
// look for .xml files
if MSDOS then
  xml=unix_g('dir /B *.xml')
  xml=xml($:-1:1)
else
  xml=unix_g('ls  -t1 *.xml') 
end
for k1=1:size(xml,'*')  // loop on .xml files
  path=xml(k1)
  if newest(path,strsubst(path,".xml",".html"))==1 then
    find_links(path,path+"2")
  end
end
quit
