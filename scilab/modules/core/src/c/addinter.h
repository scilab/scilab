#ifndef __ADDINTER_H__
#define  __ADDINTER_H__
/** the first dynamic interface is at position 500+1 **/
/* Copyright ENPC */
#define DynInterfStart 500

int  SciLibLoad(int num_names, char **names, char **files, int *nums, int *err);
void BuildName(char *name, char *str);

void CallDynInterf(int *pos, int num_names, int namepos, char **names,
				   int *nums, char **files);

#endif /*  __ADDINTER_H__ */
