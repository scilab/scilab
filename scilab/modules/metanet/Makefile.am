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


libmetanet_la_CFLAGS=  -Iincludes/  -I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/libs/hashtable/

lib_LTLIBRARIES = libmetanet.la

libmetanet_la_SOURCES = $(METANET_C_SOURCES) $(METANET_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libmetanet_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libmalloc.la \
	$(top_builddir)/libs/hashtable/libhashtable.la \
	$(top_builddir)/libs/blas/libblas.la \
	$(top_builddir)/modules/intersci/libintersci.la \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la

libmetanet_la_PKGCONFIG = metanet.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/metanet


#### metanet : Conf files ####
libmetanet_la_rootdir = $(target)
libmetanet_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### metanet : Scilab macros ###
libmetanet_la_macrodir = $(target)/macros
libmetanet_la_macro_DATA22 = macros/ge_do_replot.sci \
macros/salesman.sci \
macros/strong_con_nodes.sci \
macros/perfect_match.sci \
macros/pipe_network.sci \
macros/ge_draw_loop_arcs.sci \
macros/add_edge.sci \
macros/adj_lists.sci \
macros/ge_check_before_quit.sci \
macros/min_lcost_cflow.sci \
macros/ge_drawarcs.sci \
macros/trans_closure.sci \
macros/ge_add_history.sci \
macros/graph_sum.sci \
macros/ge_do_shortestpath.sci \
macros/ge_node_fields.sci \
macros/find_path.sci \
macros/metanet.sci \
macros/ge_copy_region_to_cb.sci \
macros/contract_edge.sci \
macros/ge_do_undo.sci \
macros/ge_do_delete_region.sci \
macros/ge_do_move.sci \
macros/ge_do_default_names.sci \
macros/supernode.sci \
macros/ge_get_info.sci \
macros/splitfilepath.sci \
macros/min_weight_tree.sci \
macros/initial_editgraph_tables.sci \
macros/graph_diameter.sci \
macros/ge_check_names.sci \
macros/arc_number.sci \
macros/bandwr.sci \
macros/save_graph.sci \
macros/ge_do_zoom.sci \
macros/editgraph_ver.sci \
macros/is_connex.sci \
macros/ge_get_nodes_in_rect.sci \
macros/ge_create_menus.sci \
macros/ge_shortcut.sci \
macros/shortest_path.sci \
macros/graph_simp.sci \
macros/ge_get_new_window.sci \
macros/make_graph.sci \
macros/check_graph.sci \
macros/ge_getnode.sci \
macros/ge_help.sci \
macros/ge_drawobj.sci \
macros/ge_arc_fields.sci \
macros/nodes_degrees.sci \
macros/subgraph.sci \
macros/ge_graph.sci \
macros/editgraph_node.sci \
macros/netclose.sci \
macros/split_edge.sci \
macros/neighbors.sci \
macros/ge_font.sci \
macros/ge_do_version.sci \
macros/ge_clear_history.sci \
macros/chain_struct.sci \
macros/ge_do_new.sci \
macros/strong_connex.sci \
macros/delete_nodes.sci \
macros/ge_get_region.sci \
macros/add_node.sci \
macros/min_lcost_flow1.sci \
macros/netwindows.sci \
macros/predecessors.sci \
macros/nodes_2_path.sci \
macros/ge_add_node.sci \
macros/ge_enablemenus.sci \
macros/best_match.sci \
macros/ge_do_paste.sci \
macros/max_flow.sci \
macros/edge_number.sci \
macros/ge_get_arcs_id.sci \
macros/ge_getarc.sci \
macros/ge_hilite_nodes.sci \
macros/ge_do_information.sci \
macros/graph_2_mat.sci \
macros/ge_getobj.sci \
macros/show_arcs.sci \
macros/ge_set_arcs_id.sci \
macros/editgraph_diagram.sci \
macros/graph_center.sci \
macros/ge_add_node_copy.sci \
macros/ge_delete_arc.sci \
macros/articul.sci \
macros/ge_show_new.sci \
macros/ge_dig_bound.sci \
macros/path_2_nodes.sci \
macros/ge_complete_defaults.sci \
macros/ge_do_load.sci \
macros/ge_do_export.sci \
macros/netwindow.sci \
macros/ge_do_SaveAs.sci \
macros/ge_do_quit.sci \
macros/cycle_basis.sci \
macros/mesh2d.sci \
macros/ge_delete_node.sci \
macros/graph_complement.sci \
macros/ge_add_arc.sci \
macros/ge_do_options.sci \
macros/ge_eventhandler.sci \
macros/connex.sci \
macros/knapsack.sci \
macros/successors.sci \
macros/ge_do_settings.sci \
macros/node_number.sci \
macros/load_graph.sci \
macros/ge_drawnodes.sci \
macros/show_graph.sci \
macros/circuit.sci \
macros/ge_do_copy.sci \
macros/graph_union.sci \
macros/ge_set_nodes_id.sci \
macros/delete_arcs.sci \
macros/ge_set_winsize.sci \
macros/dist2polylines.sci \
macros/ge_drawobjs.sci \
macros/ge_do_properties.sci \
macros/edit_graph.sci \
macros/ge_do_move_region.sci \
macros/con_nodes.sci \
macros/line_graph.sci \
macros/ge_do_region2node.sci \
macros/ge_drawtitle.sci \
macros/ge_arc_coordinates.sci \
macros/ge_do_copy_region.sci \
macros/ge_do_select.sci \
macros/graph_power.sci \
macros/girth.sci \
macros/gen_net.sci \
macros/message.sci \
macros/arc_graph.sci \
macros/convex_hull.sci \
macros/ge_disablemenus.sci \
macros/ge_compute.sci \
macros/ge_draw_std_arcs.sci \
macros/ge_do_save.sci \
macros/min_lcost_flow2.sci \
macros/ge_hilite_arcs.sci \
macros/qassign.sci \
macros/show_nodes.sci \
macros/hamilton.sci \
macros/ge_do_find.sci \
macros/min_qcost_flow.sci \
macros/mat_2_graph.sci \
macros/max_cap_path.sci \
macros/max_clique.sci \
macros/ge_newarc.sci \
macros/editgraph_arc.sci \
macros/ge_do_delete.sci \
macros/plot_graph.sci \
macros/ge_edit.sci macros/add_edge.bin \
macros/add_node.bin \
macros/adj_lists.bin \
macros/arc_graph.bin \
macros/arc_number.bin \
macros/articul.bin \
macros/bandwr.bin \
macros/best_match.bin \
macros/chain_struct.bin \
macros/check_graph.bin \
macros/circuit.bin \
macros/connex.bin \
macros/con_nodes.bin \
macros/contract_edge.bin \
macros/convex_hull.bin \
macros/cycle_basis.bin \
macros/delete_arcs.bin \
macros/delete_nodes.bin \
macros/dist2polylines.bin \
macros/edge_number.bin \
macros/editgraph_arc.bin \
macros/editgraph_diagram.bin \
macros/editgraph_node.bin \
macros/edit_graph.bin \
macros/editgraph_ver.bin \
macros/find_path.bin \
macros/ge_add_arc.bin \
macros/ge_add_history.bin \
macros/ge_add_node_copy.bin \
macros/ge_add_node.bin \
macros/ge_arc_coordinates.bin \
macros/ge_arc_fields.bin \
macros/ge_check_before_quit.bin \
macros/ge_check_names.bin \
macros/ge_clear_history.bin \
macros/ge_complete_defaults.bin \
macros/ge_compute.bin \
macros/ge_copy_region_to_cb.bin \
macros/ge_create_menus.bin \
macros/ge_delete_arc.bin \
macros/ge_delete_node.bin \
macros/ge_dig_bound.bin \
macros/ge_disablemenus.bin \
macros/ge_do_copy_region.bin \
macros/ge_do_copy.bin \
macros/ge_do_default_names.bin \
macros/ge_do_delete_region.bin \
macros/ge_do_delete.bin \
macros/ge_do_export.bin \
macros/ge_do_find.bin \
macros/ge_do_information.bin \
macros/ge_do_load.bin \
macros/ge_do_move_region.bin \
macros/ge_do_move.bin \
macros/ge_do_new.bin \
macros/ge_do_options.bin \
macros/ge_do_paste.bin \
macros/ge_do_properties.bin \
macros/ge_do_quit.bin \
macros/ge_do_region2node.bin \
macros/ge_do_replot.bin \
macros/ge_do_SaveAs.bin \
macros/ge_do_save.bin \
macros/ge_do_select.bin \
macros/ge_do_settings.bin \
macros/ge_do_shortestpath.bin \
macros/ge_do_undo.bin \
macros/ge_do_version.bin \
macros/ge_do_zoom.bin \
macros/ge_drawarcs.bin \
macros/ge_draw_loop_arcs.bin \
macros/ge_drawnodes.bin \
macros/ge_drawobj.bin \
macros/ge_drawobjs.bin \
macros/ge_draw_std_arcs.bin \
macros/ge_drawtitle.bin \
macros/ge_edit.bin \
macros/ge_enablemenus.bin \
macros/ge_eventhandler.bin \
macros/ge_font.bin \
macros/ge_getarc.bin \
macros/ge_get_arcs_id.bin \
macros/ge_get_info.bin \
macros/ge_get_new_window.bin \
macros/ge_getnode.bin \
macros/ge_get_nodes_in_rect.bin \
macros/ge_getobj.bin \
macros/ge_get_region.bin \
macros/ge_graph.bin \
macros/ge_help.bin \
macros/ge_hilite_arcs.bin \
macros/ge_hilite_nodes.bin \
macros/ge_newarc.bin \
macros/gen_net.bin \
macros/ge_node_fields.bin \
macros/ge_set_arcs_id.bin \
macros/ge_set_nodes_id.bin \
macros/ge_set_winsize.bin \
macros/ge_shortcut.bin \
macros/ge_show_new.bin \
macros/girth.bin \
macros/graph_2_mat.bin \
macros/graph_center.bin \
macros/graph_complement.bin \
macros/graph_diameter.bin \
macros/graph_power.bin \
macros/graph_simp.bin \
macros/graph_sum.bin \
macros/graph_union.bin \
macros/hamilton.bin \
macros/initial_editgraph_tables.bin \
macros/is_connex.bin \
macros/knapsack.bin \
macros/line_graph.bin \
macros/load_graph.bin \
macros/make_graph.bin \
macros/mat_2_graph.bin \
macros/max_cap_path.bin \
macros/max_clique.bin \
macros/max_flow.bin \
macros/mesh2d.bin \
macros/message.bin \
macros/metanet.bin \
macros/min_lcost_cflow.bin \
macros/min_lcost_flow1.bin \
macros/min_lcost_flow2.bin \
macros/min_qcost_flow.bin \
macros/min_weight_tree.bin \
macros/neighbors.bin \
macros/netclose.bin \
macros/netwindow.bin \
macros/netwindows.bin \
macros/node_number.bin \
macros/nodes_2_path.bin \
macros/nodes_degrees.bin \
macros/path_2_nodes.bin \
macros/perfect_match.bin \
macros/pipe_network.bin \
macros/plot_graph.bin \
macros/predecessors.bin \
macros/qassign.bin \
macros/salesman.bin \
macros/save_graph.bin \
macros/shortest_path.bin \
macros/show_arcs.bin \
macros/show_graph.bin \
macros/show_nodes.bin \
macros/split_edge.bin \
macros/splitfilepath.bin \
macros/strong_connex.bin \
macros/strong_con_nodes.bin \
macros/subgraph.bin \
macros/successors.bin \
macros/supernode.bin \
macros/trans_closure.bin macros/names macros/lib macros/buildmacros.sce


#### metanet : init scripts #### 
libmetanet_la_etcdir = $(target)/etc
libmetanet_la_etc_DATA = etc/metanet.quit etc/metanet.start

#### metanet : gateway declaration #### 
libmetanet_la_sci_gatewaydir = $(target)/sci_gateway
libmetanet_la_sci_gateway_DATA = sci_gateway/metanet

#### metanet : include files ####
libmetanet_la_includedir=$(includedir)/metanet
libmetanet_la_include_HEADERS = includes/gw_metanet.h


#### metanet : demos files ####
libmetanet_la_demosdir=$(target)/demos/
libmetanet_la_demos_DATA = demos/foret.graph \
demos/toto.graph \
demos/colored.graph \
demos/icon.graph \
demos/cross.graph \
demos/ra8.graph \
demos/meta.dem \
demos/ra9.graph \
demos/mesh1000.graph \
demos/mesh10.graph \
demos/paris2.graph \
demos/ra10.graph \
demos/metro.sci \
demos/umin.graph \
demos/node2station \
demos/mesh100.graph \
demos/paris.graph

include $(top_srcdir)/Makefile.incl.am
