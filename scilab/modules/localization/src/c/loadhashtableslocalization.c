/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <iconv.h>
#include <string.h> /* strcmp */
#include "loadhashtableslocalization.h"
#include "localization.h"
#include "getmodules.h"
#include "setgetSCIpath.h"
#include "libxml/xmlreader.h"
#include "../../fileio/includes/FileExist.h"
/*-----------------------------------------------------------------------------------*/ 
#define FILEERRORS "errors"
#define FILEMSGS "messages"
#define FILEMENUS "menus"
#define FILEFORMATPATH "%s/modules/%s/languages/%s/%s.xml"
/*-----------------------------------------------------------------------------------*/ 
BOOL LoadHashTableLocalization(struct hashtable *table,char *filenamexml);
static char *GetXmlFileEncoding(const char *filename);
static unsigned char* ConvertEncoding(char *encodingFrom, char *encodingTo, char* inputStr);
/*-----------------------------------------------------------------------------------*/ 
BOOL LoadHashTablesLocalization(char *language)
{
	BOOL bOK=FALSE;
	char *SciPath=NULL;
	int i=0;

	struct hashtable *Table_Errors=NULL;
	struct hashtable *Table_Messages=NULL;
	struct hashtable *Table_Menus=NULL;

	struct MODULESLIST *moduleslist=NULL;
	moduleslist=getmodules();

	Table_Errors=GetHashTableScilabErrors();	
	Table_Messages=GetHashTableScilabMessages();	
	Table_Menus=GetHashTableScilabMenus();	

	SciPath=getSCIpath();
	
	for(i=0;i<moduleslist->numberofModules;i++)
	{
		char *full_filename_errors=NULL;
		char *full_filename_messages=NULL;
		char *full_filename_menus=NULL;

		full_filename_errors=(char*)MALLOC(sizeof(char)*(strlen(SciPath)+strlen(FILEFORMATPATH)+strlen(moduleslist->ModuleList[i])+strlen(FILEERRORS)+strlen(language)+1));
		full_filename_messages=(char*)MALLOC(sizeof(char)*(strlen(SciPath)+strlen(FILEFORMATPATH)+strlen(moduleslist->ModuleList[i])+strlen(FILEMSGS)+strlen(language)+1));
		full_filename_menus=(char*)MALLOC(sizeof(char)*(strlen(SciPath)+strlen(FILEFORMATPATH)+strlen(moduleslist->ModuleList[i])+strlen(FILEMENUS)+strlen(language)+1));

		sprintf(full_filename_errors,FILEFORMATPATH,SciPath,moduleslist->ModuleList[i],language,FILEERRORS);
		sprintf(full_filename_messages,FILEFORMATPATH,SciPath,moduleslist->ModuleList[i],language,FILEMSGS);
		sprintf(full_filename_menus,FILEFORMATPATH,SciPath,moduleslist->ModuleList[i],language,FILEMENUS);

		if (FileExist(full_filename_errors)) LoadHashTableLocalization(Table_Errors,full_filename_errors);
		if (FileExist(full_filename_messages)) LoadHashTableLocalization(Table_Messages,full_filename_messages);
		if (FileExist(full_filename_menus)) LoadHashTableLocalization(Table_Menus,full_filename_menus);

		if (full_filename_errors) {FREE(full_filename_errors);full_filename_errors=NULL;}
		if (full_filename_messages) {FREE(full_filename_messages);full_filename_messages=NULL;}
		if (full_filename_menus) {FREE(full_filename_menus);full_filename_menus=NULL;}
		bOK=TRUE;
	}

	if (SciPath) {FREE(SciPath); SciPath=NULL;}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL LoadHashTableLocalization(struct hashtable *table,char *filenamexml)
{
	BOOL bOK=FALSE;
	BOOL bUTF_8_Mode=FALSE;

	char *TAGVALUE=NULL;
	char *STRINGVALUE=NULL;

	xmlTextReaderPtr reader;
	int ret=0;
	char *encoding=GetXmlFileEncoding(filenamexml);

	if((strcmp("utf-8", encoding)==0)||(strcmp("UTF-8", encoding)==0)) bUTF_8_Mode=TRUE;
	else bUTF_8_Mode=FALSE;

	reader = xmlReaderForFile(filenamexml, encoding, 0);

	ret = xmlTextReaderRead (reader);

	while (ret == 1)
	{
		const xmlChar *balise=NULL;
		xmlReaderTypes type;

		type = xmlTextReaderNodeType (reader);
		if (type ==  XML_READER_TYPE_ELEMENT )
		{
			balise = xmlTextReaderConstName (reader);

			if (xmlStrEqual (balise, (const xmlChar *)"tag"))
			{
				ret = xmlTextReaderRead (reader);
				type = xmlTextReaderNodeType (reader);
				if (type ==  XML_READER_TYPE_TEXT )
				{
					const xmlChar *node_value;
					if (bUTF_8_Mode)
					{
						node_value=(const xmlChar *)xmlTextReaderConstValue(reader);
					}
					else
					{
						node_value=ConvertEncoding("UTF-8",encoding,(char *)xmlTextReaderConstValue(reader));
					}

					TAGVALUE=(char*)MALLOC(sizeof(char)*(strlen(node_value)+1));
					strcpy(TAGVALUE,node_value);
				}
			}

			if (xmlStrEqual (balise, (const xmlChar *)"string"))
			{
				ret = xmlTextReaderRead (reader);
				type = xmlTextReaderNodeType (reader);
				if (type ==  XML_READER_TYPE_TEXT )
				{
					const xmlChar *node_value;
					if (bUTF_8_Mode)
					{
						node_value=(const xmlChar *)xmlTextReaderConstValue(reader);
					}
					else
					{
						node_value=ConvertEncoding("UTF-8",encoding,(char *)xmlTextReaderConstValue(reader));
					}

					STRINGVALUE=(char*)MALLOC(sizeof(char)*(strlen(node_value)+1));
					strcpy(STRINGVALUE,node_value);
				}
			}
		}

		if ( (TAGVALUE) && (STRINGVALUE))
		{
			/* remove case TAGVALUE=''  STRINGVALUE='' */
			if ( (strlen(TAGVALUE)>0) & (strlen(STRINGVALUE)>0) ) 
			{
				AppendHashTableLocalization(table,TAGVALUE,STRINGVALUE);
			}
			if (TAGVALUE) {FREE(TAGVALUE);TAGVALUE=NULL;}
			if (STRINGVALUE) {FREE(STRINGVALUE);STRINGVALUE=NULL;}
		}
		
		ret = xmlTextReaderRead (reader);
	}

	if (bUTF_8_Mode)
	{
		TAGVALUE=NULL;
		STRINGVALUE=NULL;
	}
	else
	{
		FREE(TAGVALUE);
		FREE(STRINGVALUE);
		TAGVALUE=NULL;
		STRINGVALUE=NULL;
	}

	xmlFreeTextReader(reader);
	/*
	* Cleanup function for the XML library.
	*/
	xmlCleanupParser();

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
static char *GetXmlFileEncoding(const char *filename)
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
static unsigned char* ConvertEncoding(char *encodingFrom, char *encodingTo, char* inputStr)
{
	unsigned char *strBufOut=NULL;
	unsigned char *outputStr=NULL;
	size_t inputLen, outputLen, result;
	iconv_t c_pt;

	strBufOut=(unsigned char *)MALLOC(sizeof(unsigned char)*2048);

	if ((c_pt = iconv_open(encodingTo, encodingFrom)) == (iconv_t)(-1))
	{
		printf("iconv_open failed!\n");
		return NULL;
	}

	iconv(c_pt, NULL, NULL, NULL, NULL);

	inputLen = strlen(inputStr) + 1;
	outputLen = 2048;
	outputStr = strBufOut;
	result = iconv(c_pt, &inputStr, &inputLen,  (char**)(&outputStr), &outputLen);

	if (result == -1)
	{
		return NULL;
	}

	iconv_close(c_pt);
	return strBufOut;
}
/*-----------------------------------------------------------------------------------*/
