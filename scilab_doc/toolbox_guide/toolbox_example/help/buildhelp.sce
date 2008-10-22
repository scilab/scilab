mode(-1) //force silent execution
path=get_absolute_file_path('builhelp.sce');//get the absolute path of this file
add_help_chapter("Title1",path);//add help chapter
xmltohtml(path,"Title1")
//clear the variable stack
clear path add_help_chapter get_absolute_file_path 