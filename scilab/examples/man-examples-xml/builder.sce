mode(-1)
path=get_absolute_file_path('builder.sce');

myhelps=[path+'helpdir1', "Title1";
	 path+'helpdir2', "Title2"]

// process the xml files to produce html files 

// update %helps for cross reference
%helps_save=%helps
%helps=[%helps;myhelps] 
xmltohtml(myhelps(:,1),myhelps(:,2));

//restore the previous help table
%helps=%helps_save
//erase temporary variables
clear %helps_save xmltohtml myhelps path get_absolute_file_path
