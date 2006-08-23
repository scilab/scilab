mode(-1); //force silent execution
path=get_absolute_file_path('helpbuildereng.sce');//get the absolute path of this file
add_help_chapter("GUI",path);//add help chapter
xmltohtml(path,"GUI");
//clear the variable stack
clear path add_help_chapter get_absolute_file_path; 