mode(-1); //force silent execution
path=get_absolute_file_path('helpbuilderfr.sce');//get the absolute path of this file
add_help_chapter("Matrices creuses",path);//add help chapter
xmltohtml(path,"Matrices creuses",'html-rev.xsl');
//clear the variable stack
clear path add_help_chapter get_absolute_file_path; 