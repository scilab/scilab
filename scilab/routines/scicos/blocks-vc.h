#ifndef __SCICOS_BLOCKS__ 
#define __SCICOS_BLOCKS__ 
/******* Copyright INRIA *************/
/******* Please do not edit *************/
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
extern void F2C(intrp2) __PARAMS((ARGS_scicos));
extern void F2C(intrpl) __PARAMS((ARGS_scicos));
extern void F2C(fsv) __PARAMS((ARGS_scicos));
extern void F2C(memo) __PARAMS((ARGS_scicos));
extern void F2C(fscope) __PARAMS((ARGS_scicos));
extern void F2C(scoxy) __PARAMS((ARGS_scicos));
extern void F2C(diffblk) __PARAMS((ARGS_scicos));
extern void F2C(constraint) __PARAMS((ARGS_scicos));
extern void F2C(absblk) __PARAMS((ARGS_scicos));
extern void F2C(andlog) __PARAMS((ARGS_scicos));
extern void F2C(bidon) __PARAMS((ARGS_scicos));
extern void F2C(gain) __PARAMS((ARGS_scicos));
extern void F2C(cdummy) __PARAMS((ARGS_scicos));
extern void F2C(dband) __PARAMS((ARGS_scicos));
extern void F2C(cosblk) __PARAMS((ARGS_scicos));
extern void F2C(ifthel) __PARAMS((ARGS_scicos));
extern void F2C(eselect) __PARAMS((ARGS_scicos));
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
extern void slider __PARAMS((ARGS_scicos));
extern void zcross2 __PARAMS((ARGS_scicos));
extern void mswitch __PARAMS((ARGS_scicos));
extern void logicalop __PARAMS((ARGS_scicos));
extern void switch2 __PARAMS((ARGS_scicos));
extern void variable_delay __PARAMS((ARGS_scicos));
extern void time_delay __PARAMS((ARGS_scicos));
extern void cscope __PARAMS((ARGS_scicos));
extern void cmscope __PARAMS((ARGS_scicos));
extern void satur __PARAMS((ARGS_scicos));
extern void step_func __PARAMS((ARGS_scicos));
extern void integral_func __PARAMS((ARGS_scicos));
extern void absolute_value __PARAMS((ARGS_scicos));
extern void bounce_ball __PARAMS((ARGS_scicos));
extern void bouncexy __PARAMS((ARGS_scicos));
extern void extractor __PARAMS((ARGS_scicos));
extern void scalar2vector __PARAMS((ARGS_scicos));
extern void minmax __PARAMS((ARGS_scicos));
extern void signum __PARAMS((ARGS_scicos));
extern void product __PARAMS((ARGS_scicos));
extern void summation __PARAMS((ARGS_scicos));
extern void multiplex __PARAMS((ARGS_scicos));
extern void gainblk __PARAMS((ARGS_scicos));
extern void relationalop __PARAMS((ARGS_scicos));
extern void modulo_count __PARAMS((ARGS_scicos));
extern void hystheresis __PARAMS((ARGS_scicos));
extern void ratelimiter __PARAMS((ARGS_scicos));
extern void backlash __PARAMS((ARGS_scicos));
extern void deadband __PARAMS((ARGS_scicos));
extern void ramp __PARAMS((ARGS_scicos));
extern void evaluate_expr __PARAMS((ARGS_scicos));
extern void deriv __PARAMS((ARGS_scicos));
extern void sin_blk __PARAMS((ARGS_scicos));
extern void cos_blk __PARAMS((ARGS_scicos));
extern void tan_blk __PARAMS((ARGS_scicos));
extern void asin_blk __PARAMS((ARGS_scicos));
extern void acos_blk __PARAMS((ARGS_scicos));
extern void atan_blk __PARAMS((ARGS_scicos));
extern void sinh_blk __PARAMS((ARGS_scicos));
extern void cosh_blk __PARAMS((ARGS_scicos));
extern void tanh_blk __PARAMS((ARGS_scicos));
extern void asinh_blk __PARAMS((ARGS_scicos));
extern void acosh_blk __PARAMS((ARGS_scicos));
extern void atanh_blk __PARAMS((ARGS_scicos));
extern void evtvardly __PARAMS((ARGS_scicos));
 
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
{"gain",(ScicosF) F2C(gain)},
{"gainblk",(ScicosF) gainblk},
{"gensin",(ScicosF) F2C(gensin)},
{"gensqr",(ScicosF) F2C(gensqr)},
{"hltblk",(ScicosF) F2C(hltblk)},
{"hystheresis",(ScicosF) hystheresis},
{"ifthel",(ScicosF) F2C(ifthel)},
{"integr",(ScicosF) F2C(integr)},
{"integral_func",(ScicosF) integral_func},
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
{"sin_blk",(ScicosF) sin_blk},
{"sinblk",(ScicosF) F2C(sinblk)},
{"sinh_blk",(ScicosF) sinh_blk},
{"slider",(ScicosF) slider},
{"sqrblk",(ScicosF) F2C(sqrblk)},
{"step_func",(ScicosF) step_func},
{"sum",(ScicosF) sum},
{"sum2",(ScicosF) F2C(sum2)},
{"sum3",(ScicosF) F2C(sum3)},
{"summation",(ScicosF) summation},
{"switch2",(ScicosF) switch2},
{"switchn",(ScicosF) switchn},
{"tan_blk",(ScicosF) tan_blk},
{"tanblk",(ScicosF) F2C(tanblk)},
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
{"zcross",(ScicosF) F2C(zcross)},
{"zcross2",(ScicosF) zcross2},
{(char *) 0, (ScicosF) 0}};
 
int ntabsim= 126 ;
#endif 
/***********************************/
