#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/calelm.lib

OBJSC = vfloor.obj vceil.obj  finite.obj vfrexp.obj sci_tools.obj vfinite.obj cmp.obj \
	someinterp.obj xerhlt.obj

SLATECEXT= dbesig.obj zbesig.obj dbeskg.obj zbeskg.obj dbesjg.obj zbesjg.obj dbesyg.obj zbesyg.obj  \
	zbeshg.obj 

SLATEC= d9knus.obj d9lgmc.obj dasyik.obj dbdiff.obj dbesi.obj dbesi0.obj dbesi1.obj \
	dbesk.obj dbesk0.obj dbesk1.obj dbesks.obj dbkias.obj dbkisr.obj dbsi0e.obj dbsi1e.obj \
	dbsk0e.obj dbsk1e.obj dbskes.obj dbskin.obj dbsknu.obj dcsevl.obj dexint.obj \
	dgamma.obj dgamrn.obj dhkseq.obj dlngam.obj dgamlm.obj dpsixn.obj fdump.obj initds.obj \
	j4save.obj xercnt.obj  xermsg.obj xerprn.obj xersve.obj xgetua.obj zbesi.obj \
	zbinu.obj zbesk.obj zacon.obj zbknu.obj zbunk.obj zuoik.obj zunhj.obj zunk1.obj \
	zunk2.obj zwrsk.obj zuchk.obj zs1s2.obj zmlri.obj zrati.obj zasyi.obj dgamln.obj \
	zshch.obj zseri.obj zkscl.obj zairy.obj zacai.obj zbuni.obj zuni2.obj zuni1.obj \
	zunik.obj zmlt.obj zdiv.obj zabs.obj zsqrt.obj zlog.obj zexp.obj dbesj.obj dbesy.obj \
	dbesj0.obj dbesj1.obj dbesy1.obj dbesy0.obj zbesj.obj zbesy.obj zbesh.obj d9b0mp.obj \
	d9b1mp.obj djairy.obj dyairy.obj dasyjy.obj dbsynu.obj dxlegf.obj i1mach.obj \
	d1mach.obj

OTHERS = coshin.obj arcosh.obj dad.obj dadd.obj ccopy.obj rcopy.obj ddif.obj \
	dmcopy.obj dmmul.obj dset.obj dsort.obj \
	dsum.obj  dtild.obj dvmul.obj entier.obj franck.obj gdcp2i.obj hilber.obj \
	icopy.obj  iset.obj isort.obj iwamax.obj kronc.obj kronr.obj magic.obj \
	mtran.obj pythag.obj  round.obj simple.obj urand.obj wasum.obj watan.obj \
	waxpy.obj wcopy.obj wdiv.obj wdotci.obj wdotcr.obj wlog.obj wmmul.obj wmul.obj \
	wnrm2.obj wrscal.obj wscal.obj wsign.obj wsqrt.obj wswap.obj wvmul.obj dipow.obj \
	ddpow.obj dwpow.obj wdpow.obj wipow.obj wwpow.obj int2db.obj rea2db.obj \
	isoval.obj isova0.obj imcopy.obj dlblks.obj rat.obj lnblnk.obj dmmul1.obj  \
	ddpowe.obj dipowe.obj dwpowe.obj wdpowe.obj wipowe.obj wwpowe.obj \
	ddpow1.obj dwpow1.obj wdpow1.obj wwpow1.obj \
	wwdiv.obj wddiv.obj dwdiv.obj ddrdiv.obj wwrdiv.obj dwrdiv.obj wdrdiv.obj \
	ivimp.obj intp.obj cusum.obj cupro.obj cuproi.obj rcsort.obj wsort.obj vpythag.obj \
	calerf.obj ribesl.obj rkbesl.obj rjbesl.obj rybesl.obj gamma.obj psi.obj dlgama.obj \
	dmsum.obj wmsum.obj dmprod.obj wmprod.obj drdiv.obj dcabs1.obj infinity.obj \
	unsfdcopy.obj idmax.obj idmin.obj wacos.obj wasin.obj  wtan.obj nearfloat.obj \
	dsearch.obj  somespline.obj cshep2d.obj dtensbs.obj \
	dspfit.obj pchim.obj scidcopy.obj
OBJSF=$(OTHERS) $(SLATEC) $(SLATECEXT)

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

finite.obj: ../machine.h

sci_tools.obj : sci_tools.h ../machine.h

