#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME=m2scilib
NAM= SCI/macros/m2sci

M2SCI = cla2sci.sci cod2sci.sci exp2sci.sci func2sci.sci \
	get2sci.sci isacomment.sci \
	getlocal.sci indentsci.sci ins2sci.sci isnum.sci m2sci.sci\
	lhsargs.sci rhsargs.sci askfortype.sci num2sci.sci killfuns.sci\
	warning.sci sciparam.sci gettempvar.sci updatevtps.sci \
	isname.sci linetype.sci sci_gener.sci  isinstring.sci isanmfile.sci \
	lhsvarsnames.sci mfile_path.sci readmfile.sci checkdims.sci \
	set_infos.sci get_unknown.sci m2sci_syntax.sci known_untranslated.sci

FUN=	sci_eig.sci  sci_hess.sci sci_schur.sci sci_chol.sci sci_lu.sci\
	sci_qr.sci sci_balance.sci \
	sci_length.sci sci_size.sci \
	sci_ones.sci sci_eye.sci sci_rand.sci sci_randn.sci sci_zeros.sci \
	sci_sum.sci sci_cumsum.sci sci_prod.sci sci_cumprod.sci\
	sci_mean.sci sci_median.sci sci_std.sci\
	sci_svd.sci sci_det.sci sci_cond.sci sci_rcond.sci sci_norm.sci \
	sci_rank.sci sci_null.sci sci_pinv.sci \
	sci_diag.sci sci_triu.sci sci_tril.sci \
	sci_max.sci  sci_min.sci sci_maxi.sci  sci_mini.sci sci_any.sci \
	sci_all.sci sci_error.sci sci_load.sci sci_setstr.sci \
	sci_comment.sci sci_int2str.sci \
	sci_sin.sci sci_cos.sci sci_asin.sci sci_acos.sci sci_tan.sci \
	sci_atan.sci sci_atan2.sci sci_exp.sci sci_expm.sci \
	sci_log.sci sci_logm.sci sci_log10.sci sci_log2.sci sci_sign.sci\
	sci_sinh.sci sci_cosh.sci sci_asinh.sci sci_acosh.sci \
	sci_abs.sci sci_real.sci sci_imag.sci sci_conj.sci sci_inv.sci\
	sci_sqrt.sci sci_sqrtm.sci sci_find.sci sci_kron.sci\
	sci_fix.sci sci_round.sci sci_ceil.sci sci_floor.sci sci_rem.sci \
	sci_full.sci sci_sparse.sci sci_strcmp.sci \
	sci_getenv.sci sci_unix.sci sci_disp.sci sci_clf.sci sci_clg.sci\
	sci_drawnow.sci sci_home.sci \
	sci_fft.sci sci_ifft.sci sci_filter.sci sci_conv.sci \
	sci_poly.sci sci_roots.sci sci_sort.sci \
	sci_reshape.sci sci_fliplr.sci sci_flipud.sci sci_rot90.sci\
	sci_input.sci sci_more.sci sci_feval.sci sci_eval.sci\
	sci_isstr.sci sci_isempty.sci sci_isinf.sci sci_isnan.sci \
	sci_toeplitz.sci \
	sci_linspace.sci sci_logspace.sci sci_num2str.sci \
	sci_format.sci sci_keyboard.sci sci_upper.sci sci_lower.sci \
	sci_global.sci sci_spline.sci sci_pause.sci \
	sci_quit.sci sci_exit.sci sci_cputime.sci sci_clear.sci\
	sci_besseli.sci sci_besselj.sci sci_besselk.sci sci_bessely.sci\
	sci_pwd.sci sci_fzero.sci sci_exist.sci sci_angle.sci sci_break.sci\
	sci_cd.sci sci_diary.sci sci_dir.sci sci_help.sci sci_finite.sci\
	sci_magic.sci sci_lookfor.sci sci_qz.sci sci_save.sci sci_who.sci \
	sci_pow2.sci sci_ss2tf.sci sci_tf2ss.sci sci_planerot.sci\
	sci_fopen.sci sci_fclose.sci sci_frewind.sci sci_fread.sci \
	sci_fseek.sci sci_ftell.sci sci_ferror.sci sci_nnz.sci\
	sci_tic.sci sci_toc.sci sci_gammaln.sci  sci_gamma.sci\
	sci_erf.sci sci_erfc.sci  sci_erfcx.sci sci_clc.sci sci_echo.sci\
	sci_isglobal.sci sci_str2num.sci sci_isspace.sci sci_isletter.sci\
	sci_uicontrol.sci sci_set.sci sci_get.sci sci_findstr.sci \
	sci_fprintf.sci sci_sprintf.sci sci_fscanf.sci sci_sscanf.sci\
	sci_date.sci sci_type.sci sci_diff.sci sci_logical.sci sci_islogical.sci\
	sci_fwrite.sci sci_pack.sci sci_whos.sci \
	sci_boxcar.sci sci_chebwin.sci sci_hamming.sci sci_hanning.sci \
	sci_kaiser.sci sci_mat_read.sci sci_quantile.sci sci_spec.sci \
	sci_triang.sci sci_fgetl.sci sci_dlsim.sci sci_ltitr.sci sci_ss.sci\
	sci_mod.sci


DLG=	sci_menu.sci sci_choices.sci

PLOT=	sci_plot.sci sci_line.sci sci_subplot.sci sci_loglog.sci  \
	sci_figure.sci sci_delete.sci sci_xlabel.sci sci_ylabel.sci \
	sci_title.sci sci_stem.sci sci_stairs.sci sci_colormap.sci sci_gcf.sci\
	sci_gca.sci \
	sci_text.sci sci_ginput.sci sci_hot.sci sci_gray.sci sci_cool.sci\
	sci_flag.sci sci_bone.sci sci_copper.sci sci_pink.sci sci_gtext.sci\
	sci_meshgrid.sci sci_ishold.sci sci_hold.sci sci_close.sci sci_mesh.sci\
	sci_meshdom.sci sci_semilogx.sci sci_semilogy.sci sci_quiver.sci \
	sci_contour.sci 

MISC=	sci_clock.sci


OP=	%m2sci.sci mmodlst.sci %a2sci.sci %r2sci.sci %c2sci.sci %e2sci.sci\
	%i2sci.sci %imp2sci.sci %log2sci.sci %s2sci.sci %t2sci.sci  \
	%g2sci.sci %h2sci.sci %j2sci.sci %02sci.sci %x2sci.sci %d2sci.sci \
	%p2sci.sci %l2sci.sci %q2sci.sci %52sci.sci

M5=	sci_struct.sci sci_fieldnames.sci sci_cell.sci sci_isreal.sci

MACROS = $(M2SCI) $(FUN) $(OP)  $(PLOT) $(DLG) $(MISC) $(M5)

include ../Make.lib.mak
