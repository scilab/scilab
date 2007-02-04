/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "setgetlanguage.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#define LengthAlpha2code 3
typedef struct languages_struct 
{
	int code;
	char alpha2code[LengthAlpha2code];
	char languagename[96];      /** its name **/
} LanguagesTable;
/*-----------------------------------------------------------------------------------*/ 
#define NumberLanguages 204
static LanguagesTable TabLanguages[NumberLanguages]=
/**
*  ISO 639-1 Code
*   http://www.loc.gov/standards/iso639-2/php/code_list.php 
*/
{
	{  1,	"aa",	"Afar"},
	{  2,	"ab",	"Abkhazian"},
	{  3,	"af",	"Afrikaans"},
	{  4,	"ak",	"Akan"},
	{  5,	"sq",	"Albanian"},
	{  6,	"am",	"Amharic"},
	{  7,	"ar",	"Arabic"},
	{  8,	"an",	"Aragonese"},
	{  9,	"hy",	"Armenian"},
	{ 10,	"as",	"Assamese"},
	{ 11,	"av",	"Avaric"},
	{ 12,	"ae",	"Avestan"},
	{ 13,	"ay",	"Aymara"},
	{ 14,	"az",	"Azerbaijani"},
	{ 15,	"ba",	"Bashkir"},
	{ 16,	"bm",	"Bambara"},
	{ 17,	"eu",	"Basque"},
	{ 18,	"be",	"Belarusian"},
	{ 19,	"bn",	"Bengali"},
	{ 20,	"bh",	"Bihari"},
	{ 21,	"bi",	"Bislama"},
	{ 22,	"bo",	"Tibetan"},
	{ 23,	"bs",	"Bosnian"},
	{ 24,	"br",	"Breton"},
	{ 25,	"bg",	"Bulgarian"},
	{ 26,	"my",	"Burmese"},
	{ 27,	"ca",	"Catalan; Valencian"},
	{ 28,	"cs",	"Czech"},
	{ 29,	"ch",	"Chamorro"},
	{ 30,	"ce",	"Chechen"},
	{ 31,	"zh",	"Chinese"},
	{ 32,	"cu",	"Church Slavic; Old Slavonic; Church Slavonic; Old Bulgarian; Old Church Slavonic"},
	{ 33,	"cv",	"Chuvash"},
	{ 34,	"kw",	"Cornish"},
	{ 35,	"co",	"Corsican"},
	{ 36,	"cr",	"Cree"},
	{ 37,	"cy",	"Welsh"},
	{ 38,	"cs",	"Czech"},
	{ 39,	"da",	"Danish"},
	{ 40,	"de",	"German"},
	{ 41,	"dv",	"Divehi; Dhivehi; Maldivian"},
	{ 42,	"nl",	"Dutch; Flemish"},
	{ 43,	"dz",	"Dzongkha"},
	{ 44,	"el",	"Greek, Modern (1453-)"},
	{ 45,	"en",	"English"},
	{ 46,	"eo",	"Esperanto"},
	{ 47,	"et",	"Estonian "},
	{ 48,	"eu",	"Basque"},
	{ 49,	"ee",	"Ewe"},
	{ 50,	"fo",	"Faroese"},
	{ 51,	"fa",	"Persian"},
	{ 52,	"fj",	"Fijian"},
	{ 53,	"fi",	"Finnish"},
	{ 54,	"fr",	"French"},
	{ 55,	"fy",	"Western Frisian"},
	{ 56,	"ff",	"Fulah"},
	{ 57,	"ka",	"Georgian"},
	{ 58,	"de",	"German"},
	{ 59,	"gd",	"Gaelic; Scottish Gaelic"},
	{ 60,	"ga",	"Irish"},
	{ 61,	"gl",	"Galician"},
	{ 62,	"gv",	"Manx"},
	{ 63,	"el",	"Greek, Modern (1453-)"},
	{ 64,	"gn",	"Guarani"},
	{ 65,	"gu",	"Gujarati"},
	{ 66,	"ht",	"Haitian; Haitian Creole"},
	{ 67,	"ha",	"Hausa"},
	{ 68,	"he",	"Hebrew"},
	{ 69,	"hz",	"Herero"},
	{ 70,	"hi",	"Hindi"},
	{ 71,	"ho",	"Hiri Motu"},
	{ 72,	"hr",	"Croatian"},
	{ 73,	"hu",	"Hungarian"},
	{ 74,	"hy",	"Armenian"},
	{ 75,	"ig",	"Igbo"},
	{ 76,	"is",	"Icelandic"},
	{ 77,	"io",	"Ido"},
	{ 78,	"ii",	"Sichuan Yi"},
	{ 79,	"iu",	"Inuktitut"},
	{ 80,	"ie",	"Interlingue"},
	{ 81,	"ia",	"Interlingua (International Auxiliary Language Association)"},
	{ 82,	"id",	"Indonesian"},
	{ 83,	"ik",	"Inupiaq"},
	{ 84,	"is",	"Icelandic"},
	{ 85,	"it",	"Italian"},
	{ 86,	"jv",	"Javanese"},
	{ 87,	"ja",	"Japanese"},
	{ 88,	"kl",	"Kalaallisut; Greenlandic"},
	{ 89,	"kn",	"Kannada"},
	{ 90,	"ks",	"Kashmiri"},
	{ 91,	"ka",	"Georgian"},
	{ 92,	"kr",	"Kanuri"},
	{ 93,	"kk",	"Kazakh"},
	{ 94,	"km",	"Central Khmer"},
	{ 95,	"ki",	"Kikuyu; Gikuyu"},
	{ 96,	"rw",	"Kinyarwanda"},
	{ 97,	"ky",	"Kirghiz; Kyrgyz"},
	{ 98,	"kv",	"Komi"},
	{ 99,	"kg",	"Kongo"},
	{100,	"ko",	"Korean"},
	{101,	"kj",	"Kuanyama; Kwanyama"},
	{102,	"ku",	"Kurdish"},
	{103,	"lo",	"Lao"},
	{104,	"la",	"Latin"},
	{105,	"lv",	"Latvian"},
	{106,	"li",	"Limburgan; Limburger; Limburgish"},
	{107,	"ln",	"Lingala"},
	{108,	"lt",	"Lithuanian"},
	{109,	"lb",	"Luxembourgish; Letzeburgesch"},
	{110,	"lu",	"Luba-Katanga"},
	{111,	"lg",	"Ganda"},
	{112,	"mk",	"Macedonian"},
	{113,	"mh",	"Marshallese"},
	{114,	"ml",	"Malayalam"},
	{115,	"mi",	"Maori"},
	{116,	"mr",	"Marathi"},
	{117,	"ms",	"Malay"},
	{118,	"mk",	"Macedonian"},
	{119,	"mg",	"Malagasy"},
	{120,	"mt",	"Maltese"},
	{121,	"mo",	"Moldavian"},
	{122,	"mn",	"Mongolian"},
	{123,	"mi",	"Maori"},
	{124,	"ms",	"Malay"},
	{125,	"my",	"Burmese"},
	{126,	"na",	"Nauru"},
	{127,	"nv",	"Navajo; Navaho"},
	{128,	"nr",	"Ndebele, South; South Ndebele"},
	{129,	"nd",	"Ndebele, North; North Ndebele"},
	{130,	"ng",	"Ndonga"},
	{131,	"ne",	"Nepali"},
	{132,	"nl",	"Dutch; Flemish"},
	{133,	"nn",	"Norwegian Nynorsk; Nynorsk, Norwegian"},
	{134,	"nb",	"Bokmål, Norwegian; Norwegian Bokmål"},
	{135,	"no",	"Norwegian"},
	{136,	"ny",	"Chichewa; Chewa; Nyanja"},
	{137,	"oc",	"Occitan (post 1500); Provençal"},
	{138,	"oj",	"Ojibwa"},
	{139,	"or",	"Oriya"},
	{140,	"om",	"Oromo"},
	{141,	"os",	"Ossetian; Ossetic"},
	{142,	"pa",	"Panjabi; Punjabi"},
	{143,	"fa",	"Persian"},
	{144,	"pi",	"Pali"},
	{145,	"pl",	"Polish"},
	{146,	"pt",	"Portuguese"},
	{147,	"ps",	"Pushto"},
	{148,	"qu",	"Quechua"},
	{149,	"rm",	"Romansh"},
	{150,	"ro",	"Romanian"},
	{151,	"rn",	"Rundi"},
	{152,	"ru",	"Russian"},
	{153,	"sg",	"Sango"},
	{154,	"sa",	"Sanskrit"},
	{155,	"sr",	"Serbian"},
	{156,	"hr",	"Croatian"},
	{157,	"si",	"Sinhala; Sinhalese"},
	{158,	"sk",	"Slovak"},
	{159,	"sl",	"Slovenian"},
	{160,	"se",	"Northern Sami"},
	{161,	"sm",	"Samoan"},
	{162,	"sn",	"Shona"},
	{163,	"sd",	"Sindhi"},
	{164,	"so",	"Somali"},
	{165,	"st",	"Sotho, Southern"},
	{166,	"es",	"Spanish; Castilian"},
	{167,	"sq",	"Albanian"},
	{168,	"sc",	"Sardinian"},
	{169,	"sr",	"Serbian"},
	{170,	"ss",	"Swati"},
	{171,	"su",	"Sundanese"},
	{172,	"sw",	"Swahili"},
	{173,	"sv",	"Swedish"},
	{174,	"ty",	"Tahitian"},
	{175,	"ta",	"Tamil"},
	{176,	"tt",	"Tatar"},
	{177,	"te",	"Telugu"},
	{178,	"tg",	"Tajik"},
	{179,	"tl",	"Tagalog"},
	{180,	"th",	"Thai"},
	{181,	"bo",	"Tibetan"},
	{182,	"ti",	"Tigrinya"},
	{183,	"to",	"Tonga (Tonga Islands)"},
	{184,	"tn",	"Tswana"},
	{185,	"ts",	"Tsonga"},
	{186,	"tk",	"Turkmen"},
	{187,	"tr",	"Turkish"},
	{188,	"tw",	"Twi"},
	{189,	"ug",	"Uighur; Uyghur"},
	{190,	"uk",	"Ukrainian"},
	{191,	"ur",	"Urdu"},
	{192,	"uz",	"Uzbek"},
	{193,	"ve",	"Venda"},
	{194,	"vi",	"Vietnamese"},
	{195,	"vo",	"Volapük"},
	{196,	"cy",	"Welsh"},
	{197,	"wa",	"Walloon"},
	{198,	"wo",	"Wolof"},
	{199,	"xh",	"Xhosa"},
	{200,	"yi",	"Yiddish"},
	{201,	"yo",	"Yoruba"},
	{202,	"za",	"Zhuang; Chuang"},
	{203,	"zh",	"Chinese"},
	{204,	"zu",	"Zulu"}
	};
/*-----------------------------------------------------------------------------------*/ 
static char CURRENTLANGUAGESTRING[LengthAlpha2code]="en";
static int  CURRENTLANGUAGECODE=45;
/*-----------------------------------------------------------------------------------*/ 
static BOOL LanguageIsOK(char *lang);
static int FindLanguageCode(char *lang);
static BOOL setlanguagecode(char *lang);
/*-----------------------------------------------------------------------------------*/ 
BOOL setlanguage(char *lang)
{
	BOOL bOK=FALSE;
	if (lang)
	{
		if ( LanguageIsOK(lang) )
		{
			strcpy(CURRENTLANGUAGESTRING,lang);
			setlanguagecode(lang);
			bOK=TRUE;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
char *getlanguage(void)
{
	char *RetLanguage=NULL;

	RetLanguage=(char*)MALLOC(sizeof(char)*(strlen(CURRENTLANGUAGESTRING)+1));
	strcpy(RetLanguage,CURRENTLANGUAGESTRING);

	return RetLanguage;
}
/*-----------------------------------------------------------------------------------*/ 
int getcurrentlanguagecode(void)
{
	return CURRENTLANGUAGECODE;
}
/*-----------------------------------------------------------------------------------*/ 
static BOOL LanguageIsOK(char *lang)
{
	BOOL bOK=FALSE;
	int i=0;

	for (i=0;i<NumberLanguages;i++)
	{
		if (strcmp(lang,TabLanguages[i].alpha2code)==0)
		{
			bOK=TRUE;
			return bOK;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
static int FindLanguageCode(char *lang)
{
	int Code=-1;
	int i=0;

	for (i=0;i<NumberLanguages;i++)
	{
		if (strcmp(lang,TabLanguages[i].alpha2code)==0)
		{
			Code=TabLanguages[i].code;
			return Code;
		}
	}
	return Code;
}
/*-----------------------------------------------------------------------------------*/ 
static BOOL setlanguagecode(char *lang)
{
	BOOL bOK=FALSE;
	int tmpCode=FindLanguageCode(lang);
	
	if (tmpCode>0) 
	{
		CURRENTLANGUAGECODE=tmpCode;
		bOK=TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
