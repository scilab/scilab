#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = metalib
NAM = SCI/macros/metanet

EG = dist2polylines.sci edit_graph.sci editgraph_arc.sci editgraph_diagram.sci editgraph_node.sci \
	editgraph_ver.sci ge_add_arc.sci ge_add_history.sci ge_add_node.sci ge_add_node_copy.sci \
	ge_arc_coordinates.sci ge_arc_fields.sci ge_clear_history.sci ge_complete_defaults.sci \
	ge_compute.sci ge_copy_region_to_cb.sci ge_create_menus.sci ge_delete_arc.sci \
	ge_delete_node.sci ge_dig_bound.sci ge_disablemenus.sci ge_do_SaveAs.sci ge_do_copy.sci \
	ge_do_copy_region.sci ge_do_default_names.sci ge_do_settings.sci ge_do_delete.sci \
	ge_do_delete_region.sci ge_do_load.sci ge_do_move.sci ge_do_move_region.sci ge_do_new.sci \
	ge_do_options.sci ge_do_paste.sci ge_do_properties.sci ge_do_quit.sci ge_do_region2node.sci \
	ge_do_save.sci ge_do_select.sci ge_do_shortestpath.sci ge_do_undo.sci ge_do_version.sci \
	ge_do_zoom.sci ge_draw_loop_arcs.sci ge_draw_std_arcs.sci ge_drawarcs.sci ge_drawnodes.sci \
	ge_drawobj.sci ge_drawobjs.sci ge_drawtitle.sci ge_edit.sci ge_enablemenus.sci \
	ge_eventhandler.sci ge_font.sci ge_get_arcs_id.sci ge_get_info.sci ge_get_new_window.sci \
	ge_get_nodes_in_rect.sci ge_get_region.sci ge_getarc.sci ge_getnode.sci ge_getobj.sci \
	ge_graph.sci ge_hilite_arcs.sci ge_hilite_nodes.sci ge_newarc.sci ge_node_fields.sci \
	ge_set_winsize.sci ge_shortcut.sci ge_do_export.sci ge_check_names.sci ge_do_information.sci\
	ge_help.sci initial_editgraph_tables.sci  message.sci splitfilepath.sci \
	ge_check_before_quit.sci ge_do_find.sci ge_set_nodes_id.sci ge_set_arcs_id.sci 

MET = adj_lists.sci add_edge.sci add_node.sci arc_graph.sci \
	arc_number.sci articul.sci bandwr.sci \
	best_match.sci chain_struct.sci \
	check_graph.sci circuit.sci con_nodes.sci connex.sci \
	contract_edge.sci convex_hull.sci \
	cycle_basis.sci delete_arcs.sci delete_nodes.sci \
	edge_number.sci find_path.sci gen_net.sci girth.sci graph_2_mat.sci \
	graph_center.sci graph_complement.sci \
	graph_diameter.sci \
	graph_power.sci graph_simp.sci \
	graph_sum.sci graph_union.sci hamilton.sci knapsack.sci \
	nodes_degrees.sci \
	is_connex.sci line_graph.sci load_graph.sci make_graph.sci \
	mat_2_graph.sci max_cap_path.sci max_clique.sci max_flow.sci \
	mesh2d.sci metanet.sci min_lcost_cflow.sci \
	min_lcost_flow1.sci min_lcost_flow2.sci min_qcost_flow.sci \
	min_weight_tree.sci neighbors.sci node_number.sci \
	nodes_2_path.sci \
	path_2_nodes.sci perfect_match.sci pipe_network.sci plot_graph.sci \
	predecessors.sci qassign.sci \
	salesman.sci save_graph.sci shortest_path.sci \
	split_edge.sci \
	strong_con_nodes.sci strong_connex.sci subgraph.sci \
	successors.sci \
	supernode.sci trans_closure.sci \
	netclose.sci netwindow.sci netwindows.sci \
	show_arcs.sci show_graph.sci show_nodes.sci   
MACROS= $(EG) $(MET) 
include ../Make.lib.mak
