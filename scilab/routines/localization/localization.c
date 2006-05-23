/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* HUANG Xu */
/*-----------------------------------------------------------------------------------*/ 
#include "localization.h"
/*-----------------------------------------------------------------------------------*/ 
#define LENGTH_OUTPUT 1024
int count=0;//count the number of the #text and type==3 node 
gchar *tag=NULL;
gchar *string=NULL;

char strBufOut[LENGTH_OUTPUT];
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
		
//		const char *node_value=(const char *)xmlTextReaderConstValue(reader);
		if((count%2)!=0)//odd, tag
		{
			tag=(gchar *)malloc(strlen(node_value)+1);
			strcpy(tag,node_value);
		}
		else//even, string
		{
	
			string=(gchar *)malloc(strlen(node_value)+1);
			strcpy(string,node_value);
			//hashtable insert

			/* it works also
			gchar *tag1=g_strdup(tag);
			gchar *string1=g_strdup(string);*/

			g_hash_table_replace(table, g_strdup(tag), g_strdup(string));
			//g_hash_table_insert(table, tag1, string1);
			printf("tag:%s, string:%s\t inserted\n",tag,string);
			free(tag); tag=NULL;
			free(string); string=NULL;

		}
	}

}
/*-----------------------------------------------------------------------------------*/ 
void AppendXmlFile(const char *filename, GHashTable *table, char* encoding)
{
    xmlTextReaderPtr reader;
    int ret;


//    reader = xmlReaderForFile(filename, NULL, 0);
	reader = xmlReaderForFile(filename, encoding, 0);

    if (reader != NULL) 
	{
        ret = xmlTextReaderRead(reader);
        while (ret == 1)
		{
            ProcessNode(reader, table, encoding);
            ret = xmlTextReaderRead(reader);
        }

		printf("ATTENTION:Appending of the xmlfile %s has finished\n", filename);

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
            fprintf(stderr, "%s : failed to parse\n", filename);
		}
	}
	else 
	{
		fprintf(stderr, "Unable to open %s\n", filename);
	}
	
}
/*-----------------------------------------------------------------------------------*/ 

