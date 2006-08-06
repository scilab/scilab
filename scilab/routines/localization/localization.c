/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* HUANG Xu */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "localization.h"
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */

/*-----------------------------------------------------------------------------------*/ 
#define LENGTH_OUTPUT 1024
static int count=0;//count the number of the #text and type==3 node 
static char *tag=NULL;
static char *string=NULL;

static char *Key_String=NULL;
static char *Key_Value=NULL;

static char strBufOut[LENGTH_OUTPUT];

static struct hashtable *Table_Scilab_Errors=NULL;

static char *Replace(char *s1, char *s2, char *s3);
/*-----------------------------------------------------------------------------------*/ 
char* ConvertEncoding(char *encodingFrom, char *encodingTo, const char* inputStr)
{

  char * outputStr;
  size_t inputLen, outputLen, result;

  iconv_t c_pt;
  if ((c_pt = iconv_open(encodingTo, encodingFrom)) == (iconv_t)(-1))
  {
    printf("iconv_open failed!\n");
    return NULL;
  }
  iconv(c_pt, NULL, NULL, NULL, NULL);

  inputLen = strlen(inputStr) + 1;
  outputLen = LENGTH_OUTPUT;
  outputStr = strBufOut;
  result = iconv(c_pt, &inputStr, &inputLen, &outputStr, &outputLen);

  if (result == -1)
  {
    return NULL;
  }
  
  iconv_close(c_pt);
  return strBufOut;
}
/*-----------------------------------------------------------------------------------*/ 
void ProcessNode(xmlTextReaderPtr reader, struct hashtable *table, char *encoding) 
{
	if(xmlTextReaderNodeType(reader)==3)//to get all nodes whose type is 3(#text node)
	{

		
		const char *node_value;
		count++;
		
		if((strcmp("utf-8", encoding)==0)||(strcmp("UTF-8", encoding)==0))
		{
			node_value=(const char *)xmlTextReaderConstValue(reader);
		}
		else
		{
			node_value=ConvertEncoding("UTF-8",encoding,(const char *)xmlTextReaderConstValue(reader));
		}
		
		if((count%2)!=0)//odd, tag
		{
			Key_String=(char *)MALLOC(strlen(node_value)+1);
			strcpy(Key_String,node_value);
		}
		else//even, string
		{
			struct key_string *k;
			struct value_string *v;

			Key_Value=(char *)MALLOC(strlen(node_value)+1);
			strcpy(Key_Value,node_value);

			k=(struct key_string*)MALLOC(sizeof(struct key_string));
			v=(struct value_string*)MALLOC(sizeof(struct value_string));

			k->Key_String=Key_String;
			v->Value_String=Key_Value;
		
			InsertHashtable_string(table,k, v);
		}
	}
}
/*-----------------------------------------------------------------------------------*/
char *GetXmlFileEncoding(const char *filename)
{
	FILE *stream;
	char *encoding;
	encoding=(char *)MALLOC(sizeof(char)*32);
	
	if( (stream  = fopen(filename, "r" )) != NULL ) // C4996
	{
		char FirstLine[256];
		if( fgets( FirstLine, 256, stream ) == NULL)
		{
	        printf( "fgets error\n" );
			return NULL;
		}
	    else
		{
			char *pEncodingStart;
			char *pQuotationStart;
			char *pQuotationEnd;
			char *pTemp;
			size_t length;

			pEncodingStart=strstr(FirstLine, "encoding");
			pQuotationStart=strchr(pEncodingStart,'"');
			pTemp=pQuotationStart+1;
			pQuotationEnd=strchr(pTemp,'"');
			length=pQuotationEnd-pTemp;
			strncpy(encoding, pTemp, length);
			strcpy(encoding+length,"\0");
		}
		fclose( stream );
	}
	else
	{
		printf( "ERROR:xmlfile %s was not opened or xmlfile %s doesn't exist\n", filename, filename);//what to add to exit the whole program???
	}

	if(encoding==NULL)
	{
		strcpy(encoding,"utf-8");
	}
	return encoding;

}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_LOCALIZATION_DLL int AppendXmlFile(const char *filename, struct hashtable *table)
{
	int bOK=0;
    xmlTextReaderPtr reader;
    int ret;
	char *encoding=GetXmlFileEncoding(filename);

	reader = xmlReaderForFile(filename, encoding, 0);

    if (reader != NULL) 
	{
        ret = xmlTextReaderRead(reader);
        while (ret == 1)
		{
            ProcessNode(reader, table, encoding);
            ret = xmlTextReaderRead(reader);
        }

        xmlFreeTextReader(reader);
		/*
		* Cleanup function for the XML library.
		*/
		xmlCleanupParser();
		/*
		* this is to debug memory for regression tests
		*/
		xmlMemoryDump();

        if (ret != 0) 
		{
            bOK=0;
		}
		else
		{
			bOK=1;
		}
	}
	else 
	{
		bOK=0;
	}
	return bOK;
	
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_LOCALIZATION_DLL struct hashtable *GetHashTableScilabErrors(void)
{
	return Table_Scilab_Errors;
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_LOCALIZATION_DLL int InitializeHashTableScilabErrors(char* SCIPATH)
{
	char *FileLanguage=NULL;
	Table_Scilab_Errors=CreateHashtable_string();

	FileLanguage=(char*)MALLOC( (strlen(SCIPATH)+strlen("/localization/errors.xml")+1)*sizeof(char));
	strcpy(FileLanguage,SCIPATH);
	strcat(FileLanguage,"/localization/errors.xml");

	AppendXmlFile(FileLanguage, Table_Scilab_Errors);

	FREE(FileLanguage);
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_LOCALIZATION_DLL char *QueryStringError(char *Tag)
{	
	char oldpiece[8];
	char newpiece[8];
	char *StringError=NULL;
	char *StringWithoutSomeChars=NULL;

	/* Replace \r\n by \\r\\n */
	strcpy(oldpiece,"\r\n");
	strcpy(newpiece,"\\r\\n");
	StringWithoutSomeChars=Replace( Tag,oldpiece,newpiece);

	StringError=SearchHashtable_string(Table_Scilab_Errors,StringWithoutSomeChars);//show the string we need
	FREE(StringWithoutSomeChars);
	
	if (StringError)
	{
		/* Replace \\r\\n by \r\n */
		strcpy(oldpiece,"\\r\\n");
		strcpy(newpiece,"\r\n");
		StringWithoutSomeChars=Replace(StringError,oldpiece,newpiece);
		FREE(StringError);
		StringError=StringWithoutSomeChars;
	}

	return StringError;
}
/*-----------------------------------------------------------------------------------*/ 
static char *Replace(char *S1, char *S2, char *S3) 
{ 
   int str_index, newstr_index, oldpiece_index, end,new_len, old_len, cpy_len; 
   char *c=NULL; 
   char *newstring=NULL;

   newstring = (char*)MALLOC((strlen(S1)*2)*sizeof(char));

   if (newstring)
   {
	   if ((c = (char *) strstr(S1, S2)) == NULL) return S1; 

	   new_len        = strlen(S3);
	   old_len        = strlen(S2);
	   end            = strlen(S1) - old_len;
	   oldpiece_index = c - S1;

	   newstr_index = 0; 
	   str_index = 0; 

	   while(str_index <= end && c != NULL) 
	   { 
		   /* Copy characters from the left of matched pattern occurrence */
		   cpy_len = oldpiece_index-str_index;
		   strncpy(newstring+newstr_index, S1+str_index, cpy_len);
		   newstr_index += cpy_len;
		   str_index    += cpy_len;

		   /* Copy replacement characters instead of matched pattern */
		   strcpy(newstring+newstr_index, S3);
		   newstr_index += new_len;
		   str_index    += old_len;

		   /* Check for another pattern match */
		   if((c = (char *) strstr(S1+str_index, S2)) != NULL) oldpiece_index = c - S1;
	   }

   }
   else return S1; 
   
   return newstring; 
}
/*-----------------------------------------------------------------------------------*/ 