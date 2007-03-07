#ifndef _cc_matrix_H_
#define _cc_matrix_H_


class CMatrix 
{
 private: 
  int m,n;
  char *name;
  double *v;

 public:
  CMatrix(char *name1,char *job);
  void scijob(char *jobname);
  CMatrix& plus(CMatrix &B);
  void print();
  void inv();

};

#endif


