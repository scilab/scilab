#ifndef __SCICOS_BLOCKS__ 
#define __SCICOS_BLOCKS__ 
/******* Copyright INRIA *************/
/******* Please do not edit *************/
extern void F2C(intcos) __PARAMS((ARGS_scicos));
extern void F2C(coselm) __PARAMS((ARGS_scicos));
extern void F2C(evtdly) __PARAMS((ARGS_scicos));
extern void F2C(cstblk) __PARAMS((ARGS_scicos));
extern void F2C(lusat) __PARAMS((ARGS_scicos));
extern void F2C(pload) __PARAMS((ARGS_scicos));
extern void F2C(qzcel) __PARAMS((ARGS_scicos));
extern void F2C(qzflr) __PARAMS((ARGS_scicos));
extern void F2C(qzrnd) __PARAMS((ARGS_scicos));
extern void F2C(qztrn) __PARAMS((ARGS_scicos));
extern void F2C(scope) __PARAMS((ARGS_scicos));
extern void F2C(lsplit) __PARAMS((ARGS_scicos));
extern void F2C(csslti) __PARAMS((ARGS_scicos));
extern void F2C(dsslti) __PARAMS((ARGS_scicos));
extern void F2C(trash) __PARAMS((ARGS_scicos));
extern void F2C(zcross) __PARAMS((ARGS_scicos));
extern void F2C(expblk) __PARAMS((ARGS_scicos));
extern void F2C(logblk) __PARAMS((ARGS_scicos));
extern void F2C(sinblk) __PARAMS((ARGS_scicos));
extern void F2C(tanblk) __PARAMS((ARGS_scicos));
extern void F2C(powblk) __PARAMS((ARGS_scicos));
extern void F2C(sqrblk) __PARAMS((ARGS_scicos));
extern void F2C(delay) __PARAMS((ARGS_scicos));
extern void F2C(selblk) __PARAMS((ARGS_scicos));
extern void F2C(forblk) __PARAMS((ARGS_scicos));
extern void F2C(ifthel) __PARAMS((ARGS_scicos));
extern void F2C(writef) __PARAMS((ARGS_scicos));
extern void F2C(invblk) __PARAMS((ARGS_scicos));
extern void F2C(hltblk) __PARAMS((ARGS_scicos));
extern void F2C(gensin) __PARAMS((ARGS_scicos));
extern void F2C(rndblk) __PARAMS((ARGS_scicos));
extern void F2C(lookup) __PARAMS((ARGS_scicos));
extern void F2C(timblk) __PARAMS((ARGS_scicos));
extern void F2C(gensqr) __PARAMS((ARGS_scicos));
extern void F2C(mfclck) __PARAMS((ARGS_scicos));
extern void F2C(sawtth) __PARAMS((ARGS_scicos));
extern void F2C(tcslti) __PARAMS((ARGS_scicos));
extern void F2C(tcsltj) __PARAMS((ARGS_scicos));
extern void F2C(scopxy) __PARAMS((ARGS_scicos));
extern void F2C(evscpe) __PARAMS((ARGS_scicos));
extern void F2C(integr) __PARAMS((ARGS_scicos));
extern void F2C(readf) __PARAMS((ARGS_scicos));
extern void F2C(affich) __PARAMS((ARGS_scicos));
extern void F2C(intpol) __PARAMS((ARGS_scicos));
extern void F2C(intplt) __PARAMS((ARGS_scicos));
extern void F2C(minblk) __PARAMS((ARGS_scicos));
extern void F2C(maxblk) __PARAMS((ARGS_scicos));
extern void F2C(dlradp) __PARAMS((ARGS_scicos));
extern void F2C(iocopy) __PARAMS((ARGS_scicos));
extern void F2C(sum2) __PARAMS((ARGS_scicos));
extern void F2C(sum3) __PARAMS((ARGS_scicos));
extern void F2C(delayv) __PARAMS((ARGS_scicos));
extern void F2C(mux) __PARAMS((ARGS_scicos));
extern void F2C(demux) __PARAMS((ARGS_scicos));
extern void F2C(samphold) __PARAMS((ARGS_scicos));
extern void F2C(dollar) __PARAMS((ARGS_scicos));
extern void F2C(mscope) __PARAMS((ARGS_scicos));
extern void F2C(eselect) __PARAMS((ARGS_scicos));
extern void F2C(intrp2) __PARAMS((ARGS_scicos));
extern void F2C(intrpl) __PARAMS((ARGS_scicos));
extern void F2C(fsv) __PARAMS((ARGS_scicos));
extern void F2C(memo) __PARAMS((ARGS_scicos));
extern void F2C(fscope) __PARAMS((ARGS_scicos));
extern void F2C(scoxy) __PARAMS((ARGS_scicos));
extern void F2C(absblk) __PARAMS((ARGS_scicos));
extern void F2C(andlog) __PARAMS((ARGS_scicos));
extern void F2C(bidon) __PARAMS((ARGS_scicos));
extern void F2C(gain) __PARAMS((ARGS_scicos));
extern void F2C(cdummy) __PARAMS((ARGS_scicos));
extern void F2C(dband) __PARAMS((ARGS_scicos));
extern void F2C(cosblk) __PARAMS((ARGS_scicos));
extern void selector __PARAMS((ARGS_scicos));
extern void sum __PARAMS((ARGS_scicos));
extern void prod __PARAMS((ARGS_scicos));
extern void switchn __PARAMS((ARGS_scicos));
extern void relay __PARAMS((ARGS_scicos));
extern void readc __PARAMS((ARGS_scicos));
extern void writec __PARAMS((ARGS_scicos));
extern void writeau __PARAMS((ARGS_scicos));
extern void readau __PARAMS((ARGS_scicos));
extern void plusblk __PARAMS((ARGS_scicos));
 
OpTab tabsim[] ={
{"absblk",(ScicosF) F2C(absblk)},
{"affich",(ScicosF) F2C(affich)},
{"andlog",(ScicosF) F2C(andlog)},
{"bidon",(ScicosF) F2C(bidon)},
{"cdummy",(ScicosF) F2C(cdummy)},
{"cosblk",(ScicosF) F2C(cosblk)},
{"coselm",(ScicosF) F2C(coselm)},
{"csslti",(ScicosF) F2C(csslti)},
{"cstblk",(ScicosF) F2C(cstblk)},
{"dband",(ScicosF) F2C(dband)},
{"delay",(ScicosF) F2C(delay)},
{"delayv",(ScicosF) F2C(delayv)},
{"demux",(ScicosF) F2C(demux)},
{"dlradp",(ScicosF) F2C(dlradp)},
{"dollar",(ScicosF) F2C(dollar)},
{"dsslti",(ScicosF) F2C(dsslti)},
{"eselect",(ScicosF) F2C(eselect)},
{"evscpe",(ScicosF) F2C(evscpe)},
{"evtdly",(ScicosF) F2C(evtdly)},
{"expblk",(ScicosF) F2C(expblk)},
{"forblk",(ScicosF) F2C(forblk)},
{"fscope",(ScicosF) F2C(fscope)},
{"fsv",(ScicosF) F2C(fsv)},
{"gain",(ScicosF) F2C(gain)},
{"gensin",(ScicosF) F2C(gensin)},
{"gensqr",(ScicosF) F2C(gensqr)},
{"hltblk",(ScicosF) F2C(hltblk)},
{"ifthel",(ScicosF) F2C(ifthel)},
{"intcos",(ScicosF) F2C(intcos)},
{"integr",(ScicosF) F2C(integr)},
{"intplt",(ScicosF) F2C(intplt)},
{"intpol",(ScicosF) F2C(intpol)},
{"intrp2",(ScicosF) F2C(intrp2)},
{"intrpl",(ScicosF) F2C(intrpl)},
{"invblk",(ScicosF) F2C(invblk)},
{"iocopy",(ScicosF) F2C(iocopy)},
{"logblk",(ScicosF) F2C(logblk)},
{"lookup",(ScicosF) F2C(lookup)},
{"lsplit",(ScicosF) F2C(lsplit)},
{"lusat",(ScicosF) F2C(lusat)},
{"maxblk",(ScicosF) F2C(maxblk)},
{"memo",(ScicosF) F2C(memo)},
{"mfclck",(ScicosF) F2C(mfclck)},
{"minblk",(ScicosF) F2C(minblk)},
{"mscope",(ScicosF) F2C(mscope)},
{"mux",(ScicosF) F2C(mux)},
{"pload",(ScicosF) F2C(pload)},
{"plusblk",(ScicosF) plusblk},
{"powblk",(ScicosF) F2C(powblk)},
{"prod",(ScicosF) prod},
{"qzcel",(ScicosF) F2C(qzcel)},
{"qzflr",(ScicosF) F2C(qzflr)},
{"qzrnd",(ScicosF) F2C(qzrnd)},
{"qztrn",(ScicosF) F2C(qztrn)},
{"readau",(ScicosF) readau},
{"readc",(ScicosF) readc},
{"readf",(ScicosF) F2C(readf)},
{"relay",(ScicosF) relay},
{"rndblk",(ScicosF) F2C(rndblk)},
{"samphold",(ScicosF) F2C(samphold)},
{"sawtth",(ScicosF) F2C(sawtth)},
{"scope",(ScicosF) F2C(scope)},
{"scopxy",(ScicosF) F2C(scopxy)},
{"scoxy",(ScicosF) F2C(scoxy)},
{"selblk",(ScicosF) F2C(selblk)},
{"selector",(ScicosF) selector},
{"sinblk",(ScicosF) F2C(sinblk)},
{"sqrblk",(ScicosF) F2C(sqrblk)},
{"sum",(ScicosF) sum},
{"sum2",(ScicosF) F2C(sum2)},
{"sum3",(ScicosF) F2C(sum3)},
{"switchn",(ScicosF) switchn},
{"tanblk",(ScicosF) F2C(tanblk)},
{"tcslti",(ScicosF) F2C(tcslti)},
{"tcsltj",(ScicosF) F2C(tcsltj)},
{"timblk",(ScicosF) F2C(timblk)},
{"trash",(ScicosF) F2C(trash)},
{"writeau",(ScicosF) writeau},
{"writec",(ScicosF) writec},
{"writef",(ScicosF) F2C(writef)},
{"zcross",(ScicosF) F2C(zcross)},
{(char *) 0, (ScicosF) 0}};
 
int ntabsim= 81 ;
#endif 
/***********************************/
