SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = percentlib
NAM = SCI/macros/percent

MACROS =%ar_p.sci %b_c_spb.sci %b_f_spb.sci %b_g_s.sci %b_g_spb.sci %b_h_s.sci \
	%b_h_spb.sci %c_e.sci %c_i_c.sci %c_i_lss.sci %c_i_r.sci \
	%c_n_l.sci %c_o_l.sci %choose.sci %l_n_c.sci %l_n_l.sci \
	%l_n_m.sci %l_n_p.sci %l_n_s.sci %l_o_c.sci %l_o_l.sci \
	%l_o_m.sci %l_o_p.sci %l_o_s.sci %lss_a_lss.sci %lss_a_p.sci \
	%lss_a_r.sci %lss_a_s.sci %lss_c_lss.sci %lss_c_p.sci \
	%lss_c_r.sci %lss_c_s.sci %lss_e.sci %lss_f_lss.sci \
	%lss_f_p.sci %lss_f_r.sci %lss_f_s.sci %lss_i_lss.sci \
	%lss_i_p.sci %lss_i_r.sci %lss_i_s.sci %lss_l_lss.sci \
	%lss_l_p.sci %lss_l_r.sci %lss_l_s.sci %lss_m_lss.sci \
	%lss_m_p.sci %lss_m_r.sci %lss_m_s.sci %lss_n_lss.sci \
	%lss_n_p.sci %lss_n_r.sci %lss_n_s.sci %lss_o_lss.sci \
	%lss_o_p.sci %lss_o_r.sci %lss_o_s.sci %lss_r_lss.sci \
	%lss_r_p.sci %lss_r_r.sci %lss_r_s.sci %lss_s.sci \
	%lss_s_lss.sci %lss_s_p.sci %lss_s_r.sci %lss_s_s.sci \
	%lss_t.sci %lss_v_lss.sci %lss_v_p.sci %lss_v_r.sci \
	%lss_v_s.sci %m_n_l.sci %m_o_l.sci \
	%p_a_lss.sci %p_a_r.sci %p_c_lss.sci %p_c_r.sci \
	%p_d_p.sci %p_d_r.sci %p_d_s.sci %p_e.sci %p_f_lss.sci \
	%p_f_r.sci %p_i_lss.sci %p_i_p.sci %p_i_r.sci %p_i_s.sci \
	%p_j_s.sci %p_k_p.sci %p_k_r.sci %p_k_s.sci %p_l_lss.sci \
	%p_l_p.sci %p_l_r.sci %p_l_s.sci %p_m_lss.sci %p_m_r.sci \
	%p_n_l.sci %p_n_lss.sci %p_n_r.sci %p_o_l.sci %p_o_lss.sci \
	%p_o_r.sci %p_p_s.sci %p_q_p.sci %p_q_r.sci %p_q_s.sci \
	%p_r_lss.sci %p_r_p.sci %p_r_r.sci %p_r_s.sci %p_s_lss.sci \
	%p_s_r.sci %p_v_lss.sci %p_v_p.sci %p_v_r.sci %p_v_s.sci \
	%p_x_r.sci %p_y_p.sci %p_y_r.sci %p_y_s.sci %p_z_p.sci \
	%p_z_r.sci %p_z_s.sci %r_a_lss.sci %r_a_p.sci %r_a_r.sci \
	%r_a_s.sci %r_c_lss.sci %r_c_p.sci %r_c_r.sci %r_c_s.sci \
	%r_d_p.sci %r_d_r.sci %r_d_s.sci %r_e.sci %r_f_lss.sci \
	%r_f_p.sci %r_f_r.sci %r_f_s.sci %r_i_lss.sci %r_i_p.sci \
	%r_i_r.sci %r_i_s.sci %r_j_s.sci %r_k_p.sci %r_k_r.sci \
	%r_k_s.sci %r_l_lss.sci %r_l_p.sci %r_l_r.sci %r_l_s.sci \
	%r_m_lss.sci %r_m_p.sci %r_m_r.sci %r_m_s.sci %r_n_lss.sci \
	%r_n_p.sci %r_n_r.sci %r_n_s.sci %r_o_lss.sci %r_o_p.sci \
	%r_o_r.sci %r_o_s.sci %r_p.sci %r_p_s.sci %r_q_p.sci %r_q_r.sci \
	%r_q_s.sci %r_r_lss.sci %r_r_p.sci %r_r_r.sci %r_r_s.sci \
	%r_s.sci %r_s_lss.sci %r_s_p.sci %r_s_r.sci %r_s_s.sci %r_t.sci \
	%r_v_lss.sci %r_v_p.sci %r_v_r.sci %r_v_s.sci %r_x_p.sci \
	%r_x_r.sci %r_x_s.sci %r_y_p.sci %r_y_r.sci %r_y_s.sci %r_z_p.sci \
	%r_z_r.sci %r_z_s.sci %s_5.sci %s_a_lss.sci %s_a_r.sci \
	%s_a_sp.sci %s_c_lss.sci %s_c_r.sci %s_c_sp.sci %s_d_p.sci \
	%s_d_r.sci %s_d_sp.sci %s_e.sci %s_f_lss.sci %s_f_r.sci \
	%s_f_sp.sci %s_g_b.sci %s_g_s.sci %s_h_b.sci %s_h_s.sci \
	%s_i_b.sci %s_i_c.sci %s_i_lss.sci %s_i_p.sci %s_i_r.sci \
	%s_i_sp.sci %s_k_p.sci %s_k_r.sci %s_k_sp.sci \
	%s_l_lss.sci %s_l_p.sci %s_l_r.sci %s_l_sp.sci %s_m_lss.sci \
	%s_m_r.sci %s_n_l.sci %s_n_lss.sci %s_n_r.sci %s_o_l.sci \
	%s_o_lss.sci %s_o_r.sci %s_q_p.sci %s_q_r.sci %s_q_sp.sci \
	%s_r_lss.sci %s_r_p.sci %s_r_r.sci %s_r_sp.sci %s_s_lss.sci \
	%s_s_r.sci %s_s_sp.sci %s_v_lss.sci %s_v_p.sci %s_v_r.sci \
	%s_v_s.sci %s_x_r.sci %s_y_p.sci %s_y_r.sci %s_y_sp.sci \
	%s_z_p.sci %s_z_r.sci %s_z_sp.sci %sp_a_s.sci %sp_a_sp.sci \
	%sp_c_s.sci %sp_d_s.sci %sp_d_sp.sci %sp_e.sci %sp_f_s.sci \
	%sp_i_s.sci %sp_i_sp.sci %sp_k_s.sci %sp_k_sp.sci %sp_l_s.sci \
	%sp_l_sp.sci \
	%sp_q_s.sci %sp_q_sp.sci %sp_r_s.sci %sp_r_sp.sci %sp_s_s.sci \
	%sp_s_sp.sci %sp_y_s.sci %sp_y_sp.sci %sp_z_s.sci %sp_z_sp.sci \
	%spb_c_b.sci %spb_f_b.sci %spb_g_b.sci %spb_g_spb.sci %spb_h_b.sci \
	%spb_h_spb.sci  %spb_i_b.sci %spb_e.sci %b_i_spb.sci \
	%c_f_s.sci %b_i_s.sci %b_i_b.sci\
	%hm_a_hm.sci %hm_a_s.sci %hm_abs.sci %hm_c_hm.sci %hm_ceil.sci \
	%hm_conj.sci %hm_cos.sci %hm_d_hm.sci %hm_d_s.sci %hm_e.sci \
	%hm_exp.sci %hm_f_hm.sci %hm_floor.sci %hm_imag.sci %hm_int.sci \
	%hm_j_hm.sci %hm_j_s.sci %hm_log.sci %hm_m_s.sci %hm_ones.sci \
	%hm_p.sci %hm_q_hm.sci %hm_r_s.sci %hm_rand.sci %hm_real.sci \
	%hm_round.sci %hm_s.sci %hm_s_hm.sci %hm_sign.sci %hm_sin.sci \
	%hm_size.sci %hm_sqrt.sci %hm_x_hm.sci %hm_x_s.sci %s_i_hm.sci \
	%s_i_s.sci %s_l_hm.sci %s_m_hm.sci %s_q_hm.sci %s_x_hm.sci \
	%s_a_hm.sci %p_i_hm.sci %hm_i_hm.sci  \
	%hm_1_s.sci %hm_2_s.sci %hm_3_s.sci %hm_4_s.sci %hm_o_s.sci \
	%hm_n_s.sci %s_1_hm.sci %s_2_hm.sci %s_3_hm.sci %s_4_hm.sci \
	%s_o_hm.sci %s_n_hm.sci %hm_s_s.sci %s_s_hm.sci %hm_o_hm.sci \
	%hm_n_hm.sci \
	%hm_fft.sci %hm_sum.sci %hm_cumsum.sci %hm_prod.sci %hm_cumprod.sci\
	%hm_matrix.sci %hm_degree.sci %hm_maxi.sci %hm_mini.sci \
	%hm_mean.sci %hm_st_deviation.sci %hm_median.sci \
	%s_matrix.sci\
	%ip_a_s.sci %ip_s_s.sci %ip_m_s.sci %s_a_ip.sci %s_s_ip.sci \
	%s_m_ip.sci %ip_p.sci %s_i_spb.sci %b_i_sp.sci %c_a_c.sci %msp_p.sci\
	%msp_o_msp.sci %msp_n_msp.sci %ip_n_ip.sci %ip_o_ip.sci 

include ../Make.lib
