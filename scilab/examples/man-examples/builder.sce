
// conversion of old man files to xml file 
dtd= SCI+'/man/man.dtd' 

myhelps=['helpdir1', "Title1";
	 'helpdir2', "Title2"]
n=size(myhelps,'r'); 

for k=1:n 
  formatman(myhelps(k,1),"xml",dtd);
end 

// process the xml files to produce html files 

xmltohtml(myhelps(:,1),myhelps(:,2),'html-jpc.xsl');


