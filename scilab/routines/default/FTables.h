/******* Please do not edit *************/
/**************** fydot ***************/
extern void C2F(fex)(ARGS_fydot);
extern void C2F(fex2)(ARGS_fydot);
extern void C2F(fex3)(ARGS_fydot);
extern void C2F(fexab)(ARGS_fydot);
extern void C2F(loren)(ARGS_fydot);
extern void C2F(arnol)(ARGS_fydot);
extern void C2F(bcomp)(ARGS_fydot);
extern void C2F(lcomp)(ARGS_fydot);
 
FTAB FTab_fydot[] ={
{"arnol", (voidf)  C2F(arnol)},
{"bcomp", (voidf)  C2F(bcomp)},
{"fex", (voidf)  C2F(fex)},
{"fex2", (voidf)  C2F(fex2)},
{"fex3", (voidf)  C2F(fex3)},
{"fexab", (voidf)  C2F(fexab)},
{"lcomp", (voidf)  C2F(lcomp)},
{"loren", (voidf)  C2F(loren)},
{(char *) 0, (voidf) 0}};
/**************** ffeval ***************/
extern void C2F(parab)(ARGS_ffeval);
extern void C2F(parabc)(ARGS_ffeval);
 
FTAB FTab_ffeval[] ={
{"parab", (voidf)  C2F(parab)},
{"parabc", (voidf)  C2F(parabc)},
{(char *) 0, (voidf) 0}};
/**************** fres ***************/
extern void C2F(resid)(ARGS_fres);
 
FTAB FTab_fres[] ={
{"resid", (voidf)  C2F(resid)},
{(char *) 0, (voidf) 0}};
/**************** fadda ***************/
extern void C2F(aplusp)(ARGS_fadda);
 
FTAB FTab_fadda[] ={
{"aplusp", (voidf)  C2F(aplusp)},
{(char *) 0, (voidf) 0}};
/**************** fj2 ***************/
extern void C2F(dgbydy)(ARGS_fj2);
 
FTAB FTab_fj2[] ={
{"dgbydy", (voidf)  C2F(dgbydy)},
{(char *) 0, (voidf) 0}};
/**************** fjac ***************/
extern void C2F(jex)(ARGS_fjac);
 
FTAB FTab_fjac[] ={
{"jex", (voidf)  C2F(jex)},
{(char *) 0, (voidf) 0}};
/**************** dgetx ***************/
extern void C2F(corexx)(ARGS_dgetx);
 
FTAB FTab_dgetx[] ={
{"corexx", (voidf)  C2F(corexx)},
{(char *) 0, (voidf) 0}};
/**************** dgety ***************/
extern void C2F(corexy)(ARGS_dgety);
 
FTAB FTab_dgety[] ={
{"corexy", (voidf)  C2F(corexy)},
{(char *) 0, (voidf) 0}};
/**************** fcoldg ***************/
extern void C2F(cndg)(ARGS_fcoldg);
 
FTAB FTab_fcoldg[] ={
{"cndg", (voidf)  C2F(cndg)},
{(char *) 0, (voidf) 0}};
/**************** fcolg ***************/
extern void C2F(cng)(ARGS_fcolg);
 
FTAB FTab_fcolg[] ={
{"cng", (voidf)  C2F(cng)},
{(char *) 0, (voidf) 0}};
/**************** fcolf ***************/
extern void C2F(cnf)(ARGS_fcolf);
 
FTAB FTab_fcolf[] ={
{"cnf", (voidf)  C2F(cnf)},
{(char *) 0, (voidf) 0}};
/**************** fcoldf ***************/
extern void C2F(cndf)(ARGS_fcoldf);
 
FTAB FTab_fcoldf[] ={
{"cndf", (voidf)  C2F(cndf)},
{(char *) 0, (voidf) 0}};
/**************** fcolgu ***************/
extern void C2F(cngu)(ARGS_fcolgu);
 
FTAB FTab_fcolgu[] ={
{"cngu", (voidf)  C2F(cngu)},
{(char *) 0, (voidf) 0}};
/**************** fintg ***************/
extern void C2F(intgex)(ARGS_fintg);
 
FTAB FTab_fintg[] ={
{"intgex", (voidf)  C2F(intgex)},
{(char *) 0, (voidf) 0}};
/**************** fint2d ***************/
extern void C2F(int2dex)(ARGS_fint2d);
 
FTAB FTab_fint2d[] ={
{"int2dex", (voidf)  C2F(int2dex)},
{(char *) 0, (voidf) 0}};
/**************** fint3d ***************/
extern void C2F(int3dex)(ARGS_fint3d);
 
FTAB FTab_fint3d[] ={
{"int3dex", (voidf)  C2F(int3dex)},
{(char *) 0, (voidf) 0}};
/**************** fsolvf ***************/
extern void C2F(fsol1)(ARGS_fsolvf);
 
FTAB FTab_fsolvf[] ={
{"fsol1", (voidf)  C2F(fsol1)},
{(char *) 0, (voidf) 0}};
/**************** fsolvj ***************/
extern void C2F(fsolj1)(ARGS_fsolvj);
 
FTAB FTab_fsolvj[] ={
{"fsolj1", (voidf)  C2F(fsolj1)},
{(char *) 0, (voidf) 0}};
/**************** foptim ***************/
extern void C2F(genros)(ARGS_foptim);
extern void C2F(topt2)(ARGS_foptim);
extern void C2F(icsemc)(ARGS_foptim);
 
FTAB FTab_foptim[] ={
{"genros", (voidf)  C2F(genros)},
{"icsemc", (voidf)  C2F(icsemc)},
{"topt2", (voidf)  C2F(topt2)},
{(char *) 0, (voidf) 0}};
/**************** fschur ***************/
extern void C2F(folhp)(ARGS_fschur);
extern void C2F(find)(ARGS_fschur);
 
FTAB FTab_fschur[] ={
{"find", (voidf)  C2F(find)},
{"folhp", (voidf)  C2F(folhp)},
{(char *) 0, (voidf) 0}};
/**************** schsel ***************/
extern void C2F(sb02mv)(ARGS_schsel);
extern void C2F(sb02mw)(ARGS_schsel);
 
FTAB FTab_schsel[] ={
{"sb02mv", (voidf)  C2F(sb02mv)},
{"sb02mw", (voidf)  C2F(sb02mw)},
{(char *) 0, (voidf) 0}};
/**************** zchsel ***************/
extern void C2F(zb02mv)(ARGS_zchsel);
extern void C2F(zb02mw)(ARGS_zchsel);
 
FTAB FTab_zchsel[] ={
{"zb02mv", (voidf)  C2F(zb02mv)},
{"zb02mw", (voidf)  C2F(zb02mw)},
{(char *) 0, (voidf) 0}};
/**************** gshsel ***************/
extern void C2F(sb02ow)(ARGS_gshsel);
extern void C2F(sb02ox)(ARGS_gshsel);
 
FTAB FTab_gshsel[] ={
{"sb02ow", (voidf)  C2F(sb02ow)},
{"sb02ox", (voidf)  C2F(sb02ox)},
{(char *) 0, (voidf) 0}};
/**************** gzhsel ***************/
extern void C2F(zb02ow)(ARGS_gzhsel);
extern void C2F(zb02ox)(ARGS_gzhsel);
 
FTAB FTab_gzhsel[] ={
{"zb02ow", (voidf)  C2F(zb02ow)},
{"zb02ox", (voidf)  C2F(zb02ox)},
{(char *) 0, (voidf) 0}};
/**************** fydot2 ***************/
extern void C2F(fexcd)(ARGS_fydot2);
extern void C2F(fcd)(ARGS_fydot2);
extern void C2F(fcd1)(ARGS_fydot2);
extern void C2F(phis)(ARGS_fydot2);
extern void C2F(phit)(ARGS_fydot2);
 
FTAB FTab_fydot2[] ={
{"fcd", (voidf)  C2F(fcd)},
{"fcd1", (voidf)  C2F(fcd1)},
{"fexcd", (voidf)  C2F(fexcd)},
{"phis", (voidf)  C2F(phis)},
{"phit", (voidf)  C2F(phit)},
{(char *) 0, (voidf) 0}};
/**************** fresd ***************/
extern void C2F(dres1)(ARGS_fresd);
extern void C2F(dres2)(ARGS_fresd);
extern void C2F(res1)(ARGS_fresd);
extern void C2F(res2)(ARGS_fresd);
 
FTAB FTab_fresd[] ={
{"dres1", (voidf)  C2F(dres1)},
{"dres2", (voidf)  C2F(dres2)},
{"res1", (voidf)  C2F(res1)},
{"res2", (voidf)  C2F(res2)},
{(char *) 0, (voidf) 0}};
/**************** fjacd ***************/
extern void C2F(djac1)(ARGS_fjacd);
extern void C2F(djac2)(ARGS_fjacd);
extern void C2F(jac2)(ARGS_fjacd);
 
FTAB FTab_fjacd[] ={
{"djac1", (voidf)  C2F(djac1)},
{"djac2", (voidf)  C2F(djac2)},
{"jac2", (voidf)  C2F(jac2)},
{(char *) 0, (voidf) 0}};
/**************** fsurf ***************/
 
FTAB FTab_fsurf[] ={
{(char *) 0, (voidf) 0}};
/**************** fsurfd ***************/
extern void C2F(gr1)(ARGS_fsurfd);
extern void C2F(gr2)(ARGS_fsurfd);
 
FTAB FTab_fsurfd[] ={
{"gr1", (voidf)  C2F(gr1)},
{"gr2", (voidf)  C2F(gr2)},
{(char *) 0, (voidf) 0}};
/**************** fbutn ***************/
extern void C2F(haltscicos)(ARGS_fbutn);
 
FTAB FTab_fbutn[] ={
{"haltscicos", (voidf)  C2F(haltscicos)},
{(char *) 0, (voidf) 0}};
/**************** interf ***************/
extern void C2F(foubare)(ARGS_interf);
extern void C2F(foobar1)(ARGS_interf);
extern void C2F(icomp)(ARGS_interf);
extern void C2F(loset)(ARGS_interf);
extern void C2F(arset)(ARGS_interf);
extern void C2F(icse0)(ARGS_interf);
extern void C2F(icse)(ARGS_interf);
extern void C2F(icscof)(ARGS_interf);
 
FTAB FTab_interf[] ={
{"arset", (voidf)  C2F(arset)},
{"foobar1", (voidf)  C2F(foobar1)},
{"foubare", (voidf)  C2F(foubare)},
{"icomp", (voidf)  C2F(icomp)},
{"icscof", (voidf)  C2F(icscof)},
{"icse", (voidf)  C2F(icse)},
{"icse0", (voidf)  C2F(icse0)},
{"loset", (voidf)  C2F(loset)},
{(char *) 0, (voidf) 0}};
/**************** lsqrsolvf ***************/
extern void C2F(lsqrsol1)(ARGS_lsqrsolvf);
 
FTAB FTab_lsqrsolvf[] ={
{"lsqrsol1", (voidf)  C2F(lsqrsol1)},
{(char *) 0, (voidf) 0}};
/**************** lsqrsolvj ***************/
extern void C2F(lsqrsolj1)(ARGS_lsqrsolvj);
 
FTAB FTab_lsqrsolvj[] ={
{"lsqrsolj1", (voidf)  C2F(lsqrsolj1)},
{(char *) 0, (voidf) 0}};
/***********************************/
