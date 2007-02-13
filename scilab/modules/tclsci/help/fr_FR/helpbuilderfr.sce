mode(-1); //force silent execution
path=get_absolute_file_path('helpbuilderfr.sce');//get the absolute path of this file
add_help_chapter("Interface Tcl/Tk",path);//add help chapter
xmltohtml(path,"Interface Tcl/Tk",'html-rev.xsl');
//clear the variable stack
clear path add_help_chapter get_absolute_file_path; 