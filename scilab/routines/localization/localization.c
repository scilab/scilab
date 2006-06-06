/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* HUANG Xu */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "localization.h"
/*-----------------------------------------------------------------------------------*/ 
#define LENGTH_OUTPUT 1024
static int count=0;//count the number of the #text and type==3 node 
static gchar *tag=NULL;
static gchar *string=NULL;

static char strBufOut[LENGTH_OUTPUT];

static GHashTable *Table_Scilab_Errors=NULL;

char *Replace(char *s1, char *s2, char *s3);
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
void ProcessNode(xmlTextReaderPtr reader, GHashTable *table, char *encoding) 
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
			tag=(gchar *)malloc(strlen(node_value)+1);
			strcpy(tag,node_value);
		}
		else//even, string
		{
	
			string=(gchar *)malloc(strlen(node_value)+1);
			strcpy(string,node_value);
			g_hash_table_replace(table, g_strdup(tag), g_strdup(string));
			free(tag); tag=NULL;
			free(string); string=NULL;

		}
	}
}
/*-----------------------------------------------------------------------------------*/
char *GetXmlFileEncoding(const char *filename)
{
	FILE *stream;
	char *encoding;
	encoding=(char *)malloc(sizeof(char)*32);
	
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
int AppendXmlFile(const char *filename, GHashTable *table)
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
GHashTable *GetHashTableScilabErrors(void)
{
	return Table_Scilab_Errors;
}
/*-----------------------------------------------------------------------------------*/ 
int InitializeHashTableScilabErrors(char* SCIPATH)
{
	char *FileLanguage=NULL;
	Table_Scilab_Errors=CreateHashtable();

	FileLanguage=(char*)malloc( (strlen(SCIPATH)+strlen("/localization/errors.xml")+1)*sizeof(char));
	strcpy(FileLanguage,SCIPATH);
	strcat(FileLanguage,"/localization/errors.xml");

	AppendXmlFile(FileLanguage, Table_Scilab_Errors);

	free(FileLanguage);
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
char *QueryStringError(char *Tag)
{	
	char oldpiece[8];
	char newpiece[8];
	char *StringError=NULL;
	char *StringWithoutSomeChars=NULL;

	/* Replace \r\n by \\r\\n */
	strcpy(oldpiece,"\r\n");
	strcpy(newpiece,"\\r\\n");
	StringWithoutSomeChars=Replace( Tag,oldpiece,newpiece);
	

	StringError=SearchHash(Table_Scilab_Errors,StringWithoutSomeChars);//show the string we need
	free(StringWithoutSomeChars);
	
	if (StringError)
	{
		/* Replace \\r\\n by \r\n */
		strcpy(oldpiece,"\\r\\n");
		strcpy(newpiece,"\r\n");
		StringWithoutSomeChars=Replace(StringError,oldpiece,newpiece);
		
		StringError=StringWithoutSomeChars;
	}

	return StringError;
}
/*-----------------------------------------------------------------------------------*/ 
char *Replace(char *s1, char *s2, char *s3) 
{ 
  char *retour=NULL; 
  
  if(s1 && s2 && s3)
  {
	char *tmp=NULL; 
	int i=0; 
	int j=0; 
	int lenS2=strlen(s2);
	int lenS3=strlen(s3);

	if (!(tmp = strstr(s1, s2))) return (s1); 
	
	retour = malloc(1); 
	while ((tmp = strstr(s1 + i, s2)) != 0) 
    { 
      if (!(strcmp(s1 + i, tmp))) 
        { 
          retour = realloc(retour, strlen(retour) + lenS3); 
          strcpy(retour + j, s3); 
          i += lenS2; 
          j += lenS3; 
        } 
      else 
        { 
          retour = realloc(retour, strlen(retour) + 1); 
          retour[j++] = s1[i++]; 
        } 
    } 
	while (s1[i]) 
    { 
      retour = realloc(retour, strlen(retour) + 1); 
      retour[j++] = s1[i++]; 
    } 
	retour[j] = 0; 
  }
  else retour=NULL;

  return (retour); 
} 
/*-----------------------------------------------------------------------------------*/ 