##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

METANET_C_SOURCES = src/c/paths.c \
src/c/dmtree.c \
src/c/files.c \
src/c/myhsearch.c \
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

libmetanet_la_includedir=$(includedir)/metanet

libmetanet_la_include_HEADERS = includes/gw_metanet.h

libmetanet_la_CFLAGS=  -Iincludes/  -I$(top_srcdir)/libs/MALLOC/includes/

lib_LTLIBRARIES = libmetanet.la

libmetanet_la_SOURCES = $(METANET_C_SOURCES) $(METANET_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libmetanet_la_DEPENDENCIES = \
	$(top_builddir)/libs/MALLOC/libmalloc.la \
	$(top_builddir)/libs/blas/libblas.la \
	$(top_builddir)/modules/intersci/libintersci.la \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la

libmetanet_la_LIBADD = $(libmetanet_la_DEPENDENCIES)

libmetanet_la_PKGCONFIG = metanet.pc
