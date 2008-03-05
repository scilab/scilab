//** Entry point function list list 
comp_fun = ["comedi_analog_input","comedi_analog_output","comedi_digital_output","comedi_digital_input"]

//** File list 
c_prog_lst = listfiles('*.c')
prog_lst = strsubst(c_prog_lst, '.c', '.o');

libs = "libcomedi.so" ; //** external Comedi library

flag = "c"         ; //** C code 

makename = "Makelib"; //** default makefile 

loadername = "loader.sce" //** default loader 

libname = "hilcomedi"; //** output library 

ldflags = ""; //** liker options

cflags = "-lcomedi -lm"; //** C compiler options  

libn = ilib_for_link(comp_fun, prog_lst, libs, flag, makename, loadername, libname)

disp("Shared library ready ..."); 
