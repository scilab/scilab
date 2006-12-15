##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

STATISTICS_C_SOURCES = src/c/CdfBase.c \
src/c/ipmpar1.c

STATISTICS_FORTRAN_SOURCES = src/dcdflib/psi.f \
src/dcdflib/dexpm1.f \
src/dcdflib/alngam.f \
src/dcdflib/erfc1.f \
src/dcdflib/cdff.f \
src/dcdflib/spmpar.f \
src/dcdflib/cumnor.f \
src/dcdflib/cdfpoi.f \
src/dcdflib/bcorr.f \
src/dcdflib/cumchi.f \
src/dcdflib/apser.f \
src/dcdflib/gamln1.f \
src/dcdflib/dlanor.f \
src/dcdflib/cdft.f \
src/dcdflib/gaminv.f \
src/dcdflib/cumnbn.f \
src/dcdflib/cdffnc.f \
src/dcdflib/rlog1.f \
src/dcdflib/cdfchn.f \
src/dcdflib/dt1.f \
src/dcdflib/bgrat.f \
src/dcdflib/dinvnr.f \
src/dcdflib/dlngam.f \
src/dcdflib/rlog.f \
src/dcdflib/basym.f \
src/dcdflib/bfrac.f \
src/dcdflib/cumbin.f \
src/dcdflib/cumbet.f \
src/dcdflib/rcomp.f \
src/dcdflib/cdfgam.f \
src/dcdflib/gratio.f \
src/dcdflib/gam1.f \
src/dcdflib/exparg.f \
src/dcdflib/ipmpar.f \
src/dcdflib/cumf.f \
src/dcdflib/cumpoi.f \
src/dcdflib/gamln.f \
src/dcdflib/dstrem.f \
src/dcdflib/dln1mx.f \
src/dcdflib/betaln.f \
src/dcdflib/stvaln.f \
src/dcdflib/cumt.f \
src/dcdflib/esum.f \
src/dcdflib/bpser.f \
src/dcdflib/cdfnor.f \
src/dcdflib/cumfnc.f \
src/dcdflib/erf.f \
src/dcdflib/alnrel.f \
src/dcdflib/cumchn.f \
src/dcdflib/rexp.f \
src/dcdflib/fpser.f \
src/dcdflib/bratio.f \
src/dcdflib/cdfchi.f \
src/dcdflib/gamma.f \
src/dcdflib/dinvr.f \
src/dcdflib/brcomp.f \
src/dcdflib/devlpl.f \
src/dcdflib/cdfnbn.f \
src/dcdflib/dzror.f \
src/dcdflib/gsumln.f \
src/dcdflib/dln1px.f \
src/dcdflib/algdiv.f \
src/dcdflib/dlnbet.f \
src/dcdflib/cumgam.f \
src/dcdflib/dbetrm.f \
src/dcdflib/grat1.f \
src/dcdflib/brcmp1.f \
src/dcdflib/cdfbin.f \
src/dcdflib/bup.f \
src/dcdflib/cdfbet.f

GATEWAY_C_SOURCES = sci_gateway/c/gw_statistics.c \
sci_gateway/c/sci_cdfchi.c \
sci_gateway/c/sci_cdft.c \
sci_gateway/c/sci_cdfgam.c \
sci_gateway/c/sci_cdff.c \
sci_gateway/c/sci_cdffnc.c \
sci_gateway/c/sci_cdfpoi.c \
sci_gateway/c/sci_cdfbin.c \
sci_gateway/c/sci_cdfchn.c \
sci_gateway/c/sci_cdfbet.c \
sci_gateway/c/sci_cdfnbn.c \
sci_gateway/c/sci_cdfnor.c

libstatistics_la_CFLAGS=  -Iincludes/
#-Isrc/c/
lib_LTLIBRARIES = libstatistics.la

libstatistics_la_SOURCES = $(STATISTICS_C_SOURCES) $(STATISTICS_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES)

libstatistics_la_LIBADD = \
	$(top_builddir)/libs/lapack/liblapack.la

#	$(top_builddir)/modules/arnoldi/libarnoldi.la \
#	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la 

libstatistics_la_PKGCONFIG = statistics.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/statistics


#### statistics : Conf files ####
libstatistics_la_rootdir = $(target)
libstatistics_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### statistics : Scilab macros ###
libstatistics_la_macrodir = $(target)/macros
libstatistics_la_macro_DATA = macros/cmoment.sci \
macros/strange.sci \
macros/wcenter.sci \
macros/regress.sci \
macros/nfreq.sci \
macros/geomean.sci \
macros/harmean.sci \
macros/trimmean.sci \
macros/nanmean.sci \
macros/center.sci \
macros/mvvacov.sci \
macros/sample.sci \
macros/nanstdev.sci \
macros/ftest.sci \
macros/stdev.sci \
macros/correl.sci \
macros/variance.sci \
macros/perctl.sci \
macros/mvcorrel.sci \
macros/nanmedian.sci \
macros/quart.sci \
macros/variancef.sci \
macros/moment.sci \
macros/mad.sci \
macros/ftuneq.sci \
macros/pca.sci \
macros/nand2mean.sci \
macros/nancumsum.sci \
macros/tabul.sci \
macros/covar.sci \
macros/nanmin.sci \
macros/mean.sci \
macros/nanmax.sci \
macros/nanmeanf.sci \
macros/msd.sci \
macros/meanf.sci \
macros/samplef.sci \
macros/st_deviation.sci \
macros/samwr.sci \
macros/iqr.sci \
macros/stdevf.sci \
macros/median.sci \
macros/thrownan.sci \
macros/nansum.sci macros/center.bin \
macros/cmoment.bin \
macros/correl.bin \
macros/covar.bin \
macros/ftest.bin \
macros/ftuneq.bin \
macros/geomean.bin \
macros/harmean.bin \
macros/iqr.bin \
macros/mad.bin \
macros/meanf.bin \
macros/mean.bin \
macros/median.bin \
macros/moment.bin \
macros/msd.bin \
macros/mvcorrel.bin \
macros/mvvacov.bin \
macros/nancumsum.bin \
macros/nand2mean.bin \
macros/nanmax.bin \
macros/nanmeanf.bin \
macros/nanmean.bin \
macros/nanmedian.bin \
macros/nanmin.bin \
macros/nanstdev.bin \
macros/nansum.bin \
macros/nfreq.bin \
macros/pca.bin \
macros/perctl.bin \
macros/quart.bin \
macros/regress.bin \
macros/samplef.bin \
macros/sample.bin \
macros/samwr.bin \
macros/stdevf.bin \
macros/st_deviation.bin \
macros/stdev.bin \
macros/strange.bin \
macros/tabul.bin \
macros/thrownan.bin \
macros/trimmean.bin \
macros/variancef.bin \
macros/variance.bin \
macros/wcenter.bin macros/names macros/lib macros/buildmacros.sce


#### statistics : init scripts #### 
libstatistics_la_etcdir = $(target)/etc
libstatistics_la_etc_DATA = etc/statistics.quit etc/statistics.start

#### statistics : gateway declaration #### 
libstatistics_la_sci_gatewaydir = $(target)/sci_gateway
libstatistics_la_sci_gateway_DATA = sci_gateway/statistics

#### statistics : documentation FR #### 
libstatistics_la_helpfrdir = $(target)/help/fr
libstatistics_la_helpfr_DATA = help/fr/labostat.htm \
help/fr/cmoment.htm \
help/fr/correl.htm \
help/fr/covar.htm \
help/fr/ftest.htm \
help/fr/ftuneq.htm \
help/fr/geomean.htm \
help/fr/harmean.htm \
help/fr/iqr.htm \
help/fr/mean.htm \
help/fr/mad.htm \
help/fr/nanmeanf.htm \
help/fr/moment.htm \
help/fr/msd.htm \
help/fr/mvvacov.htm \
help/fr/nanmax.htm \
help/fr/nand2mean.htm \
help/fr/nanstdev.htm \
help/fr/nanmean.htm \
help/fr/nanmin.htm \
help/fr/nanmedian.htm \
help/fr/thrownan.htm \
help/fr/nansum.htm \
help/fr/nfreq.htm \
help/fr/pca.htm \
help/fr/perctl.htm \
help/fr/quart.htm \
help/fr/regress.htm \
help/fr/samplef.htm \
help/fr/sample.htm \
help/fr/samwr.htm \
help/fr/stdevf.htm \
help/fr/strange.htm \
help/fr/tabul.htm \
help/fr/st_deviation.htm \
help/fr/trimmean.htm \
help/fr/variancef.htm \
help/fr/variance.htm \
help/fr/cdfbet.htm \
help/fr/cdfbin.htm \
help/fr/cdff.htm \
help/fr/whatis.htm \
help/fr/meanf.htm \
help/fr/center.htm \
help/fr/nancumsum.htm \
help/fr/centrered.htm \
help/fr/cdfchi.htm \
help/fr/cdfchn.htm \
help/fr/cdffnc.htm \
help/fr/cdfgam.htm \
help/fr/cdfnbn.htm \
help/fr/cdfnor.htm \
help/fr/cdfpoi.htm \
help/fr/cdft.htm \
help/fr/median.htm


#### statistics : documentation EN #### 
libstatistics_la_helpendir = $(target)/help/eng
libstatistics_la_helpen_DATA = help/eng/whatis.htm \
help/eng/labostat.htm \
help/eng/cdfbet.htm \
help/eng/cdfbin.htm \
help/eng/cdfchi.htm \
help/eng/cdfchn.htm \
help/eng/cdffnc.htm \
help/eng/cdff.htm \
help/eng/cdfgam.htm \
help/eng/cdfnbn.htm \
help/eng/cdfnor.htm \
help/eng/cdfpoi.htm \
help/eng/cdft.htm \
help/eng/center.htm \
help/eng/cmoment.htm \
help/eng/centrered.htm \
help/eng/correl.htm \
help/eng/covar.htm \
help/eng/ftest.htm \
help/eng/ftuneq.htm \
help/eng/geomean.htm \
help/eng/harmean.htm \
help/eng/iqr.htm \
help/eng/mean.htm \
help/eng/mad.htm \
help/eng/nanmeanf.htm \
help/eng/meanf.htm \
help/eng/median.htm \
help/eng/moment.htm \
help/eng/msd.htm \
help/eng/mvvacov.htm \
help/eng/nanmax.htm \
help/eng/nancumsum.htm \
help/eng/nand2mean.htm \
help/eng/nanstdev.htm \
help/eng/nanmean.htm \
help/eng/nanmin.htm \
help/eng/nanmedian.htm \
help/eng/thrownan.htm \
help/eng/nansum.htm \
help/eng/nfreq.htm \
help/eng/pca.htm \
help/eng/perctl.htm \
help/eng/quart.htm \
help/eng/regress.htm \
help/eng/samplef.htm \
help/eng/sample.htm \
help/eng/samwr.htm \
help/eng/stdevf.htm \
help/eng/strange.htm \
help/eng/st_deviation.htm \
help/eng/tabul.htm \
help/eng/trimmean.htm \
help/eng/variancef.htm \
help/eng/variance.htm


#### statistics : include files ####
libstatistics_la_includedir=$(includedir)/statistics
libstatistics_la_include_HEADERS = includes/gw_statistics.h

