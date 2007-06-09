/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "org_scilab_modules_localization_QueryString.h"
#include "QueryStringError.h"
#include "QueryStringMenu.h"
#include "QueryStringMessage.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
JNIEXPORT jstring JNICALL Java_org_scilab_modules_localization_QueryString_error
(JNIEnv *env, jclass cl, jstring tag)
{
	const char *ctag = NULL;
	char *LocalizedString = NULL;
	jstring StrReturn;

	ctag = (*env)->GetStringUTFChars(env, tag, NULL);
	LocalizedString=QueryStringError((char*)ctag);

	StrReturn = (*env)->NewStringUTF(env, LocalizedString);
	if (LocalizedString) { FREE(LocalizedString);LocalizedString = NULL; }

	return StrReturn;
}  
/*-----------------------------------------------------------------------------------*/ 
JNIEXPORT jstring JNICALL Java_org_scilab_modules_localization_QueryString_message
(JNIEnv *env, jclass cl, jstring tag)
{
	const char *ctag = NULL;
	char *LocalizedString = NULL;
	jstring StrReturn;

	ctag = (*env)->GetStringUTFChars(env, tag, NULL);
	LocalizedString=QueryStringMessage((char*)ctag);

	StrReturn = (*env)->NewStringUTF(env, LocalizedString);
	if (LocalizedString) { FREE(LocalizedString);LocalizedString = NULL; }

	return StrReturn;
}
/*-----------------------------------------------------------------------------------*/ 
JNIEXPORT jstring JNICALL Java_org_scilab_modules_localization_QueryString_menu
(JNIEnv *env, jclass cl, jstring tag)
{
	const char *ctag = NULL;
	char *LocalizedString = NULL;
	jstring StrReturn;

	ctag = (*env)->GetStringUTFChars(env, tag, NULL);
	LocalizedString=QueryStringMenu((char*)ctag);

	StrReturn = (*env)->NewStringUTF(env, LocalizedString);
	if (LocalizedString) { FREE(LocalizedString);LocalizedString = NULL; }

	return StrReturn;
}
/*-----------------------------------------------------------------------------------*/ 


