#include <mex.h> 
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc mex_mexfunction01;
extern Gatefunc mex_mexfunction02;
extern Gatefunc mex_mexfunction03;
extern Gatefunc mex_mexfunction04;
extern Gatefunc mex_mexfunction05;
extern Gatefunc mex_mexfunction06;
extern Gatefunc mex_mexfunction07;
extern Gatefunc mex_mexfunction08;
extern Gatefunc mex_mexfunction09;
extern Gatefunc mex_mexfunction10;
extern Gatefunc mex_mexfunction11;
extern Gatefunc mex_mexfunction12;
extern Gatefunc mex_mexfunction13;
extern Gatefunc mex_mexfunction14;
extern Gatefunc mex_mexfunction15;
static GenericTable Tab[]={
  {mex_gateway,mex_mexfunction01,"mexf01"},
  {mex_gateway,mex_mexfunction02,"mexf02"},
  {mex_gateway,mex_mexfunction03,"mexf03"},
  {mex_gateway,mex_mexfunction04,"mexf04"},
  {mex_gateway,mex_mexfunction05,"mexf05"},
  {mex_gateway,mex_mexfunction06,"mexf06"},
  {mex_gateway,mex_mexfunction07,"mexf07"},
  {mex_gateway,mex_mexfunction08,"mexf08"},
  {mex_gateway,mex_mexfunction09,"mexf09"},
  {mex_gateway,mex_mexfunction10,"mexf10"},
  {mex_gateway,mex_mexfunction11,"mexf11"},
  {mex_gateway,mex_mexfunction12,"mexf12"},
  {mex_gateway,mex_mexfunction13,"mexf13"},
  {mex_gateway,mex_mexfunction14,"mexf14"},
  {mex_gateway,mex_mexfunction15,"mexf15"},
};
 
int C2F(libmex)()
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}
