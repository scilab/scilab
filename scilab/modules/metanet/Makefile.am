##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

METANET_C_SOURCES = src/c/paths.c \
src/c/dmtree.c \
src/c/files.c \
src/c/hashtable_metanet.c \
src/c/saveg.c \
src/c/loadg.c \
src/c/connex.c \
src/c/transc.c

METANET_FORTRAN_SOURCES = src/fortran/visitor.f \
src/fortran/cfc.f \
src/fortran/bmatch.f \
src/fortran/compmat.f \
src/fortran/compc.f \
src/fortran/clique1.f \
src/fortran/arbor.f \
src/fortran/compfc.f \
src/fortran/busack.f \
src/fortran/diam.f \
src/fortran/dfs.f \
src/fortran/frmtrs.f \
src/fortran/getran.f \
src/fortran/mesh2b.f \
src/fortran/johns.f \
src/fortran/kiltq.f \
src/fortran/flomax.f \
src/fortran/floqua.f \
src/fortran/eclat.f \
src/fortran/l2que.f \
src/fortran/prim1.f \
src/fortran/cent.f \
src/fortran/hamil.f \
src/fortran/deumesh.f \
src/fortran/kilter.f \
src/fortran/permuto.f \
src/fortran/match.f \
src/fortran/pcchna.f \
src/fortran/ftrans.f \
src/fortran/frang.f \
src/fortran/dfs1.f \
src/fortran/dfs2.f \
src/fortran/bandred.f \
src/fortran/minty.f \
src/fortran/tconex.f \
src/fortran/carete.f \
src/fortran/prfmatch.f \
src/fortran/prim.f \
src/fortran/meshmesh.f \
src/fortran/chcm.f \
src/fortran/relax.f \
src/fortran/fordfulk.f \
src/fortran/seed.f \
src/fortran/mintyq.f \
src/fortran/clique.f \
src/fortran/hullcvex.f \
src/fortran/knapsk.f \
src/fortran/dijkst.f \
src/fortran/ford.f

GATEWAY_C_SOURCES = sci_gateway/c/gw_metanet.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_m6bandred.f \
sci_gateway/fortran/sci_m6bmatch.f \
sci_gateway/fortran/sci_m6busack.f \
sci_gateway/fortran/sci_m6cent.f \
sci_gateway/fortran/sci_m6chcm.f \
sci_gateway/fortran/sci_m6clique1.f \
sci_gateway/fortran/sci_m6clique.f \
sci_gateway/fortran/sci_m6compc.f \
sci_gateway/fortran/sci_m6compfc.f \
sci_gateway/fortran/sci_m6concom.f \
sci_gateway/fortran/sci_m6deumesh.f \
sci_gateway/fortran/sci_m6dfs2.f \
sci_gateway/fortran/sci_m6dfs.f \
sci_gateway/fortran/sci_m6diam.f \
sci_gateway/fortran/sci_m6dijkst.f \
sci_gateway/fortran/sci_m6dmtree.f \
sci_gateway/fortran/sci_m6edge2st.f \
sci_gateway/fortran/sci_m6findiso.f \
sci_gateway/fortran/sci_m6flomax.f \
sci_gateway/fortran/sci_m6floqua.f \
sci_gateway/fortran/sci_m6ford.f \
sci_gateway/fortran/sci_m6fordfulk.f \
sci_gateway/fortran/sci_m6frang.f \
sci_gateway/fortran/sci_m6hamil.f \
sci_gateway/fortran/sci_m6hullcvex.f \
sci_gateway/fortran/sci_m6johns.f \
sci_gateway/fortran/sci_m6kilter.f \
sci_gateway/fortran/sci_m6knapsk.f \
sci_gateway/fortran/sci_m6loadg.f \
sci_gateway/fortran/sci_m6lp2tad.f \
sci_gateway/fortran/sci_m6lp2tau.f \
sci_gateway/fortran/sci_m6mesh2b.f \
sci_gateway/fortran/sci_m6meshmesh.f \
sci_gateway/fortran/sci_m6ns2p.f \
sci_gateway/fortran/sci_m6p2ns.f \
sci_gateway/fortran/sci_m6pcchna.f \
sci_gateway/fortran/sci_m6permuto.f \
sci_gateway/fortran/sci_m6prevn2p.f \
sci_gateway/fortran/sci_m6prevn2st.f \
sci_gateway/fortran/sci_m6prfmatch.f \
sci_gateway/fortran/sci_m6relax.f \
sci_gateway/fortran/sci_m6saveg.f \
sci_gateway/fortran/sci_m6sconcom.f \
sci_gateway/fortran/sci_m6ta2lpd.f \
sci_gateway/fortran/sci_m6ta2lpu.f \
sci_gateway/fortran/sci_m6tconex.f \
sci_gateway/fortran/sci_m6transc.f \
sci_gateway/fortran/sci_m6umtree1.f \
sci_gateway/fortran/sci_m6umtree.f \
sci_gateway/fortran/sci_m6visitor.f

libscimetanet_la_CFLAGS=  	-I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/fileio/includes/ \
				-I$(top_srcdir)/modules/operations/includes/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/libs/hashtable/

pkglib_LTLIBRARIES = libscimetanet.la

libscimetanet_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscimetanet_la_SOURCES = $(METANET_C_SOURCES) $(METANET_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC= $(METANET_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscimetanet_la_CFLAGS)

libscimetanet_la_LIBADD = $(top_builddir)/libs/hashtable/libscihashtable.la $(top_builddir)/modules/intersci/libsciintersci.la $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la

#### Target ######
modulename=metanet


#### metanet : Conf files ####
libscimetanet_la_rootdir = $(mydatadir)
libscimetanet_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### metanet : init scripts ####
libscimetanet_la_etcdir = $(mydatadir)/etc
libscimetanet_la_etc_DATA = etc/metanet.quit etc/metanet.start

#### metanet : gateway declaration ####
libscimetanet_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscimetanet_la_sci_gateway_DATA = sci_gateway/metanet_gateway.xml

#### metanet : include files ####
libscimetanet_la_includedir=$(pkgincludedir)/metanet
libscimetanet_la_include_HEADERS = includes/gw_metanet.h

#### metanet : Macros ####
MACROSDIRSEXT =  macros/editor macros/graph_tools

include $(top_srcdir)/Makefile.incl.am
