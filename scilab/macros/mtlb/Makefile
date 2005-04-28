SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = mtlblib
NAM = SCI/macros/mtlb

MTLB = mtlb.sci mtlb_e.sci mtlb_i.sci mtlb_is.sci mtlb_eval.sci \
	mtlb_max.sci mtlb_min.sci \
	mtlb_ones.sci mtlb_zeros.sci mtlb_eye.sci mtlb_rand.sci \
	mtlb_fscanf.sci \
	mtlb_all.sci mtlb_any.sci mtlb_sum.sci mtlb_cumsum.sci \
	mtlb_prod.sci mtlb_cumprod.sci mtlb_median.sci mtlb_std.sci mtlb_mean.sci\
	mtlb_ifft.sci  mtlb_fft.sci mtlb_fftshift.sci mtlb_filter.sci mtlb_choices.sci  \
	mtlb_findstr.sci mtlb_exist.sci  mtlb_qz.sci \
	mtlb_load.sci mtlb_save.sci mtlb_fread.sci mtlb_fwrite.sci\
	fseek_origin.sci mtlb_sscanf.sci mtlb_fprintf.sci mtlb_sprintf.sci \
	mtlb_diff.sci mtlb_fliplr.sci \
        mtlb_find.sci mtlb_isa.sci\
	%s_m_b.sci %b_m_s.sci %s_x_b.sci %b_x_s.sci %b_g_s.sci %b_h_s.sci \
	%s_a_b.sci %b_a_s.sci %s_s_b.sci %b_s_s.sci \
	%b_c_s.sci %b_f_s.sci %s_c_b.sci %s_f_b.sci %s_g_b.sci %s_h_b.sci \
	%b_sum.sci %b_prod.sci \
	mtlb_0.sci mtlb_a.sci mtlb_diag.sci mtlb_double.sci mtlb_diag.sci \
	mtlb_dir.sci mtlb_e.sci mtlb_eig.sci mtlb_false.sci mtlb_fopen.sci mtlb_format.sci \
	mtlb_full.sci mtlb_imp.sci mtlb_int8.sci mtlb_int16.sci mtlb_int32.sci \
	mtlb_isfield.sci mtlb_isletter.sci mtlb_isspace.sci mtlb_linspace.sci \
	mtlb_logical.sci mtlb_logic.sci \
	mtlb_lower.sci mtlb_l.sci mtlb_more.sci mtlb_num2str.sci mtlb_qr.sci \
	mtlb_randn.sci mtlb_rcond.sci mtlb_repmat.sci mtlb_setstr.sci mtlb_size.sci \
	mtlb_sort.sci mtlb_s.sci mtlb_strcmp.sci mtlb_strcmpi.sci \
	mtlb_strfind.sci mtlb_strrep.sci mtlb_toeplitz.sci mtlb_tril.sci mtlb_triu.sci mtlb_true.sci mtlb_t.sci \
	mtlb_type.sci mtlb_uint8.sci mtlb_uint16.sci mtlb_uint32.sci mtlb_upper.sci \
	mtlb_delete.sci mtlb_beta.sci mtlb_conv.sci mtlb_dec2hex.sci \
	mtlb_echo.sci mtlb_realmin.sci mtlb_realmax.sci

M5 =    cell.sci struct.sci  \
	%ce_string.sci %st_string.sci generic_i_st.sci\
	%s_i_st.sci %b_i_st.sci %i_i_st.sci %sp_i_st.sci \
	%c_i_st.sci %ce_i_st.sci %p_i_st.sci \
	%l_i_st.sci %st_i_st.sci %lss_i_st.sci\
	%spb_i_st.sci %mc_i_st.sci %hm_i_st.sci\
	%fptr_i_st.sci %h_i_st.sci %ip_i_st.sci %msp_i_st.sci \
	%st_p.sci %st_e.sci %st_size.sci \
        %ce_e.sci %ce_p.sci %ce_size.sci  \
	%b_i_ce.sci %ce_i_ce.sci %i_i_ce.sci %p_i_ce.sci  \
	%s_i_ce.sci %spb_i_ce.sci %c_i_ce.sci %hm_i_ce.sci \
	%lss_i_ce.sci  %r_i_ce.sci  %sp_i_ce.sci \
	%ce_c_ce.sci %ce_f_ce.sci \
	createstruct.sci %st_6.sci %ce_6.sci \
	%st_i_b.sci  %st_i_fptr.sci  %st_i_i.sci   %st_i_msp.sci\
	%st_i_s.sci %st_i_spb.sci %st_i_c.sci %st_i_h.sci %st_i_ip.sci\
	%st_i_p.sci %st_i_sp.sci %st_i_lss.sci %st_i_r.sci st_i_generic.sci\
	 %st_t.sci %st_c_st.sci\
	%st_f_st.sci %ce_t.sci %ce_i_s.sci generic_i_ce.sci mtlb_axis.sci \
	mtlb_box.sci \
	%b_d_s.sci %b_l_s.sci %b_p_s.sci %b_r_s.sci %s_d_b.sci %s_l_b.sci \
	%s_p_b.sci %s_r_b.sci %msp_t.sci %msp_nnz.sci %msp_m_s.sci \
	%msp_find.sci sci_sparse.sci %msp_e.sci %s_a_msp.sci %msp_a_s.sci \
	%msp_abs.sci %msp_length.sci %msp_maxi.sci %msp_sparse.sci %s_m_msp.sci


PLOT=   mtlb_plot.sci mtlb_subplot.sci mtlb_get.sci mtlb_grid.sci \
	mtlb_hold.sci mtlb_mesh.sci mtlb_meshdom.sci \
	mtlb_semilogx.sci mtlb_semilogy.sci mtlb_loglog.sci \
	mtlb_image.sci mtlb_pcolor.sci mtlb_axes.sci mtlb_box.sci mtlb_colordef.sci \
	mtlb_close.sci

UTIL=enlarge_shape.sci makecell.sci firstnonsingleton.sci asciimat.sci mstr2sci.sci char.sci


MACROS =$(MTLB) $(M5) $(PLOT) $(UTIL)

include ../Make.lib

