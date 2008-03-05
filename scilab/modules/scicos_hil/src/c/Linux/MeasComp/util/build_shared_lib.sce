//** 6 Dic 2007
comp_fun = ['diff_analog_input','analog_output','digital_input','digital_output']

c_prog_lst = listfiles('*.c')

prog_lst = strsubst(c_prog_lst, '.c', '.o');

libs = "libmccusb.so" ; //** external USB library 

flag = "c"         ; //**

makename = 'Makelib';

loadername = 'loader.sce'

libname ='hilmcusb1208fs'

libn = ilib_for_link(comp_fun, prog_lst, libs, flag, makename, loadername, libname)

