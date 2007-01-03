##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/tfds


#### tfds : Conf files ####
libtfds_la_rootdir = $(target)
libtfds_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### tfds : Scilab macros ###
libtfds_la_macrodir = $(target)/macros
libtfds_la_macro_DATA22 = macros/artest.sci \
macros/logr.sci \
macros/bifish.sci \
macros/portrait.sci \
macros/BInit.sci \
macros/systems.sci \
macros/finit.sci \
macros/boucle.sci \
macros/lotest.sci \
macros/mineInit.sci \
macros/comric.sci \
macros/gaincom.sci \
macros/recur.sci \
macros/obsric.sci \
macros/gainobs.sci \
macros/chaintest.sci \
macros/test_d.sci \
macros/obscont1.sci \
macros/gpeche.sci \
macros/tdinit.sci \
macros/portr3d.sci \
macros/exemple.sci \
macros/tangent.sci macros/artest.bin \
macros/bifish.bin \
macros/BInit.bin \
macros/boucle.bin \
macros/chaintest.bin \
macros/comric.bin \
macros/exemple.bin \
macros/finit.bin \
macros/gaincom.bin \
macros/gainobs.bin \
macros/gpeche.bin \
macros/logr.bin \
macros/lotest.bin \
macros/mineInit.bin \
macros/obscont1.bin \
macros/obsric.bin \
macros/portr3d.bin \
macros/portrait.bin \
macros/recur.bin \
macros/systems.bin \
macros/tangent.bin \
macros/tdinit.bin \
macros/test_d.bin macros/names macros/lib macros/buildmacros.sce


#### tfds : init scripts #### 
libtfds_la_etcdir = $(target)/etc
libtfds_la_etc_DATA = etc/tfds.quit etc/tfds.start

include $(top_srcdir)/Makefile.incl.am
