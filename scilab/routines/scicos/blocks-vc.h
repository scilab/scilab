#ifndef __SCICOS_BLOCKS__ 
#define __SCICOS_BLOCKS__ 
/******* Copyright INRIA *************/
/******* Please do not edit *************/
extern void F2C(evtdly) (ARGS_scicos);
extern void F2C(cstblk) (ARGS_scicos);
extern void F2C(lusat) (ARGS_scicos);
extern void F2C(pload) (ARGS_scicos);
extern void F2C(qzcel) (ARGS_scicos);
extern void F2C(qzflr) (ARGS_scicos);
extern void F2C(qzrnd) (ARGS_scicos);
extern void F2C(qztrn) (ARGS_scicos);
extern void F2C(scope) (ARGS_scicos);
extern void F2C(lsplit) (ARGS_scicos);
extern void F2C(csslti) (ARGS_scicos);
extern void F2C(dsslti) (ARGS_scicos);
extern void F2C(trash) (ARGS_scicos);
extern void F2C(zcross) (ARGS_scicos);
extern void F2C(expblk) (ARGS_scicos);
extern void F2C(logblk) (ARGS_scicos);
extern void F2C(sinblk) (ARGS_scicos);
extern void F2C(tanblk) (ARGS_scicos);
extern void F2C(powblk) (ARGS_scicos);
extern void F2C(sqrblk) (ARGS_scicos);
extern void F2C(delay) (ARGS_scicos);
extern void F2C(selblk) (ARGS_scicos);
extern void F2C(forblk) (ARGS_scicos);
extern void F2C(writef) (ARGS_scicos);
extern void F2C(invblk) (ARGS_scicos);
extern void F2C(hltblk) (ARGS_scicos);
extern void F2C(gensin) (ARGS_scicos);
extern void F2C(rndblk) (ARGS_scicos);
extern void F2C(lookup) (ARGS_scicos);
extern void F2C(timblk) (ARGS_scicos);
extern void F2C(gensqr) (ARGS_scicos);
extern void F2C(mfclck) (ARGS_scicos);
extern void F2C(sawtth) (ARGS_scicos);
extern void F2C(tcslti) (ARGS_scicos);
extern void F2C(tcsltj) (ARGS_scicos);
extern void F2C(scopxy) (ARGS_scicos);
extern void F2C(evscpe) (ARGS_scicos);
extern void F2C(integr) (ARGS_scicos);
extern void F2C(readf) (ARGS_scicos);
extern void F2C(affich) (ARGS_scicos);
extern void F2C(intpol) (ARGS_scicos);
extern void F2C(intplt) (ARGS_scicos);
extern void F2C(minblk) (ARGS_scicos);
extern void F2C(maxblk) (ARGS_scicos);
extern void F2C(dlradp) (ARGS_scicos);
extern void F2C(iocopy) (ARGS_scicos);
extern void F2C(sum2) (ARGS_scicos);
extern void F2C(sum3) (ARGS_scicos);
extern void F2C(delayv) (ARGS_scicos);
extern void F2C(mux) (ARGS_scicos);
extern void F2C(demux) (ARGS_scicos);
extern void F2C(samphold) (ARGS_scicos);
extern void F2C(dollar) (ARGS_scicos);
extern void F2C(mscope) (ARGS_scicos);
extern void F2C(intrp2) (ARGS_scicos);
extern void F2C(intrpl) (ARGS_scicos);
extern void F2C(fsv) (ARGS_scicos);
extern void F2C(memo) (ARGS_scicos);
extern void F2C(fscope) (ARGS_scicos);
extern void F2C(scoxy) (ARGS_scicos);
extern void F2C(diffblk) (ARGS_scicos);
extern void F2C(constraint) (ARGS_scicos);
extern void F2C(absblk) (ARGS_scicos);
extern void F2C(andlog) (ARGS_scicos);
extern void F2C(bidon) (ARGS_scicos);
extern void F2C(gain) (ARGS_scicos);
extern void F2C(cdummy) (ARGS_scicos);
extern void F2C(dband) (ARGS_scicos);
extern void F2C(cosblk) (ARGS_scicos);
extern void F2C(ifthel) (ARGS_scicos);
extern void F2C(eselect) (ARGS_scicos);
extern void selector (ARGS_scicos);
extern void sum (ARGS_scicos);
extern void prod (ARGS_scicos);
extern void switchn (ARGS_scicos);
extern void relay (ARGS_scicos);
extern void readc (ARGS_scicos);
extern void writec (ARGS_scicos);
extern void writeau (ARGS_scicos);
extern void readau (ARGS_scicos);
extern void plusblk (ARGS_scicos);
extern void slider (ARGS_scicos);
extern void zcross2 (ARGS_scicos);
extern void mswitch (ARGS_scicos);
extern void logicalop (ARGS_scicos);
extern void switch2 (ARGS_scicos);
extern void variable_delay (ARGS_scicos);
extern void time_delay (ARGS_scicos);
extern void cscope (ARGS_scicos);
extern void cmscope (ARGS_scicos);
extern void satur (ARGS_scicos);
extern void step_func (ARGS_scicos);
extern void integral_func (ARGS_scicos);
extern void absolute_value (ARGS_scicos);
extern void bounce_ball (ARGS_scicos);
extern void bouncexy (ARGS_scicos);
extern void extractor (ARGS_scicos);
extern void scalar2vector (ARGS_scicos);
extern void minmax (ARGS_scicos);
extern void signum (ARGS_scicos);
extern void product (ARGS_scicos);
extern void summation (ARGS_scicos);
extern void multiplex (ARGS_scicos);
extern void gainblk (ARGS_scicos);
extern void relationalop (ARGS_scicos);
extern void modulo_count (ARGS_scicos);
extern void hystheresis (ARGS_scicos);
extern void ratelimiter (ARGS_scicos);
extern void backlash (ARGS_scicos);
extern void deadband (ARGS_scicos);
extern void ramp (ARGS_scicos);
extern void evaluate_expr (ARGS_scicos);
extern void deriv (ARGS_scicos);
extern void sin_blk (ARGS_scicos);
extern void cos_blk (ARGS_scicos);
extern void tan_blk (ARGS_scicos);
extern void asin_blk (ARGS_scicos);
extern void acos_blk (ARGS_scicos);
extern void atan_blk (ARGS_scicos);
extern void sinh_blk (ARGS_scicos);
extern void cosh_blk (ARGS_scicos);
extern void tanh_blk (ARGS_scicos);
extern void asinh_blk (ARGS_scicos);
extern void acosh_blk (ARGS_scicos);
extern void atanh_blk (ARGS_scicos);
extern void evtvardly (ARGS_scicos);
 
OpTab tabsim[] ={
{"absblk",(ScicosF) F2C(absblk)},
{"absolute_value",(ScicosF) absolute_value},
{"acos_blk",(ScicosF) acos_blk},
{"acosh_blk",(ScicosF) acosh_blk},
{"affich",(ScicosF) F2C(affich)},
{"andlog",(ScicosF) F2C(andlog)},
{"asin_blk",(ScicosF) asin_blk},
{"asinh_blk",(ScicosF) asinh_blk},
{"atan_blk",(ScicosF) atan_blk},
{"atanh_blk",(ScicosF) atanh_blk},
{"backlash",(ScicosF) backlash},
{"bidon",(ScicosF) F2C(bidon)},
{"bounce_ball",(ScicosF) bounce_ball},
{"bouncexy",(ScicosF) bouncexy},
{"cdummy",(ScicosF) F2C(cdummy)},
{"cmscope",(ScicosF) cmscope},
{"constraint",(ScicosF) F2C(constraint)},
{"cos_blk",(ScicosF) cos_blk},
{"cosblk",(ScicosF) F2C(cosblk)},
{"cosh_blk",(ScicosF) cosh_blk},
{"cscope",(ScicosF) cscope},
{"csslti",(ScicosF) F2C(csslti)},
{"cstblk",(ScicosF) F2C(cstblk)},
{"dband",(ScicosF) F2C(dband)},
{"deadband",(ScicosF) deadband},
{"delay",(ScicosF) F2C(delay)},
{"delayv",(ScicosF) F2C(delayv)},
{"demux",(ScicosF) F2C(demux)},
{"deriv",(ScicosF) deriv},
{"diffblk",(ScicosF) F2C(diffblk)},
{"dlradp",(ScicosF) F2C(dlradp)},
{"dollar",(ScicosF) F2C(dollar)},
{"dsslti",(ScicosF) F2C(dsslti)},
{"eselect",(ScicosF) F2C(eselect)},
{"evaluate_expr",(ScicosF) evaluate_expr},
{"evscpe",(ScicosF) F2C(evscpe)},
{"evtdly",(ScicosF) F2C(evtdly)},
{"evtvardly",(ScicosF) evtvardly},
{"expblk",(ScicosF) F2C(expblk)},
{"extractor",(ScicosF) extractor},
{"forblk",(ScicosF) F2C(forblk)},
{"fscope",(ScicosF) F2C(fscope)},
{"fsv",(ScicosF) F2C(fsv)},
{"gainblk",(ScicosF) gainblk},
{"gain",(ScicosF) F2C(gain)},
{"gensin",(ScicosF) F2C(gensin)},
{"gensqr",(ScicosF) F2C(gensqr)},
{"hltblk",(ScicosF) F2C(hltblk)},
{"hystheresis",(ScicosF) hystheresis},
{"ifthel",(ScicosF) F2C(ifthel)},
{"integral_func",(ScicosF) integral_func},
{"integr",(ScicosF) F2C(integr)},
{"intplt",(ScicosF) F2C(intplt)},
{"intpol",(ScicosF) F2C(intpol)},
{"intrp2",(ScicosF) F2C(intrp2)},
{"intrpl",(ScicosF) F2C(intrpl)},
{"invblk",(ScicosF) F2C(invblk)},
{"iocopy",(ScicosF) F2C(iocopy)},
{"logblk",(ScicosF) F2C(logblk)},
{"logicalop",(ScicosF) logicalop},
{"lookup",(ScicosF) F2C(lookup)},
{"lsplit",(ScicosF) F2C(lsplit)},
{"lusat",(ScicosF) F2C(lusat)},
{"maxblk",(ScicosF) F2C(maxblk)},
{"memo",(ScicosF) F2C(memo)},
{"mfclck",(ScicosF) F2C(mfclck)},
{"minblk",(ScicosF) F2C(minblk)},
{"minmax",(ScicosF) minmax},
{"modulo_count",(ScicosF) modulo_count},
{"mscope",(ScicosF) F2C(mscope)},
{"mswitch",(ScicosF) mswitch},
{"multiplex",(ScicosF) multiplex},
{"mux",(ScicosF) F2C(mux)},
{"pload",(ScicosF) F2C(pload)},
{"plusblk",(ScicosF) plusblk},
{"powblk",(ScicosF) F2C(powblk)},
{"prod",(ScicosF) prod},
{"product",(ScicosF) product},
{"qzcel",(ScicosF) F2C(qzcel)},
{"qzflr",(ScicosF) F2C(qzflr)},
{"qzrnd",(ScicosF) F2C(qzrnd)},
{"qztrn",(ScicosF) F2C(qztrn)},
{"ramp",(ScicosF) ramp},
{"ratelimiter",(ScicosF) ratelimiter},
{"readau",(ScicosF) readau},
{"readc",(ScicosF) readc},
{"readf",(ScicosF) F2C(readf)},
{"relationalop",(ScicosF) relationalop},
{"relay",(ScicosF) relay},
{"rndblk",(ScicosF) F2C(rndblk)},
{"samphold",(ScicosF) F2C(samphold)},
{"satur",(ScicosF) satur},
{"sawtth",(ScicosF) F2C(sawtth)},
{"scalar2vector",(ScicosF) scalar2vector},
{"scope",(ScicosF) F2C(scope)},
{"scopxy",(ScicosF) F2C(scopxy)},
{"scoxy",(ScicosF) F2C(scoxy)},
{"selblk",(ScicosF) F2C(selblk)},
{"selector",(ScicosF) selector},
{"signum",(ScicosF) signum},
{"sinblk",(ScicosF) F2C(sinblk)},
{"sin_blk",(ScicosF) sin_blk},
{"sinh_blk",(ScicosF) sinh_blk},
{"slider",(ScicosF) slider},
{"sqrblk",(ScicosF) F2C(sqrblk)},
{"step_func",(ScicosF) step_func},
{"sum2",(ScicosF) F2C(sum2)},
{"sum3",(ScicosF) F2C(sum3)},
{"summation",(ScicosF) summation},
{"sum",(ScicosF) sum},
{"switch2",(ScicosF) switch2},
{"switchn",(ScicosF) switchn},
{"tanblk",(ScicosF) F2C(tanblk)},
{"tan_blk",(ScicosF) tan_blk},
{"tanh_blk",(ScicosF) tanh_blk},
{"tcslti",(ScicosF) F2C(tcslti)},
{"tcsltj",(ScicosF) F2C(tcsltj)},
{"timblk",(ScicosF) F2C(timblk)},
{"time_delay",(ScicosF) time_delay},
{"trash",(ScicosF) F2C(trash)},
{"variable_delay",(ScicosF) variable_delay},
{"writeau",(ScicosF) writeau},
{"writec",(ScicosF) writec},
{"writef",(ScicosF) F2C(writef)},
{"zcross2",(ScicosF) zcross2},
{"zcross",(ScicosF) F2C(zcross)},
{(char *) 0, (ScicosF) 0}};
 
int ntabsim= 126 ;
#endif 
/***********************************/
