#ifndef __ADDINTER_H__
#define  __ADDINTER_H__
/** the first dynamic interface is at position 500+1 **/
/* Copyright ENPC */
#define DynInterfStart 500

/**
 * <long-description>
 *
 * @param num_names 
 * @param names 
 * @param files 
 * @param nums  
 * @param err   
 * @return <ReturnValue>
 */
int SciLibLoad(int num_names, char **names, char **files, int *nums, int *err);
/**
 * <long-description>
 *
 * @param pos   
 * @param num_names 
 * @param namepos   
 * @param names 
 * @param nums  
 * @param files 
 */
void CallDynInterf(int *pos, int num_names, int namepos, char **names,
				   int *nums, char **files);

/**
 * <long-description>
 *
 * @param int   
 */
void RemoveInterf (int);

#endif /*  __ADDINTER_H__ */
