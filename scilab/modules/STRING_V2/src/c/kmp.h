/*------------------------------------------------------------------------*/
/* Copyright INRIA 2007*/
/*------------------------------------------------------------------------*/
#ifndef __KMP_H__
#define __KMP_H__

/*------------------------------------------------------------------------*/
/**
 * The KMP method to search in a string.
 * @Main_String[] is  the input of the main string.
 * @Sub_String[] is  the input of substring.
 * @return  the start point of the substring or 0.
 */
int kmp(char Main_String[],char Sub_String[],int pos,int *next);
/*------------------------------------------------------------------------*/
/**
 * To get the next value of the substring of the KMP method.
 * @*next is the pointer to the next value.
 * @Substring[] is  the input of substring.
 */
void getnext(char Substring[], int *next);
#endif /* __KMP_H__ */
/*------------------------------------------------------------------------*/
