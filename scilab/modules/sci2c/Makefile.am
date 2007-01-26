

#### Target ######
target=$(datadir)/sci2c


#### sci2c : Conf files ####
libsci2c_la_rootdir = $(target)
libsci2c_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### sci2c : Scilab macros ###
libsci2c_la_macrodir = $(target)/macros
MACROS = macros/sci2c.sci  macros/names macros/lib macros/buildmacros.sce


#### sci2c : init scripts #### 
libsci2c_la_etcdir = $(target)/etc
libsci2c_la_etc_DATA = etc/sci2c.quit etc/sci2c.start

#### sci2c : gateway declaration #### 
libsci2c_la_sci_gatewaydir = $(target)/sci_gateway
libsci2c_la_sci_gateway_DATA = sci_gateway/sci2c


include $(top_srcdir)/Makefile.incl.am
