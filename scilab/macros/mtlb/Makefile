SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = mtlblib
NAM = SCI/macros/mtlb

MTLB = mtlb.sci mtlb_e.sci mtlb_i.sci mtlb_is.sci mtlb_eval.sci \
	mtlb_max.sci mtlb_min.sci \
	mtlb_ones.sci mtlb_zeros.sci mtlb_eye.sci mtlb_rand.sci mtlb_length.sci\
	mtlb_fscanf.sci \
	mtlb_all.sci mtlb_any.sci mtlb_sum.sci mtlb_cumsum.sci \
	mtlb_prod.sci mtlb_cumprod.sci mtlb_median.sci mtlb_std.sci mtlb_mean.sci\
	mtlb_ifft.sci  mtlb_fft.sci mtlb_filter.sci mtlb_choices.sci  \
	mtlb_findstr.sci mtlb_exist.sci  mtlb_qz.sci \
	mtlb_load.sci mtlb_save.sci mtlb_fread.sci mtlb_fwrite.sci\
	fseek_origin.sci mtlb_sscanf.sci mtlb_fprintf.sci mtlb_sprintf.sci \
	mtlb_diff.sci mtlb_fliplr.sci mtlb_flipud.sci mtlb_isreal.sci \
        mtlb_find.sci mtlb_isa.sci\
	%s_m_b.sci %b_m_s.sci %s_x_b.sci %b_x_s.sci %b_g_s.sci %b_h_s.sci \
	%s_a_b.sci %b_a_s.sci %s_s_b.sci %b_s_s.sci \
	%b_c_s.sci %b_f_s.sci %s_c_b.sci %s_f_b.sci %s_g_b.sci %s_h_b.sci \
	%b_sum.sci %b_prod.sci %s_i_struct.sci 

M5 = mtlb_cell.sci struct.sci  \
	%s_i_struct.sci %b_i_struct.sci %i_i_struct.sci %sp_i_struct.sci \
	%c_i_struct.sci %s_i_struct.sci %cell_i_struct.sci %p_i_struct.sci \
	%l_i_struct.sci %struct_i_struct.sci %r_i_struct.sci %lss_i_struct.sci\
	%spb_i_struct.sci %mc_i_struct.sci %hm_i_struct.sci\
	%struct_p.sci %struct_e.sci %struct_string.sci\
        %cell_e.sci %cell_p.sci %cell_size.sci cell.sci \
	%b_i_cell.sci %cell_i_cell.sci %i_i_cell.sci %p_i_cell.sci  \
	%s_i_cell.sci %spb_i_cell.sci %c_i_cell.sci %hm_i_cell.sci \
	%lss_i_cell.sci  %r_i_cell.sci  %sp_i_cell.sci \
	%cell_c_cell.sci %cell_f_cell.sci \
	%cell_c_s.sci %cell_f_s.sci %s_f_cell.sci %s_c_cell.sci \
	%c_c_cell.sci %c_f_cell.sci %cell_f_c.sci %cell_c_c.sci \
	%i_c_cell.sci %i_f_cell.sci %cell_f_i.sci %cell_c_i.sci \
	%cell_string.sci 


PLOT= mtlb_plot.sci mtlb_subplot.sci mtlb_get.sci mtlb_clf.sci \
	mtlb_hold.sci mtlb_ishold.sci mtlb_mesh.sci mtlb_meshdom.sci \
	mtlb_semilogx.sci mtlb_semilogy.sci mtlb_loglog.sci \
	mtlb_image.sci mtlb_pcolor.sci 

UTIL=enlarge_shape.sci


MACROS =$(MTLB) $(M5) $(PLOT) $(UTIL)

include ../Make.lib

