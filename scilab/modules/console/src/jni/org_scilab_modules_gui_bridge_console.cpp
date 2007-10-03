#include "org_scilab_modules_gui_bridge_console.hxx"


#include <string>
#include <iostream>
#include <stdlib.h>
#include <jni.h>

namespace org_scilab_modules_gui_bridge_console {


// Returns the current env

JNIEnv * SwingScilabConsole::getCurrentEnv() {
JNIEnv * curEnv = NULL;
this->jvm->AttachCurrentThread((void **) &curEnv, NULL);
return curEnv;
}
// Destructor

SwingScilabConsole::~SwingScilabConsole() {
JNIEnv * curEnv = NULL;
this->jvm->AttachCurrentThread((void **) &curEnv, NULL);

curEnv->DeleteGlobalRef(this->instance);
curEnv->DeleteGlobalRef(this->instanceClass);
}

// Constructors

SwingScilabConsole::SwingScilabConsole(JavaVM * jvm_) {
jmethodID constructObject = NULL ;
jobject localInstance ;
jclass localClass ;
const std::string className="org/scilab/modules/gui/bridge/console/SwingScilabConsole";
const std::string construct="<init>";
const std::string param="()V";
jvm=jvm_;

JNIEnv * curEnv = getCurrentEnv();

localClass = curEnv->FindClass( className.c_str() ) ;
if (localClass == NULL) {
std::cerr << "Could not get the Class " << className <<  std::endl;
exit(EXIT_FAILURE);
}

this->instanceClass = (jclass) curEnv->NewGlobalRef(localClass) ;
if (this->instanceClass == NULL) {
std::cerr << "Could not create a Global Ref of " << className <<  std::endl;
exit(EXIT_FAILURE);
}

constructObject = curEnv->GetMethodID( this->instanceClass, construct.c_str() , param.c_str() ) ;
if(constructObject == NULL){
std::cerr << "Could not retrieve the constructor of the class " << className << " with the profile : " << construct << param << std::endl;
exit(EXIT_FAILURE);
}

localInstance = curEnv->NewObject( this->instanceClass, constructObject ) ;
if(localInstance == NULL){
std::cerr << "Could not instance the object " << className << " with the constructor : " << construct << param << std::endl;
exit(EXIT_FAILURE);
}
 
this->instance = curEnv->NewGlobalRef(localInstance) ;
if(this->instance == NULL){
std::cerr << "Could not create a new global ref of " << className << std::endl;
exit(EXIT_FAILURE);
}
                /* Methods ID set to NULL */
voiddisplayjstringID=NULL; 
jstringreadLineID=NULL; 
voidclearID=NULL; 
voidclearjintID=NULL; 
jintgetCharWithoutOutputID=NULL; 
voidtoHomeID=NULL; 
voidscilabLinesUpdateID=NULL; 
voidsetPromptjstringID=NULL; 


}

SwingScilabConsole::SwingScilabConsole(JavaVM * jvm_, jobject JObj) {
        jvm=jvm_;

        JNIEnv * curEnv = getCurrentEnv();

        this->instanceClass = (jclass) curEnv->NewGlobalRef(curEnv->GetObjectClass(JObj));
        if (this->instanceClass == NULL) {
               std::cerr << "Could not create a Global Ref of " << this->instanceClass <<  std::endl;
               exit(EXIT_FAILURE);
        }

        this->instance = curEnv->NewGlobalRef(JObj) ;
        if(this->instance == NULL){
               std::cerr << "Could not create a new global ref of " << this->instanceClass << std::endl;
               exit(EXIT_FAILURE);
        }
        /* Methods ID set to NULL */
        voiddisplayjstringID=NULL; 
jstringreadLineID=NULL; 
voidclearID=NULL; 
voidclearjintID=NULL; 
jintgetCharWithoutOutputID=NULL; 
voidtoHomeID=NULL; 
voidscilabLinesUpdateID=NULL; 
voidsetPromptjstringID=NULL; 


}

// Generic methods


void SwingScilabConsole::synchronize() {
if (getCurrentEnv()->MonitorEnter(instance) != JNI_OK) {
std::cerr << "Fail to enter monitor." << std::endl;
exit(EXIT_FAILURE);
}
}


void SwingScilabConsole::endSynchronize() {
if ( getCurrentEnv()->MonitorExit(instance) != JNI_OK) {
std::cerr << "Fail to exit monitor." << std::endl;
exit(EXIT_FAILURE);
}
}

// Method(s)

void SwingScilabConsole::display (char * dataToDisplay){

JNIEnv * curEnv = getCurrentEnv();

if (this->voiddisplayjstringID == NULL)
{
this->voiddisplayjstringID = curEnv->GetMethodID(this->instanceClass, "display", "(Ljava/lang/String;)V" ) ;
if (this->voiddisplayjstringID == NULL) {
std::cerr << "Could not access to the method " << "display" << std::endl;
exit(EXIT_FAILURE);
}
}
jstring dataToDisplay_ = curEnv->NewStringUTF( dataToDisplay );

  curEnv->CallVoidMethod( this->instance, voiddisplayjstringID ,dataToDisplay_);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

char * SwingScilabConsole::readLine (){

JNIEnv * curEnv = getCurrentEnv();

if (this->jstringreadLineID == NULL)
{
this->jstringreadLineID = curEnv->GetMethodID(this->instanceClass, "readLine", "()Ljava/lang/String;" ) ;
if (this->jstringreadLineID == NULL) {
std::cerr << "Could not access to the method " << "readLine" << std::endl;
exit(EXIT_FAILURE);
}
}
 jstring res =  (jstring) curEnv->CallObjectMethod( this->instance, jstringreadLineID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


const char *tempString = curEnv->GetStringUTFChars(res, 0);
char * myStringBuffer= (char*)malloc (strlen(tempString)*sizeof(char)+1);
strcpy(myStringBuffer, tempString);
curEnv->ReleaseStringUTFChars(res, tempString);

return myStringBuffer;

}

void SwingScilabConsole::clear (){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidclearID == NULL)
{
this->voidclearID = curEnv->GetMethodID(this->instanceClass, "clear", "()V" ) ;
if (this->voidclearID == NULL) {
std::cerr << "Could not access to the method " << "clear" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidclearID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void SwingScilabConsole::clear (long nbLines){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidclearjintID == NULL)
{
this->voidclearjintID = curEnv->GetMethodID(this->instanceClass, "clear", "(I)V" ) ;
if (this->voidclearjintID == NULL) {
std::cerr << "Could not access to the method " << "clear" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidclearjintID ,nbLines);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

long SwingScilabConsole::getCharWithoutOutput (){

JNIEnv * curEnv = getCurrentEnv();

if (this->jintgetCharWithoutOutputID == NULL)
{
this->jintgetCharWithoutOutputID = curEnv->GetMethodID(this->instanceClass, "getCharWithoutOutput", "()I" ) ;
if (this->jintgetCharWithoutOutputID == NULL) {
std::cerr << "Could not access to the method " << "getCharWithoutOutput" <<  std::endl;
exit(EXIT_FAILURE);
}
}
 jint res =  (jint) curEnv->CallIntMethod( this->instance, jintgetCharWithoutOutputID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


return res;

}

void SwingScilabConsole::toHome (){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidtoHomeID == NULL)
{
this->voidtoHomeID = curEnv->GetMethodID(this->instanceClass, "toHome", "()V" ) ;
if (this->voidtoHomeID == NULL) {
std::cerr << "Could not access to the method " << "toHome" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidtoHomeID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void SwingScilabConsole::scilabLinesUpdate (){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidscilabLinesUpdateID == NULL)
{
this->voidscilabLinesUpdateID = curEnv->GetMethodID(this->instanceClass, "scilabLinesUpdate", "()V" ) ;
if (this->voidscilabLinesUpdateID == NULL) {
std::cerr << "Could not access to the method " << "scilabLinesUpdate" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidscilabLinesUpdateID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void SwingScilabConsole::setPrompt (char * promptToSet){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidsetPromptjstringID == NULL)
{
this->voidsetPromptjstringID = curEnv->GetMethodID(this->instanceClass, "setPrompt", "(Ljava/lang/String;)V" ) ;
if (this->voidsetPromptjstringID == NULL) {
std::cerr << "Could not access to the method " << "setPrompt" << std::endl;
exit(EXIT_FAILURE);
}
}
jstring promptToSet_ = curEnv->NewStringUTF( promptToSet );

  curEnv->CallVoidMethod( this->instance, voidsetPromptjstringID ,promptToSet_);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}


}

