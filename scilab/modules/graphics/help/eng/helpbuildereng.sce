mode(-1); //force silent execution
path=get_absolute_file_path('helpbuildereng.sce');//get the absolute path of this file
add_help_chapter("Graphics Library",path);//add help chapter
xmltohtml(path,"graphics Library");
//clear the variable stack
clear path add_help_chapter get_absolute_file_path; 
