#include <mex.h> 
extern Gatefunc intex1c;
extern Gatefunc C2F(intex1f);
extern Gatefunc intex2c_1;
extern Gatefunc intex2c_2;
extern Gatefunc C2F(intex2f1);
extern Gatefunc C2F(intex2f2);
extern Gatefunc intex3c_1;
extern Gatefunc intex3c_2;
extern Gatefunc intex3c_3;
extern Gatefunc intex4c_1;
extern Gatefunc intex4c_2;
extern Gatefunc intex4c_3;
extern Gatefunc intex4c_4;
extern Gatefunc C2F(intex4f1);
extern Gatefunc C2F(intex4f2);
extern Gatefunc C2F(intex4f3);
extern Gatefunc C2F(intex4f4);
extern Gatefunc intex5c_1;
extern Gatefunc intex5c_2;
extern Gatefunc intex6c_1;
extern Gatefunc intex6c_2;
extern Gatefunc intex6c_3;
extern Gatefunc intex6c_4;
extern Gatefunc intex7c_1;
extern Gatefunc intex7c_2;
extern Gatefunc intex7c_3;
extern Gatefunc intex8c_1;
extern Gatefunc intex8c_2;
extern Gatefunc intex9c_1;
extern Gatefunc intex9c_2;
extern Gatefunc intex9c_3;
extern Gatefunc intex9c_4;
extern Gatefunc C2F(intex9f1);
extern Gatefunc C2F(intex9f2);
extern Gatefunc C2F(intex9f3);
extern Gatefunc C2F(intex9f4);
extern Gatefunc intex10c_1;
extern Gatefunc intex10c_2;
extern Gatefunc intex10c_3;
extern Gatefunc intex10c_4;
extern Gatefunc intex11c;
extern Gatefunc intex12c;
extern Gatefunc C2F(intex12f);
extern Gatefunc intex13c_1;
extern Gatefunc intex13c_2;
extern Gatefunc intex13c_3;
extern Gatefunc C2F(intex13f1);
extern Gatefunc C2F(intex13f2);
extern Gatefunc C2F(intex13f3);
extern Gatefunc intex14c;
extern Gatefunc C2F(intex14f);
extern Gatefunc intex15c;
extern Gatefunc intex15f;
extern Gatefunc intex16c;
extern Gatefunc intex17c_1;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway,intex1c,"ex1c"},
  {(Myinterfun)sci_gateway,C2F(intex1f),"ex1f"},
  {(Myinterfun)sci_gateway,intex2c_1,"ex2c_1"},
  {(Myinterfun)sci_gateway,intex2c_2,"ex2c_2"},
  {(Myinterfun)sci_gateway,C2F(intex2f1),"ex2f_1"},
  {(Myinterfun)sci_gateway,C2F(intex2f2),"ex2f_2"},
  {(Myinterfun)sci_gateway,intex3c_1,"ex3c_1"},
  {(Myinterfun)sci_gateway,intex3c_2,"ex3c_2"},
  {(Myinterfun)sci_gateway,intex3c_3,"ex3c_3"},
  {(Myinterfun)sci_gateway,intex4c_1,"ex4c_1"},
  {(Myinterfun)sci_gateway,intex4c_2,"ex4c_2"},
  {(Myinterfun)sci_gateway,intex4c_3,"ex4c_3"},
  {(Myinterfun)sci_gateway,intex4c_4,"ex4c_4"},
  {(Myinterfun)sci_gateway,C2F(intex4f1),"ex4f_1"},
  {(Myinterfun)sci_gateway,C2F(intex4f2),"ex4f_2"},
  {(Myinterfun)sci_gateway,C2F(intex4f3),"ex4f_3"},
  {(Myinterfun)sci_gateway,C2F(intex4f4),"ex4f_4"},
  {(Myinterfun)sci_gateway,intex5c_1,"ex5c_1"},
  {(Myinterfun)sci_gateway,intex5c_2,"ex5c_2"},
  {(Myinterfun)sci_gateway,intex6c_1,"ex6c_1"},
  {(Myinterfun)sci_gateway,intex6c_2,"ex6c_2"},
  {(Myinterfun)sci_gateway,intex6c_3,"ex6c_3"},
  {(Myinterfun)sci_gateway,intex6c_4,"ex6c_4"},
  {(Myinterfun)sci_gateway,intex7c_1,"ex7c_1"},
  {(Myinterfun)sci_gateway,intex7c_2,"ex7c_2"},
  {(Myinterfun)sci_gateway,intex7c_3,"ex7c_3"},
  {(Myinterfun)sci_gateway,intex8c_1,"ex8c_1"},
  {(Myinterfun)sci_gateway,intex8c_2,"ex8c_2"},
  {(Myinterfun)sci_gateway,intex9c_1,"ex9c_1"},
  {(Myinterfun)sci_gateway,intex9c_2,"ex9c_2"},
  {(Myinterfun)sci_gateway,intex9c_3,"ex9c_3"},
  {(Myinterfun)sci_gateway,intex9c_4,"ex9c_4"},
  {(Myinterfun)sci_gateway,C2F(intex9f1),"ex9f_1"},
  {(Myinterfun)sci_gateway,C2F(intex9f2),"ex9f_2"},
  {(Myinterfun)sci_gateway,C2F(intex9f3),"ex9f_3"},
  {(Myinterfun)sci_gateway,C2F(intex9f4),"ex9f_4"},
  {(Myinterfun)sci_gateway,intex10c_1,"ex10c_1"},
  {(Myinterfun)sci_gateway,intex10c_2,"ex10c_2"},
  {(Myinterfun)sci_gateway,intex10c_3,"ex10c_3"},
  {(Myinterfun)sci_gateway,intex10c_4,"ex10c_4"},
  {(Myinterfun)sci_gateway,intex11c,"ex11c"},
  {(Myinterfun)sci_gateway,intex12c,"ex12c"},
  {(Myinterfun)sci_gateway,C2F(intex12f),"ex12f"},
  {(Myinterfun)sci_gateway,intex13c_1,"ex13c_1"},
  {(Myinterfun)sci_gateway,intex13c_2,"ex13c_2"},
  {(Myinterfun)sci_gateway,intex13c_3,"ex13c_3"},
  {(Myinterfun)sci_gateway,C2F(intex13f1),"ex13f_1"},
  {(Myinterfun)sci_gateway,C2F(intex13f2),"ex13f_2"},
  {(Myinterfun)sci_gateway,C2F(intex13f3),"ex13f_3"},
  {(Myinterfun)sci_gateway,intex14c,"ex14c"},
  {(Myinterfun)sci_gateway,C2F(intex14f),"ex14f"},
  {(Myinterfun)sci_gateway,intex15c,"ex15c"},
  {(Myinterfun)sci_gateway,intex15f,"ex15f"},
  {(Myinterfun)sci_gateway,intex16c,"ex16c"},
  {(Myinterfun)sci_gateway,intex17c_1,"ex17c"},
};
 
int C2F(libexamples)()
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}
