/*------------------------------------------------------------------------*/
/* file: sortTemplate.h                                                   */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc :  The .h file for the gsort                                      */
/*------------------------------------------------------------------------*/

#ifndef _SORT_TEMPLATE_H_
#define _SORT_TEMPLATE_H_
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @TODO add comment
 *
 */
void sortTemplate( void);


/**
  * @TODO add comment
  *
  * @param Input_Matrix  
  * @param indices   
  * @param RowNum    
  * @param RowCol    
  * @param typex 
  * @param iord  
  */
void lgsortdouble(double *Input_Matrix,int *indices,int RowNum,int RowCol,char *typex, char *iord);


    /**
     * @TODO add comment
     *
     * @param Input_Matrix  
     * @param indices   
     * @param RowNum    
     * @param RowCol    
     * @param typex 
     * @param iord	
	 */
void lgsortstring(char **Input_Matrix,int *indices,int RowNum,int RowCol,char *typex, char *iord);

    /**
     * @TODO add comment
     *
     * @param Input_Matrix  
     * @param indices   
     * @param RowNum    
     * @param RowCol    
     * @param typex 
     * @param iord	
	 */
	void rowcolsortdouble(double *Input_Matrix, int *indices, int RowNum, int RowCol, char *typex, char *iord);

    /**
     * @TODO add comment
     *
     * @param Input_Matrix  
     * @param indices   
     * @param RowNum    
     * @param RowCol    
     * @param typex 
     * @param iord	
	 */
	void wholesortdouble(double *Input_Matrix, int *indices, int RowNum, int RowCol, char *typex, char *iord);

    /**
     *  @TODO add comment
     *
     * @param Input_Matrix  
     * @param index 
     * @param RowNum    
     * @param RowCol    
     * @param typex 
     * @param iord	
	 */
	void wholesortstring(char **Input_Matrix, int *index, int RowNum, int RowCol, char *typex, char *iord);

    /**
     * @TODO add comment
     *
     * @param Input_Matrix  
     * @param indices   
     * @param RowNum    
     * @param RowCol    
     * @param typex 
     * @param iord	
	 */
	void rowcolsortstring(char **Input_Matrix, int *indices, int RowNum, int RowCol, char *typex, char *iord);

#ifdef __cplusplus
}
#endif
#endif /* _SORT_TEMPLATE_H_  */
