#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = autolib
NAM = SCI/macros/auto

MACROS = calfrq.sci svplot.sci abcd.sci sm2ss.sci sm2des.sci \
	arhnk.sci balreal.sci observer.sci stabil.sci \
	canon.sci cont_frm.sci lyap.sci sylv.sci \
	cont_mat.sci contrss.sci unobs.sci csim.sci ctr_gram.sci \
	cls2dls.sci dscr.sci dsimul.sci dt_ility.sci \
	equil.sci equil1.sci flts.sci \
	frep2tf.sci freson.sci g_margin.sci gfrancis.sci \
	imrep2ss.sci invsyslin.sci ddp.sci ui_observer.sci \
	kpure.sci krac2.sci invrs.sci sysfact.sci \
	lqe.sci lqr.sci markp2ss.sci minreal.sci \
	minss.sci obs_gram.sci obsv_mat.sci obsvss.sci \
	p_margin.sci pfss.sci \
	phasemag.sci projsl.sci des2tf.sci repfreq.sci ss2tf.sci ss2des.sci \
	ss2ss.sci  st_ility.sci statgain.sci lqg.sci lqg2stan.sci \
	obscont.sci tf2ss.sci trzeros.sci colregul.sci rowregul.sci \
	time_id.sci zeropen.sci specfact.sci abinv.sci cainv.sci dbphi.sci \
	bilin.sci arl2.sci \
	scicos.sci lincos.sci steadycos.sci scicos_simulate.sci \
	findR.sci findAC.sci findABCD.sci findBDK.sci findx0BD.sci inistate.sci

include ../Make.lib.mak
