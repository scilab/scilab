dtd= SCI+'/man/man.dtd' 

myhelps=['helpdir1', "Title1";
	 'helpdir2', "Title2"]

// process the xml files to produce html files 

xmltohtml(myhelps(:,1),myhelps(:,2),'html-jpc.xsl');


