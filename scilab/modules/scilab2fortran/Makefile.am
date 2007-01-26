##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


#### Target ######
target=$(datadir)/scilab2fortran


#### scilab2fortran : Conf files ####
libscilab2fortran_la_rootdir = $(target)
libscilab2fortran_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### scilab2fortran : Scilab macros ###
libscilab2fortran_la_macrodir = $(target)/macros
MACROS = macros/f_gener.sci \
macros/setparam.sci \
macros/%a2for.sci \
macros/%log2for.sci \
macros/%c2for.sci \
macros/%e2for.sci \
macros/func2f.sci \
macros/%imp2for.sci \
macros/mltopsfor.sci \
macros/%h2for.sci \
macros/%i2for.sci \
macros/f_tan.sci \
macros/num2f.sci \
macros/f_cond.sci \
macros/f_cos.sci \
macros/%m2for.sci \
macros/%p2for.sci \
macros/%r2for.sci \
macros/%s2for.sci \
macros/%t2for.sci \
macros/typconv.sci \
macros/f_maxi.sci \
macros/op2for.sci \
macros/f_size.sci \
macros/ins2for.sci \
macros/callfun.sci \
macros/isnum.sci \
macros/f_argn.sci \
macros/f_sqrt.sci \
macros/cod2for.sci \
macros/f_error.sci \
macros/f_rank.sci \
macros/f_exp.sci \
macros/getwrk.sci \
macros/get2f.sci \
macros/sci2for.sci \
macros/newlab.sci \
macros/outname.sci \
macros/f_ones.sci \
macros/lin2for.sci \
macros/f_norm.sci \
macros/f_type.sci \
macros/f_log.sci \
macros/f_eye.sci \
macros/indentfor.sci \
macros/genif.sci \
macros/f_min.sci \
macros/cla2for.sci \
macros/makeargs.sci \
macros/f_max.sci \
macros/f_sin.sci \
macros/f_mini.sci \
macros/adderr.sci \
macros/modlst.sci \
macros/%rc2for.sci \
macros/gencall.sci \
macros/freewrk.sci \
macros/forparam.sci \
macros/f_zeros.sci \
macros/f_abs.sci \
macros/dclfun.sci \
macros/f_expm.sci \
macros/indent.sci \
macros/%cc2for.sci \
macros/exp2for.sci \
macros/basparam.sci \
macros/getlocal.sci \
macros/f_ent.sci \
macros/str2f.sci \
macros/f_svd.sci macros/%a2for.bin \
macros/adderr.bin \
macros/basparam.bin \
macros/%c2for.bin \
macros/callfun.bin \
macros/%cc2for.bin \
macros/cla2for.bin \
macros/cod2for.bin \
macros/dclfun.bin \
macros/%e2for.bin \
macros/exp2for.bin \
macros/f_abs.bin \
macros/f_argn.bin \
macros/f_cond.bin \
macros/f_cos.bin \
macros/f_ent.bin \
macros/f_error.bin \
macros/f_expm.bin \
macros/f_exp.bin \
macros/f_eye.bin \
macros/f_gener.bin \
macros/f_log.bin \
macros/f_maxi.bin \
macros/f_max.bin \
macros/f_mini.bin \
macros/f_min.bin \
macros/f_norm.bin \
macros/f_ones.bin \
macros/forparam.bin \
macros/f_rank.bin \
macros/freewrk.bin \
macros/f_sin.bin \
macros/f_size.bin \
macros/f_sqrt.bin \
macros/f_svd.bin \
macros/f_tan.bin \
macros/f_type.bin \
macros/func2f.bin \
macros/f_zeros.bin \
macros/gencall.bin \
macros/genif.bin \
macros/get2f.bin \
macros/getlocal.bin \
macros/getwrk.bin \
macros/%h2for.bin \
macros/%i2for.bin \
macros/%imp2for.bin \
macros/indentfor.bin \
macros/indent.bin \
macros/ins2for.bin \
macros/isnum.bin \
macros/lin2for.bin \
macros/%log2for.bin \
macros/%m2for.bin \
macros/makeargs.bin \
macros/mltopsfor.bin \
macros/modlst.bin \
macros/newlab.bin \
macros/num2f.bin \
macros/op2for.bin \
macros/outname.bin \
macros/%p2for.bin \
macros/%r2for.bin \
macros/%rc2for.bin \
macros/%s2for.bin \
macros/sci2for.bin \
macros/setparam.bin \
macros/str2f.bin \
macros/%t2for.bin \
macros/typconv.bin macros/names macros/lib macros/buildmacros.sce


#### scilab2fortran : init scripts #### 
libscilab2fortran_la_etcdir = $(target)/etc
libscilab2fortran_la_etc_DATA = etc/scilab2fortran.quit etc/scilab2fortran.start



#### scilab2fortran : demos files ####
libscilab2fortran_la_demosdir=$(target)/demos/
libscilab2fortran_la_demos_DATA = demos/dem1.sce \
demos/dem2.sce \
demos/dem3.sce \
demos/dem4.sce \
demos/dem5.sce \
demos/demotrad.dem \
demos/dem6.sce \
demos/printspecs.sci

include $(top_srcdir)/Makefile.incl.am
