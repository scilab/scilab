#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = percentlib
NAM = SCI/macros/percent

A =     %c_a_c.sci %hm_a_hm.sci %hm_a_s.sci %ip_a_s.sci \
	%lss_a_lss.sci %lss_a_p.sci %lss_a_r.sci %lss_a_s.sci \
	%p_a_lss.sci %p_a_r.sci %r_a_lss.sci %r_a_p.sci \
	%r_a_r.sci %r_a_s.sci %s_a_hm.sci %s_a_ip.sci \
	%s_a_lss.sci %s_a_r.sci %s_a_sp.sci %sp_a_s.sci \
	%sp_a_sp.sci %i_a_i.sci 

B =      %b_i_b.sci %s_g_b.sci %s_h_b.sci %s_i_b.sci \
	 %spb_c_b.sci %spb_f_b.sci %spb_g_b.sci %spb_h_b.sci \
	 %spb_i_b.sci %c_b_c.sci %c_b_s.sci

C =      %b_c_spb.sci %c_i_c.sci %hm_c_hm.sci %l_n_c.sci \
	 %l_o_c.sci %lss_c_lss.sci %lss_c_p.sci %lss_c_r.sci \
	 %lss_c_s.sci %p_c_lss.sci %p_c_r.sci %r_c_lss.sci \
	 %r_c_p.sci %r_c_r.sci %r_c_s.sci %s_c_lss.sci \
	 %s_c_r.sci %s_c_sp.sci %s_i_c.sci %sp_c_s.sci \
	 %cblock_c_cblock.sci %s_c_cblock.sci \
	 %cblock_c_s.sci 

D =      %hm_d_hm.sci %hm_d_s.sci %p_d_p.sci %p_d_r.sci \
	 %p_d_s.sci %r_d_p.sci %r_d_r.sci %r_d_s.sci %s_d_p.sci \
	 %s_d_r.sci %s_d_sp.sci %sp_d_s.sci %sp_d_sp.sci %i_d_i.sci

E =      %hm_e.sci %lss_e.sci %p_e.sci %r_e.sci %s_e.sci \
	 %sp_e.sci %spb_e.sci %cblock_e.sci %h_e.sci

F =      %b_f_spb.sci %c_f_s.sci %hm_f_hm.sci %lss_f_lss.sci \
	 %lss_f_p.sci %lss_f_r.sci %lss_f_s.sci %p_f_lss.sci \
	 %p_f_r.sci %r_f_lss.sci %r_f_p.sci %r_f_r.sci \
	 %r_f_s.sci %s_f_lss.sci %s_f_r.sci %s_f_sp.sci \
	 %sp_f_s.sci  %cblock_f_cblock.sci \
	 %s_f_cblock.sci

G =      %b_g_s.sci %b_g_spb.sci %s_g_s.sci %spb_g_spb.sci \
	 %i_g_i.sci

H =      %b_h_s.sci %b_h_spb.sci %s_h_s.sci %spb_h_spb.sci \
	 %i_h_i.sci 

I =      %b_i_s.sci %b_i_sp.sci %b_i_spb.sci %c_i_lss.sci \
	 %c_i_r.sci %hm_i_hm.sci %lss_i_lss.sci %lss_i_p.sci \
	 %lss_i_r.sci %lss_i_s.sci %p_i_hm.sci %p_i_lss.sci \
	 %p_i_p.sci %p_i_r.sci %p_i_s.sci %r_i_lss.sci \
	 %r_i_p.sci %r_i_r.sci %r_i_s.sci %s_i_hm.sci \
	 %s_i_lss.sci %s_i_p.sci %s_i_r.sci %s_i_s.sci \
	 %s_i_sp.sci %s_i_spb.sci %sp_i_s.sci %sp_i_sp.sci \
	 %i_i_hm.sci %i_i_s.sci %b_i_hm.sci %s_i_h.sci %c_i_h.sci \
	 %i_i_i.sci %hm_i_s.sci %hm_i_p.sci %hm_i_i.sci %hm_i_b.sci \
	 %c_i_s.sci %mc_i_s.sci %l_i_s.sci %lt_i_s.sci generic_i_s.sci \
	 %3d_i_h.sci %ticks_i_h.sci %champdat_i_h.sci %grayplot_i_h.sci

J =      %hm_j_hm.sci %hm_j_s.sci %p_j_s.sci %r_j_s.sci

K =      %p_k_p.sci %p_k_r.sci %p_k_s.sci %r_k_p.sci %r_k_r.sci \
	 %r_k_s.sci %s_k_p.sci %s_k_r.sci %s_k_sp.sci \
	 %sp_k_s.sci %sp_k_sp.sci %hm_k_s.sci %s_k_hm.sci %hm_k_hm.sci

L =      %c_n_l.sci %c_o_l.sci %l_n_l.sci %l_o_l.sci \
	 %lss_l_lss.sci %lss_l_p.sci %lss_l_r.sci %lss_l_s.sci \
	 %m_n_l.sci %m_o_l.sci %p_l_lss.sci %p_l_p.sci \
	 %p_l_r.sci %p_l_s.sci %p_n_l.sci %p_o_l.sci \
	 %r_l_lss.sci %r_l_p.sci %r_l_r.sci %r_l_s.sci \
	 %s_l_s.sci %s_l_hm.sci %s_l_lss.sci %s_l_p.sci %s_l_r.sci \
	 %s_l_sp.sci %s_n_l.sci %s_o_l.sci %sp_l_s.sci \
	 %sp_l_sp.sci

M =      %hm_m_s.sci %ip_m_s.sci %l_n_m.sci %l_o_m.sci \
	 %lss_m_lss.sci %lss_m_p.sci %lss_m_r.sci %lss_m_s.sci \
	 %p_m_lss.sci %p_m_r.sci %r_m_lss.sci %r_m_p.sci \
	 %r_m_r.sci %r_m_s.sci %s_m_hm.sci %s_m_ip.sci \
	 %s_m_lss.sci %s_m_r.sci %i_m_i.sci

N =      %hm_n_hm.sci %hm_n_s.sci %ip_n_ip.sci %l_n_p.sci \
	 %l_n_s.sci %lss_n_lss.sci %lss_n_p.sci %lss_n_r.sci \
	 %lss_n_s.sci %msp_n_msp.sci %p_n_lss.sci %p_n_r.sci \
	 %r_n_lss.sci %r_n_p.sci %r_n_r.sci %r_n_s.sci \
	 %s_n_hm.sci %s_n_lss.sci %s_n_r.sci

O =      %hm_o_hm.sci %hm_o_s.sci %hm_o_i.sci %ip_o_ip.sci %l_o_p.sci \
	 %l_o_s.sci %lss_o_lss.sci %lss_o_p.sci %lss_o_r.sci \
	 %lss_o_s.sci %msp_o_msp.sci %p_o_lss.sci %p_o_r.sci \
	 %r_o_lss.sci %r_o_p.sci %r_o_r.sci %r_o_s.sci \
	 %s_o_hm.sci %s_o_lss.sci %s_o_r.sci

P =      %ar_p.sci %hm_p.sci %ip_p.sci %msp_p.sci  %r_p.sci  %h_p.sci\
	 %p_p_s.sci %r_p_s.sci  %dir_p.sci 

Q =      %hm_q_hm.sci %p_q_r.sci %p_q_s.sci %r_q_r.sci \
	 %r_q_s.sci %s_q_hm.sci %s_q_r.sci %s_q_sp.sci \
	 %sp_q_s.sci %sp_q_sp.sci %p_q_p.sci %r_q_p.sci %s_q_p.sci 

R =      %hm_r_s.sci %lss_r_lss.sci %lss_r_r.sci %lss_r_s.sci \
	 %lss_s_r.sci %lss_v_r.sci %p_r_lss.sci %p_r_r.sci \
	 %p_r_s.sci %p_s_r.sci %p_v_r.sci %p_x_r.sci %p_y_r.sci \
	 %p_z_r.sci %r_r_lss.sci %r_r_r.sci %r_r_s.sci \
	 %r_s_r.sci %r_v_r.sci %r_x_r.sci %r_y_r.sci %r_z_r.sci \
	 %s_r_s.sci %s_r_lss.sci %s_r_r.sci %s_r_sp.sci %s_s_r.sci \
	 %s_v_r.sci %s_x_r.sci %s_y_r.sci %s_z_r.sci %sp_r_s.sci \
	 %sp_r_sp.sci %i_r_i.sci %lss_r_p.sci %p_r_p.sci %r_r_p.sci \
	 %s_r_p.sci

S =      %hm_1_s.sci %hm_2_s.sci %hm_3_s.sci %hm_4_s.sci \
	 %hm_s.sci %hm_s_hm.sci %hm_s_s.sci %hm_x_s.sci \
	 %ip_s_s.sci %lss_s.sci %lss_s_lss.sci %lss_s_s.sci \
	 %lss_v_s.sci %p_s_lss.sci %p_v_s.sci %p_y_s.sci \
	 %p_z_s.sci %r_s.sci %r_s_lss.sci %r_s_s.sci %r_v_s.sci \
	 %r_x_s.sci %r_y_s.sci %r_z_s.sci %s_s_hm.sci \
	 %s_s_ip.sci %s_s_lss.sci %s_s_sp.sci %s_v_s.sci \
	 %sp_s_s.sci %sp_s_sp.sci %sp_y_s.sci %sp_z_s.sci \
	 %i_s_i.sci %lss_s_p.sci %r_s_p.sci  

T =      %lss_t.sci %r_t.sci

U =     

V =      %lss_v_lss.sci %p_v_lss.sci %r_v_lss.sci %s_v_lss.sci \
	 %lss_v_p.sci %p_v_p.sci %r_v_p.sci %s_v_p.sci 

W =     

X =      %hm_x_hm.sci %s_x_hm.sci %i_x_i.sci %r_x_p.sci

Y =      %s_y_sp.sci %sp_y_sp.sci %p_y_p.sci %r_y_p.sci %s_y_p.sci

Z =      %s_z_sp.sci %sp_z_sp.sci %r_z_p.sci %s_z_p.sci %p_z_p.sci 

O0 =     

O1 =      %s_1_hm.sci %hm_1_hm.sci

O2 =      %s_2_hm.sci %hm_2_hm.sci

O3 =      %s_3_hm.sci %hm_3_hm.sci

O4 =      %s_4_hm.sci %hm_4_hm.sci

O5 =      %s_5.sci %hm_5.sci

HMFUNS = %hm_abs.sci %hm_ceil.sci %hm_conj.sci \
	 %hm_cos.sci %hm_cumprod.sci %hm_cumsum.sci \
	 %hm_degree.sci %hm_exp.sci %hm_fft.sci %hm_floor.sci \
	 %hm_imag.sci %hm_int.sci %hm_log.sci %hm_matrix.sci \
	 %hm_maxi.sci %hm_mean.sci %hm_median.sci %hm_mini.sci \
	 %hm_ones.sci %hm_prod.sci %hm_rand.sci %hm_real.sci \
	 %hm_round.sci %hm_sign.sci %hm_sin.sci %hm_size.sci \
	 %hm_sqrt.sci %hm_st_deviation.sci %hm_sum.sci \
	 %hm_zeros.sci %hm_bool2s.sci %hm_find.sci %hm_isnan.sci \
	 %hm_isreal.sci %hm_iconvert.sci %hm_and.sci  %hm_or.sci 

BFUNS =  %b_diag.sci %b_tril.sci %b_triu.sci %b_string.sci %b_iconvert.sci \
	%b_matrix.sci

CFUNS =  %c_diag.sci %c_eye.sci %c_mclose.sci %c_ones.sci %c_rand.sci \
	%c_tril.sci %c_triu.sci %c_matrix.sci

LSSFUNS= %lss_eye.sci %lss_inv.sci %lss_ones.sci %lss_rand.sci %lss_size.sci\
	 %lss_norm.sci

PFUNS =  %p_det.sci %p_inv.sci %p_prod.sci %p_simp.sci %p_string.sci %p_sum.sci \
	%p_matrix.sci

RFUNS =  %r_clean.sci %r_det.sci %r_diag.sci %r_eye.sci %r_inv.sci \
	 %r_ones.sci %r_rand.sci %r_simp.sci %r_size.sci %r_string.sci \
	 %r_tril.sci %r_triu.sci %r_norm.sci

SFUNS =  %s_pow.sci %s_simp.sci %s_sort.sci %s_and.sci %s_matrix.sci %s_or.sci

SPFUNS = %sp_ceil.sci %sp_cos.sci %sp_diag.sci %sp_exp.sci %sp_floor.sci \
	 %sp_int.sci %sp_inv.sci %sp_norm.sci \
	 %sp_round.sci %sp_sin.sci %sp_sort.sci %sp_sqrt.sci %sp_sum.sci \
	 %sp_tril.sci %sp_triu.sci %spb_diag.sci %spb_tril.sci %spb_triu.sci\
	 %sp_and.sci %sp_or.sci %spb_and.sci %spb_or.sci %sp_length.sci 

MSPFUNS = %msp_spones.sci %msp_i_s.sci

IFUNS =  %i_and.sci %i_or.sci %i_round.sci %i_prod.sci %i_length.sci %i_ascii.sci \
	%i_matrix.sci %i_bezout.sci %i_gcd.sci %i_lcm.sci

HFUNS = %h_delete.sci %h_draw.sci %h_get.sci %h_set.sci %h_matrix.sci

FUNS =   %cblock_p.sci %choose.sci %l_isequal.sci

XLSFUNS = %xls_p.sci %xlssheet_p.sci %xls_e.sci %xlssheet_e.sci %xlssheet_size.sci \
	 %xlssheet_string.sci 

MACROS =$(O0) $(O1) $(O2) $(O3) $(O4) $(O5) $(A) $(B) $(C) $(D) \
	$(E) $(F) $(G) $(H) $(I) $(J) $(K) $(L) $(M) $(N) \
	$(O) $(P) $(Q) $(R) $(S) $(T) $(U) $(V) $(W) $(X) $(Y) $(Z)\
	$(HMFUNS) $(BFUNS) $(CFUNS) $(LSSFUNS) $(PFUNS) $(RFUNS) \
	$(SFUNS) $(SPFUNS) $(MSPFUNS) $(IFUNS) $(FUNS) $(HFUNS) $(XLSFUNS)

include ../Make.lib.mak
