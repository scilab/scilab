// bug6

mode (-1)
clear all
//getf ('../fonct_qualif.sci')

files = ['']
files=listfiles([SCI+'/macros/scicos/*.cosf'])

affich_result(files <> [''], 6)

//disp("Error n° "+string(ierr))
//disp(lasterror());

clear all

