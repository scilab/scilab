/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __TABLESLANGUAGES_H__
#define __TABLESLANGUAGES_H__
/*--------------------------------------------------------------------------*/
/* See :
	http://www.unicode.org/cldr
	http://unicode.org/cldr/data/diff/main/index.html
	http://inter-locale.com/ID/draft-ietf-ltru-registry-14.html
*/
/*--------------------------------------------------------------------------*/
#define LengthAlphacode 32
#define LengthAlpha2code 3
#define LengthLanguage 64
typedef struct languages_struct
{
    int code;
    char alphacode[LengthAlphacode];
    char languagedescription[LengthLanguage];

} LanguagesTable;
/*--------------------------------------------------------------------------*/
typedef struct languagesalias_struct
{
    char alias[LengthAlphacode];
    char alphacode[LengthAlphacode];

} LanguagesTableAlias;
/*--------------------------------------------------------------------------*/
#define NumberLanguagesAlias 42
static LanguagesTableAlias LANGUAGE_LOCALE_ALIAS[NumberLanguagesAlias] =
{
    {"bg", "bg_BG"},
    {"ca", "ca_ES"},
    {"cn", "zh_CN"},
    {"cs", "cs_CZ"},
    {"cy", "cy_GB"},
    {"da", "da_DK"},
    {"de", "de_DE"},
    {"el", "el_GR"},
    {"en", "C"}, /* When the user is requesting english ... switch back to the default */
    {"en_US", "C"},
    {"es", "es_ES"},
    {"et", "et_EE"},
    {"eu", "eu_ES"},
    {"fa", "fa_IR"},
    {"fi", "fi_FI"},
    {"fo", "fo_FO"},
    {"fr", "fr_FR"},
    {"he", "he_IL"},
    {"hr", "hr_HR"},
    {"hu", "hu_HU"},
    {"id", "id_ID"},
    {"is", "is_IS"},
    {"it", "it_IT"},
    {"ja", "ja_JP"},
    {"ko", "ko_KR"},
    {"lt", "lt_LT"},
    {"my", "ms_MY"},
    {"nb", "nb_NO"},
    {"nl", "nl_NL"},
    {"pl", "pl_PL"},
    {"pt", "pt_PT"},
    {"ro", "ro_RO"},
    {"ru", "ru_RU"},
    {"sk", "sk_SK"},
    {"sl", "sl_SI"},
    {"sr", "sr_YU"},
    {"sv", "sv_SE"},
    {"th", "th_TH"},
    {"tl", "tl_PH"},
    {"tr", "tr_TR"},
    {"tw", "zh_TW"},
    {"uk", "uk_UA"}
};
/*--------------------------------------------------------------------------*/
#define NumberLanguages 220
static LanguagesTable LANGUAGE_COUNTRY_TAB[NumberLanguages] =
{
    {  1, "aa_DJ", "Afar (Djibouti)"},
    {  2, "aa_ER", "Afar (Eritrea)"},
    {  3, "aa_ER_SAAHO", "Afar (Eritrea, SAAHO)"},
    {  4, "aa_ET", "Afar (Ethiopia)"},
    {  5, "af_ZA", "Afrikaans (South Africa)"},
    {  6, "sq_AL", "Albanian (Albania)"},
    {  7, "am_ET", "Amharic (Ethiopia)"},
    {  8, "ar_DZ", "Arabic (Algeria)"},
    {  9, "ar_BH", "Arabic (Bahrain)"},
    { 10, "ar_EG", "Arabic (Egypt)"},
    { 11, "ar_IQ", "Arabic (Iraq)"},
    { 12, "ar_JO", "Arabic (Jordan)"},
    { 13, "ar_KW", "Arabic (Kuwait)"},
    { 14, "ar_LB", "Arabic (Lebanon)"},
    { 15, "ar_LY", "Arabic (Libya)"},
    { 16, "ar_MA", "Arabic (Morocco)"},
    { 17, "ar_OM", "Arabic (Oman)"},
    { 18, "ar_QA", "Arabic (Qatar)"},
    { 19, "ar_SA", "Arabic (Saudi Arabia)"},
    { 20, "ar_SD", "Arabic (Sudan)"},
    { 21, "ar_SY", "Arabic (Syria)"},
    { 22, "ar_TN", "Arabic (Tunisia)"},
    { 23, "ar_AE", "Arabic (United Arab Emirates)"},
    { 24, "ar_YE", "Arabic (Yemen)"},
    { 25, "hy_AM", "Armenian (Armenia)"},
    { 26, "hy_AM_REVISED", "Armenian (Armenia, Revised Orthography)"},
    { 27, "as_IN", "Assamese (India)"},
    { 28, "az_AZ", "Azerbaijani (Azerbaijan)"},
    { 29, "az_Cyrl_AZ", "Azerbaijani (Cyrillic, Azerbaijan)"},
    { 30, "az_Latn_AZ", "Azerbaijani (Latin, Azerbaijan)"},
    { 31, "az_Cyrl", "Azerbaijani (Cyrillic)"},
    { 32, "az_Latn", "Azerbaijani (Latin)"},
    { 33, "eu_ES", "Basque (Spain)"},
    { 34, "be_BY", "Belarusian (Belarus)"},
    { 35, "bn_IN", "Bengali (India)"},
    { 36, "byn_ER", "Blin (Eritrea)"},
    { 37, "bs_BA", "Bosnian (Bosnia and Herzegovina)"},
    { 38, "bg_BG", "Bulgarian (Bulgaria)"},
    { 39, "ca_ES", "Catalan (Spain)"},
    { 40, "zh_CN", "Chinese (China)"},
    { 41, "zh_Hans_CN", "Chinese (Simplified Han, China)"},
    { 42, "zh_HK", "Chinese (Hong Kong S.A.R., China)"},
    { 43, "zh_Hant_HK", "Chinese (Traditional Han, Hong Kong S.A.R., China)"},
    { 44, "zh_Hant_MO", "Chinese (Traditional Han, Macao S.A.R., China)"},
    { 45, "zh_MO", "Chinese (Macao S.A.R., China)"},
    { 46, "zh_Hans_SG", "Chinese (Simplified Han, Singapore)"},
    { 47, "zh_SG", "Chinese (Singapore)"},
    { 48, "zh_Hant_TW", "Chinese (Traditional Han, Taiwan)"},
    { 49, "zh_TW", "Chinese (Taiwan)"},
    { 50, "zh_Hans", "Chinese (Simplified Han)"},
    { 51, "zh_Hant", "Chinese (Traditional Han)"},
    { 52, "kw_GB", "Cornish (United Kingdom)"},
    { 53, "hr_HR", "Croatian (Croatia)"},
    { 54, "cs_CZ", "Czech (Czech Republic)"},
    { 55, "da_DK", "Danish (Denmark)"},
    { 56, "dv_MV", "Divehi (Maldives)"},
    { 57, "nl_BE", "Dutch (Belgium)"},
    { 58, "nl_NL", "Dutch (Netherlands)"},
    { 59, "dz_BT", "Dzongkha (Bhutan)"},
    { 60, "en_AS", "English (American Samoa)"},
    { 61, "en_AU", "English (Australia)"},
    { 62, "en_BE", "English (Belgium)"},
    { 63, "en_BZ", "English (Belize)"},
    { 64, "en_BW", "English (Botswana)"},
    { 65, "en_CA", "English (Canada)"},
    { 66, "en_GU", "English (Guam)"},
    { 67, "en_HK", "English (Hong Kong S.A.R., China)"},
    { 68, "en_IN", "English (India)"},
    { 69, "en_IE", "English (Ireland)"},
    { 70, "en_JM", "English (Jamaica)"},
    { 71, "en_MT", "English (Malta)"},
    { 72, "en_MH", "English (Marshall Islands)"},
    { 73, "en_NZ", "English (New Zealand)"},
    { 74, "en_MP", "English (Northern Mariana Islands)"},
    { 75, "en_PK", "English (Pakistan)"},
    { 76, "en_PH", "English (Philippines)"},
    { 77, "en_SG", "English (Singapore)"},
    { 78, "en_ZA", "English (South Africa)"},
    { 79, "en_TT", "English (Trinidad and Tobago)"},
    { 80, "en_VI", "English (U.S. Virgin Islands)"},
    { 81, "en_GB", "English (United Kingdom)"},
    { 82, "en_UM", "English (United States Minor Outlying Islands)"},
    { 83, "en_US", "English (United States)"},
    { 84, "en_US_POSIX", "English (United States, Computer)"},
    { 85, "en_ZW", "English (Zimbabwe)"},
    { 86, "et_EE", "Estonian (Estonia)"},
    { 87, "fo_FO", "Faroese (Faroe Islands)"},
    { 88, "fi_FI", "Finnish (Finland)"},
    { 89, "fr_BE", "French (Belgium)"},
    { 90, "fr_CA", "French (Canada)"},
    { 91, "fr_FR", "French (France)"},
    { 92, "fr_LU", "French (Luxembourg)"},
    { 93, "fr_MC", "French (Monaco)"},
    { 94, "fr_CH", "French (Switzerland)"},
    { 95, "gl_ES", "Gallegan (Spain)"},
    { 96, "gez_ER", "Geez (Eritrea)"},
    { 97, "gez_ET", "Geez (Ethiopia)"},
    { 98, "ka_GE", "Georgian (Georgia)"},
    { 99, "de_AT", "German (Austria)"},
    { 100, "de_BE", "German (Belgium)"},
    { 101, "de_DE", "German (Germany)"},
    { 102, "de_LI", "German (Liechtenstein)"},
    { 103, "de_LU", "German (Luxembourg)"},
    { 104, "de_CH", "German (Switzerland)"},
    { 105, "el_CY", "Greek (Cyprus)"},
    { 106, "el_GR", "Greek (Greece)"},
    { 107, "el_POLYTONI", "Greek (POLYTONI)"},
    { 108, "gu_IN", "Gujarati (India)"},
    { 109, "haw_US", "Hawaiian (United States)"},
    { 110, "he_IL", "Hebrew (Israel)"},
    { 111, "hi_IN", "Hindi (India)"},
    { 112, "hu_HU", "Hungarian (Hungary)"},
    { 113, "is_IS", "Icelandic (Iceland)"},
    { 114, "id_ID", "Indonesian (Indonesia)"},
    { 115, "ga_IE", "Irish (Ireland)"},
    { 116, "it_IT", "Italian (Italy)"},
    { 117, "it_CH", "Italian (Switzerland)"},
    { 118, "ja_JP", "Japanese (Japan)"},
    { 119, "kl_GL", "Kalaallisut (Greenland)"},
    { 120, "kn_IN", "Kannada (India)"},
    { 121, "kk_KZ", "Kazakh (Kazakhstan)"},
    { 122, "km_KH", "Khmer (Cambodia)"},
    { 123, "ky_KG", "Kirghiz (Kyrgyzstan)"},
    { 124, "kok_IN", "Konkani (India)"},
    { 125, "ko_KR", "Korean (South Korea)"},
    { 126, "lo_LA", "Lao (Laos)"},
    { 127, "lv_LV", "Latvian (Latvia)"},
    { 128, "lt_LT", "Lithuanian (Lithuania)"},
    { 129, "mk_MK", "Macedonian (Macedonia)"},
    { 130, "ms_BN", "Malay (Brunei)"},
    { 131, "ms_MY", "Malay (Malaysia)"},
    { 132, "ml_IN", "Malayalam (India)"},
    { 133, "mt_MT", "Maltese (Malta)"},
    { 134, "gv_GB", "Manx (United Kingdom)"},
    { 135, "mr_IN", "Marathi (India)"},
    { 136, "mn_MN", "Mongolian (Mongolia)"},
    { 137, "nb_NO", "Norwegian Bokmål (Norway)"},
    { 138, "nn_NO", "Norwegian Nynorsk (Norway)"},
    { 139, "or_IN", "Oriya (India)"},
    { 140, "om_ET", "Oromo (Ethiopia)"},
    { 141, "om_KE", "Oromo (Kenya)"},
    { 142, "ps_AF", "Pashto (Pushto) (Afghanistan)"},
    { 143, "fa_AF", "Persian (Afghanistan)"},
    { 144, "fa_IR", "Persian (Iran)"},
    { 145, "pl_PL", "Polish (Poland)"},
    { 146, "pt_BR", "Portuguese (Brazil)"},
    { 147, "pt_PT", "Portuguese (Portugal)"},
    { 148, "pa_IN", "Punjabi (India)"},
    { 149, "ro_RO", "Romanian (Romania)"},
    { 150, "ru_RU", "Russian (Russia)"},
    { 151, "ru_UA", "Russian (Ukraine)"},
    { 152, "sa_IN", "Sanskrit (India)"},
    { 153, "sr_BA", "Serbian (Bosnia and Herzegovina)"},
    { 154, "sr_Cyrl_BA", "Serbian (Cyrillic, Bosnia and Herzegovina)"},
    { 155, "sr_Latn_BA", "Serbian (Latin, Bosnia and Herzegovina)"},
    { 156, "sr_CS", "Serbian (Serbia And Montenegro)"},
    { 157, "sr_Cyrl_CS", "Serbian (Cyrillic, Serbia And Montenegro)"},
    { 158, "sr_Latn_CS", "Serbian (Latin, Serbia And Montenegro)"},
    { 159, "sr_Cyrl_YU", "Serbian (Cyrillic, YU)"},
    { 160, "sr_Latn_YU", "Serbian (Latin, YU)"},
    { 161, "sr_YU", "Serbian (YU)"},
    { 162, "sr_Cyrl", "Serbian (Cyrillic)"},
    { 163, "sr_Latn", "Serbian (Latin)"},
    { 164, "sh_BA", "Serbo-Croatian (Bosnia and Herzegovina)"},
    { 165, "sh_CS", "Serbo-Croatian (Serbia And Montenegro)"},
    { 166, "sh_YU", "Serbo-Croatian (YU)"},
    { 167, "sid_ET", "Sidamo (Ethiopia)"},
    { 168, "sk_SK", "Slovak (Slovakia)"},
    { 169, "sl_SI", "Slovenian (Slovenia)"},
    { 170, "so_DJ", "Somali (Djibouti)"},
    { 171, "so_ET", "Somali (Ethiopia)"},
    { 172, "so_KE", "Somali (Kenya)"},
    { 173, "so_SO", "Somali (Somalia)"},
    { 174, "es_AR", "Spanish (Argentina)"},
    { 175, "es_BO", "Spanish (Bolivia)"},
    { 176, "es_CL", "Spanish (Chile)"},
    { 177, "es_CO", "Spanish (Colombia)"},
    { 178, "es_CR", "Spanish (Costa Rica)"},
    { 179, "es_DO", "Spanish (Dominican Republic)"},
    { 180, "es_EC", "Spanish (Ecuador)"},
    { 181, "es_SV", "Spanish (El Salvador)"},
    { 182, "es_GT", "Spanish (Guatemala)"},
    { 183, "es_HN", "Spanish (Honduras)"},
    { 184, "es_MX", "Spanish (Mexico)"},
    { 185, "es_NI", "Spanish (Nicaragua)"},
    { 186, "es_PA", "Spanish (Panama)"},
    { 187, "es_PY", "Spanish (Paraguay)"},
    { 188, "es_PE", "Spanish (Peru)"},
    { 189, "es_PR", "Spanish (Puerto Rico)"},
    { 190, "es_ES", "Spanish (Spain)"},
    { 191, "es_US", "Spanish (United States)"},
    { 192, "es_UY", "Spanish (Uruguay)"},
    { 193, "es_VE", "Spanish (Venezuela)"},
    { 194, "sw_KE", "Swahili (Kenya)"},
    { 195, "sw_TZ", "Swahili (Tanzania)"},
    { 196, "sv_FI", "Swedish (Finland)"},
    { 197, "sv_SE", "Swedish (Sweden)"},
    { 198, "syr_SY", "Syriac (Syria)"},
    { 199, "ta_IN", "Tamil (India)"},
    { 200, "tt_RU", "Tatar (Russia)"},
    { 201, "te_IN", "Telugu (India)"},
    { 202, "th_TH", "Thai (Thailand)"},
    { 203, "tig_ER", "Tigre (Eritrea)"},
    { 204, "ti_ER", "Tigrinya (Eritrea)"},
    { 205, "ti_ET", "Tigrinya (Ethiopia)"},
    { 206, "tr_TR", "Turkish (Turkey)"},
    { 207, "uk_UA", "Ukrainian (Ukraine)"},
    { 208, "ur_PK", "Urdu (Pakistan)"},
    { 209, "uz_AF", "Uzbek (Afghanistan)"},
    { 210, "uz_Arab_AF", "Uzbek (Arabic, Afghanistan)"},
    { 211, "uz_Cyrl_UZ", "Uzbek (Cyrillic, Uzbekistan)"},
    { 212, "uz_Latn_UZ", "Uzbek (Latin, Uzbekistan)"},
    { 213, "uz_UZ", "Uzbek (Uzbekistan)"},
    { 214, "uz_Arab", "Uzbek (Arabic)"},
    { 215, "uz_Cyrl", "Uzbek (Cyrillic)"},
    { 216, "uz_Latn", "Uzbek (Latin)"},
    { 217, "vi_VN", "Vietnamese (Vietnam)"},
    { 218, "wal_ET", "Walamo (Ethiopia)"},
    { 219, "cy_GB", "Welsh (United Kingdom)"},
    { 220, "C", "English (Default)"}
};
/*--------------------------------------------------------------------------*/
// Charsets to test when they are mandatory (e.g., on Debian fr_FR is not
// accepted but fr_FR.UTF-8 is).
// These two charsets are currently supported by Scilab, this is why there are
// no other ones.
#define NumberOfCharsets 2
static char * CHARSETS[NumberOfCharsets] =
{
    "UTF-8",
    "ISO-8859-1"
};
/*--------------------------------------------------------------------------*/
#endif /* __TABLESLANGUAGES_H__ */
/*--------------------------------------------------------------------------*/
