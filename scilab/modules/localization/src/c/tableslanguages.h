/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
    const wchar_t *alphacode;
    const wchar_t *languagedescription;

} LanguagesTable;
/*--------------------------------------------------------------------------*/
typedef struct languagesalias_struct
{
    const wchar_t *alias;
    const wchar_t *alphacode;

} LanguagesTableAlias;
/*--------------------------------------------------------------------------*/
#define NumberLanguagesAlias 42
static LanguagesTableAlias LANGUAGE_LOCALE_ALIAS[NumberLanguagesAlias] =
{
    {L"bg", L"bg_BG"},
    {L"ca", L"ca_ES"},
    {L"cn", L"zh_CN"},
    {L"cs", L"cs_CZ"},
    {L"cy", L"cy_GB"},
    {L"da", L"da_DK"},
    {L"de", L"de_DE"},
    {L"el", L"el_GR"},
    {L"en", L"C"}, /* When the user is requesting english ... switch back to the default */
    {L"en_US", L"C"},
    {L"es", L"es_ES"},
    {L"et", L"et_EE"},
    {L"eu", L"eu_ES"},
    {L"fa", L"fa_IR"},
    {L"fi", L"fi_FI"},
    {L"fo", L"fo_FO"},
    {L"fr", L"fr_FR"},
    {L"he", L"he_IL"},
    {L"hr", L"hr_HR"},
    {L"hu", L"hu_HU"},
    {L"id", L"id_ID"},
    {L"is", L"is_IS"},
    {L"it", L"it_IT"},
    {L"ja", L"ja_JP"},
    {L"ko", L"ko_KR"},
    {L"lt", L"lt_LT"},
    {L"my", L"ms_MY"},
    {L"nb", L"nb_NO"},
    {L"nl", L"nl_NL"},
    {L"pl", L"pl_PL"},
    {L"pt", L"pt_PT"},
    {L"ro", L"ro_RO"},
    {L"ru", L"ru_RU"},
    {L"sk", L"sk_SK"},
    {L"sl", L"sl_SI"},
    {L"sr", L"sr_YU"},
    {L"sv", L"sv_SE"},
    {L"th", L"th_TH"},
    {L"tl", L"tl_PH"},
    {L"tr", L"tr_TR"},
    {L"tw", L"zh_TW"},
    {L"uk", L"uk_UA"}
};
/*--------------------------------------------------------------------------*/
#define NumberLanguages 220
static LanguagesTable LANGUAGE_COUNTRY_TAB[NumberLanguages] =
{
    {  1, L"aa_DJ", L"Afar (Djibouti)"},
    {  2, L"aa_ER", L"Afar (Eritrea)"},
    {  3, L"aa_ER_SAAHO", L"Afar (Eritrea, SAAHO)"},
    {  4, L"aa_ET", L"Afar (Ethiopia)"},
    {  5, L"af_ZA", L"Afrikaans (South Africa)"},
    {  6, L"sq_AL", L"Albanian (Albania)"},
    {  7, L"am_ET", L"Amharic (Ethiopia)"},
    {  8, L"ar_DZ", L"Arabic (Algeria)"},
    {  9, L"ar_BH", L"Arabic (Bahrain)"},
    { 10, L"ar_EG", L"Arabic (Egypt)"},
    { 11, L"ar_IQ", L"Arabic (Iraq)"},
    { 12, L"ar_JO", L"Arabic (Jordan)"},
    { 13, L"ar_KW", L"Arabic (Kuwait)"},
    { 14, L"ar_LB", L"Arabic (Lebanon)"},
    { 15, L"ar_LY", L"Arabic (Libya)"},
    { 16, L"ar_MA", L"Arabic (Morocco)"},
    { 17, L"ar_OM", L"Arabic (Oman)"},
    { 18, L"ar_QA", L"Arabic (Qatar)"},
    { 19, L"ar_SA", L"Arabic (Saudi Arabia)"},
    { 20, L"ar_SD", L"Arabic (Sudan)"},
    { 21, L"ar_SY", L"Arabic (Syria)"},
    { 22, L"ar_TN", L"Arabic (Tunisia)"},
    { 23, L"ar_AE", L"Arabic (United Arab Emirates)"},
    { 24, L"ar_YE", L"Arabic (Yemen)"},
    { 25, L"hy_AM", L"Armenian (Armenia)"},
    { 26, L"hy_AM_REVISED", L"Armenian (Armenia, Revised Orthography)"},
    { 27, L"as_IN", L"Assamese (India)"},
    { 28, L"az_AZ", L"Azerbaijani (Azerbaijan)"},
    { 29, L"az_Cyrl_AZ", L"Azerbaijani (Cyrillic, Azerbaijan)"},
    { 30, L"az_Latn_AZ", L"Azerbaijani (Latin, Azerbaijan)"},
    { 31, L"az_Cyrl", L"Azerbaijani (Cyrillic)"},
    { 32, L"az_Latn", L"Azerbaijani (Latin)"},
    { 33, L"eu_ES", L"Basque (Spain)"},
    { 34, L"be_BY", L"Belarusian (Belarus)"},
    { 35, L"bn_IN", L"Bengali (India)"},
    { 36, L"byn_ER", L"Blin (Eritrea)"},
    { 37, L"bs_BA", L"Bosnian (Bosnia and Herzegovina)"},
    { 38, L"bg_BG", L"Bulgarian (Bulgaria)"},
    { 39, L"ca_ES", L"Catalan (Spain)"},
    { 40, L"zh_CN", L"Chinese (China)"},
    { 41, L"zh_Hans_CN", L"Chinese (Simplified Han, China)"},
    { 42, L"zh_HK", L"Chinese (Hong Kong S.A.R., China)"},
    { 43, L"zh_Hant_HK", L"Chinese (Traditional Han, Hong Kong S.A.R., China)"},
    { 44, L"zh_Hant_MO", L"Chinese (Traditional Han, Macao S.A.R., China)"},
    { 45, L"zh_MO", L"Chinese (Macao S.A.R., China)"},
    { 46, L"zh_Hans_SG", L"Chinese (Simplified Han, Singapore)"},
    { 47, L"zh_SG", L"Chinese (Singapore)"},
    { 48, L"zh_Hant_TW", L"Chinese (Traditional Han, Taiwan)"},
    { 49, L"zh_TW", L"Chinese (Taiwan)"},
    { 50, L"zh_Hans", L"Chinese (Simplified Han)"},
    { 51, L"zh_Hant", L"Chinese (Traditional Han)"},
    { 52, L"kw_GB", L"Cornish (United Kingdom)"},
    { 53, L"hr_HR", L"Croatian (Croatia)"},
    { 54, L"cs_CZ", L"Czech (Czech Republic)"},
    { 55, L"da_DK", L"Danish (Denmark)"},
    { 56, L"dv_MV", L"Divehi (Maldives)"},
    { 57, L"nl_BE", L"Dutch (Belgium)"},
    { 58, L"nl_NL", L"Dutch (Netherlands)"},
    { 59, L"dz_BT", L"Dzongkha (Bhutan)"},
    { 60, L"en_AS", L"English (American Samoa)"},
    { 61, L"en_AU", L"English (Australia)"},
    { 62, L"en_BE", L"English (Belgium)"},
    { 63, L"en_BZ", L"English (Belize)"},
    { 64, L"en_BW", L"English (Botswana)"},
    { 65, L"en_CA", L"English (Canada)"},
    { 66, L"en_GU", L"English (Guam)"},
    { 67, L"en_HK", L"English (Hong Kong S.A.R., China)"},
    { 68, L"en_IN", L"English (India)"},
    { 69, L"en_IE", L"English (Ireland)"},
    { 70, L"en_JM", L"English (Jamaica)"},
    { 71, L"en_MT", L"English (Malta)"},
    { 72, L"en_MH", L"English (Marshall Islands)"},
    { 73, L"en_NZ", L"English (New Zealand)"},
    { 74, L"en_MP", L"English (Northern Mariana Islands)"},
    { 75, L"en_PK", L"English (Pakistan)"},
    { 76, L"en_PH", L"English (Philippines)"},
    { 77, L"en_SG", L"English (Singapore)"},
    { 78, L"en_ZA", L"English (South Africa)"},
    { 79, L"en_TT", L"English (Trinidad and Tobago)"},
    { 80, L"en_VI", L"English (U.S. Virgin Islands)"},
    { 81, L"en_GB", L"English (United Kingdom)"},
    { 82, L"en_UM", L"English (United States Minor Outlying Islands)"},
    { 83, L"en_US", L"English (United States)"},
    { 84, L"en_US_POSIX", L"English (United States, Computer)"},
    { 85, L"en_ZW", L"English (Zimbabwe)"},
    { 86, L"et_EE", L"Estonian (Estonia)"},
    { 87, L"fo_FO", L"Faroese (Faroe Islands)"},
    { 88, L"fi_FI", L"Finnish (Finland)"},
    { 89, L"fr_BE", L"French (Belgium)"},
    { 90, L"fr_CA", L"French (Canada)"},
    { 91, L"fr_FR", L"French (France)"},
    { 92, L"fr_LU", L"French (Luxembourg)"},
    { 93, L"fr_MC", L"French (Monaco)"},
    { 94, L"fr_CH", L"French (Switzerland)"},
    { 95, L"gl_ES", L"Gallegan (Spain)"},
    { 96, L"gez_ER", L"Geez (Eritrea)"},
    { 97, L"gez_ET", L"Geez (Ethiopia)"},
    { 98, L"ka_GE", L"Georgian (Georgia)"},
    { 99, L"de_AT", L"German (Austria)"},
    { 100, L"de_BE", L"German (Belgium)"},
    { 101, L"de_DE", L"German (Germany)"},
    { 102, L"de_LI", L"German (Liechtenstein)"},
    { 103, L"de_LU", L"German (Luxembourg)"},
    { 104, L"de_CH", L"German (Switzerland)"},
    { 105, L"el_CY", L"Greek (Cyprus)"},
    { 106, L"el_GR", L"Greek (Greece)"},
    { 107, L"el_POLYTONI", L"Greek (POLYTONI)"},
    { 108, L"gu_IN", L"Gujarati (India)"},
    { 109, L"haw_US", L"Hawaiian (United States)"},
    { 110, L"he_IL", L"Hebrew (Israel)"},
    { 111, L"hi_IN", L"Hindi (India)"},
    { 112, L"hu_HU", L"Hungarian (Hungary)"},
    { 113, L"is_IS", L"Icelandic (Iceland)"},
    { 114, L"id_ID", L"Indonesian (Indonesia)"},
    { 115, L"ga_IE", L"Irish (Ireland)"},
    { 116, L"it_IT", L"Italian (Italy)"},
    { 117, L"it_CH", L"Italian (Switzerland)"},
    { 118, L"ja_JP", L"Japanese (Japan)"},
    { 119, L"kl_GL", L"Kalaallisut (Greenland)"},
    { 120, L"kn_IN", L"Kannada (India)"},
    { 121, L"kk_KZ", L"Kazakh (Kazakhstan)"},
    { 122, L"km_KH", L"Khmer (Cambodia)"},
    { 123, L"ky_KG", L"Kirghiz (Kyrgyzstan)"},
    { 124, L"kok_IN", L"Konkani (India)"},
    { 125, L"ko_KR", L"Korean (South Korea)"},
    { 126, L"lo_LA", L"Lao (Laos)"},
    { 127, L"lv_LV", L"Latvian (Latvia)"},
    { 128, L"lt_LT", L"Lithuanian (Lithuania)"},
    { 129, L"mk_MK", L"Macedonian (Macedonia)"},
    { 130, L"ms_BN", L"Malay (Brunei)"},
    { 131, L"ms_MY", L"Malay (Malaysia)"},
    { 132, L"ml_IN", L"Malayalam (India)"},
    { 133, L"mt_MT", L"Maltese (Malta)"},
    { 134, L"gv_GB", L"Manx (United Kingdom)"},
    { 135, L"mr_IN", L"Marathi (India)"},
    { 136, L"mn_MN", L"Mongolian (Mongolia)"},
    { 137, L"nb_NO", L"Norwegian Bokmål (Norway)"},
    { 138, L"nn_NO", L"Norwegian Nynorsk (Norway)"},
    { 139, L"or_IN", L"Oriya (India)"},
    { 140, L"om_ET", L"Oromo (Ethiopia)"},
    { 141, L"om_KE", L"Oromo (Kenya)"},
    { 142, L"ps_AF", L"Pashto (Pushto) (Afghanistan)"},
    { 143, L"fa_AF", L"Persian (Afghanistan)"},
    { 144, L"fa_IR", L"Persian (Iran)"},
    { 145, L"pl_PL", L"Polish (Poland)"},
    { 146, L"pt_BR", L"Portuguese (Brazil)"},
    { 147, L"pt_PT", L"Portuguese (Portugal)"},
    { 148, L"pa_IN", L"Punjabi (India)"},
    { 149, L"ro_RO", L"Romanian (Romania)"},
    { 150, L"ru_RU", L"Russian (Russia)"},
    { 151, L"ru_UA", L"Russian (Ukraine)"},
    { 152, L"sa_IN", L"Sanskrit (India)"},
    { 153, L"sr_BA", L"Serbian (Bosnia and Herzegovina)"},
    { 154, L"sr_Cyrl_BA", L"Serbian (Cyrillic, Bosnia and Herzegovina)"},
    { 155, L"sr_Latn_BA", L"Serbian (Latin, Bosnia and Herzegovina)"},
    { 156, L"sr_CS", L"Serbian (Serbia And Montenegro)"},
    { 157, L"sr_Cyrl_CS", L"Serbian (Cyrillic, Serbia And Montenegro)"},
    { 158, L"sr_Latn_CS", L"Serbian (Latin, Serbia And Montenegro)"},
    { 159, L"sr_Cyrl_YU", L"Serbian (Cyrillic, YU)"},
    { 160, L"sr_Latn_YU", L"Serbian (Latin, YU)"},
    { 161, L"sr_YU", L"Serbian (YU)"},
    { 162, L"sr_Cyrl", L"Serbian (Cyrillic)"},
    { 163, L"sr_Latn", L"Serbian (Latin)"},
    { 164, L"sh_BA", L"Serbo-Croatian (Bosnia and Herzegovina)"},
    { 165, L"sh_CS", L"Serbo-Croatian (Serbia And Montenegro)"},
    { 166, L"sh_YU", L"Serbo-Croatian (YU)"},
    { 167, L"sid_ET", L"Sidamo (Ethiopia)"},
    { 168, L"sk_SK", L"Slovak (Slovakia)"},
    { 169, L"sl_SI", L"Slovenian (Slovenia)"},
    { 170, L"so_DJ", L"Somali (Djibouti)"},
    { 171, L"so_ET", L"Somali (Ethiopia)"},
    { 172, L"so_KE", L"Somali (Kenya)"},
    { 173, L"so_SO", L"Somali (Somalia)"},
    { 174, L"es_AR", L"Spanish (Argentina)"},
    { 175, L"es_BO", L"Spanish (Bolivia)"},
    { 176, L"es_CL", L"Spanish (Chile)"},
    { 177, L"es_CO", L"Spanish (Colombia)"},
    { 178, L"es_CR", L"Spanish (Costa Rica)"},
    { 179, L"es_DO", L"Spanish (Dominican Republic)"},
    { 180, L"es_EC", L"Spanish (Ecuador)"},
    { 181, L"es_SV", L"Spanish (El Salvador)"},
    { 182, L"es_GT", L"Spanish (Guatemala)"},
    { 183, L"es_HN", L"Spanish (Honduras)"},
    { 184, L"es_MX", L"Spanish (Mexico)"},
    { 185, L"es_NI", L"Spanish (Nicaragua)"},
    { 186, L"es_PA", L"Spanish (Panama)"},
    { 187, L"es_PY", L"Spanish (Paraguay)"},
    { 188, L"es_PE", L"Spanish (Peru)"},
    { 189, L"es_PR", L"Spanish (Puerto Rico)"},
    { 190, L"es_ES", L"Spanish (Spain)"},
    { 191, L"es_US", L"Spanish (United States)"},
    { 192, L"es_UY", L"Spanish (Uruguay)"},
    { 193, L"es_VE", L"Spanish (Venezuela)"},
    { 194, L"sw_KE", L"Swahili (Kenya)"},
    { 195, L"sw_TZ", L"Swahili (Tanzania)"},
    { 196, L"sv_FI", L"Swedish (Finland)"},
    { 197, L"sv_SE", L"Swedish (Sweden)"},
    { 198, L"syr_SY", L"Syriac (Syria)"},
    { 199, L"ta_IN", L"Tamil (India)"},
    { 200, L"tt_RU", L"Tatar (Russia)"},
    { 201, L"te_IN", L"Telugu (India)"},
    { 202, L"th_TH", L"Thai (Thailand)"},
    { 203, L"tig_ER", L"Tigre (Eritrea)"},
    { 204, L"ti_ER", L"Tigrinya (Eritrea)"},
    { 205, L"ti_ET", L"Tigrinya (Ethiopia)"},
    { 206, L"tr_TR", L"Turkish (Turkey)"},
    { 207, L"uk_UA", L"Ukrainian (Ukraine)"},
    { 208, L"ur_PK", L"Urdu (Pakistan)"},
    { 209, L"uz_AF", L"Uzbek (Afghanistan)"},
    { 210, L"uz_Arab_AF", L"Uzbek (Arabic, Afghanistan)"},
    { 211, L"uz_Cyrl_UZ", L"Uzbek (Cyrillic, Uzbekistan)"},
    { 212, L"uz_Latn_UZ", L"Uzbek (Latin, Uzbekistan)"},
    { 213, L"uz_UZ", L"Uzbek (Uzbekistan)"},
    { 214, L"uz_Arab", L"Uzbek (Arabic)"},
    { 215, L"uz_Cyrl", L"Uzbek (Cyrillic)"},
    { 216, L"uz_Latn", L"Uzbek (Latin)"},
    { 217, L"vi_VN", L"Vietnamese (Vietnam)"},
    { 218, L"wal_ET", L"Walamo (Ethiopia)"},
    { 219, L"cy_GB", L"Welsh (United Kingdom)"},
    { 220, L"C", L"English (Default)"}
};
/*--------------------------------------------------------------------------*/
// Charsets to test when they are mandatory (e.g., on Debian fr_FR is not
// accepted but fr_FR.UTF-8 is).
// These two charsets are currently supported by Scilab, this is why there are
// no other ones.
#define NumberOfCharsets 2
static const char* CHARSETS[NumberOfCharsets] =
{
    "UTF-8",
    "ISO-8859-1"
};

/*--------------------------------------------------------------------------*/
#endif /* __TABLESLANGUAGES_H__ */
/*--------------------------------------------------------------------------*/
