
/*

Copyright 2007 INRIA

Author : Sylvestre Ledru

This software is a computer program whose purpose is to hide the complexity
of accessing Java objects/methods from C++ code.

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
*/
#include "DrawableFigureGL.hxx"

#include <string>
#include <iostream>
#include <stdlib.h>
#include <jni.h>

namespace org_scilab_modules_renderer_figureDrawing {

// Returns the current env

JNIEnv * DrawableFigureGL::getCurrentEnv() {
JNIEnv * curEnv = NULL;
this->jvm->AttachCurrentThread((void **) &curEnv, NULL);
return curEnv;
}
// Destructor

DrawableFigureGL::~DrawableFigureGL() {
JNIEnv * curEnv = NULL;
this->jvm->AttachCurrentThread((void **) &curEnv, NULL);

curEnv->DeleteGlobalRef(this->instance);
curEnv->DeleteGlobalRef(this->instanceClass);
}

// Constructors

DrawableFigureGL::DrawableFigureGL(JavaVM * jvm_) {
jmethodID constructObject = NULL ;
jobject localInstance ;
jclass localClass ;
const std::string className="org/scilab/modules/renderer/figureDrawing/DrawableFigureGL";
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
voiddisplayID=NULL; 
voidinitializeDrawingjintID=NULL; 
voidendDrawingID=NULL; 
voidshowjintID=NULL; 
voiddestroyjintID=NULL; 
voidsetFigureIndexjintID=NULL; 
voiddrawCanvasID=NULL; 
voidcloseRenderingCanvasID=NULL; 
voidsetBackgroundColorjintID=NULL; 
voidsetColorMapDatajdoubleArrayID=NULL; 
jdoubleArraygetColorMapDataID=NULL; 
jintgetColorMapSizeID=NULL; 
jintgetCanvasWidthID=NULL; 
jintgetCanvasHeightID=NULL; 
voidsetCanvasSizejintjintID=NULL; 
jintgetWindowPosXID=NULL; 
jintgetWindowPosYID=NULL; 
voidsetWindowPositionjintjintID=NULL; 
jintgetWindowWidthID=NULL; 
jintgetWindowHeightID=NULL; 
voidsetWindowSizejintjintID=NULL; 
voidsetInfoMessagejstringID=NULL; 
voidsetPixmapModejintID=NULL; 
jintgetPixmapModeID=NULL; 


}

DrawableFigureGL::DrawableFigureGL(JavaVM * jvm_, jobject JObj) {
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
        voiddisplayID=NULL; 
voidinitializeDrawingjintID=NULL; 
voidendDrawingID=NULL; 
voidshowjintID=NULL; 
voiddestroyjintID=NULL; 
voidsetFigureIndexjintID=NULL; 
voiddrawCanvasID=NULL; 
voidcloseRenderingCanvasID=NULL; 
voidsetBackgroundColorjintID=NULL; 
voidsetColorMapDatajdoubleArrayID=NULL; 
jdoubleArraygetColorMapDataID=NULL; 
jintgetColorMapSizeID=NULL; 
jintgetCanvasWidthID=NULL; 
jintgetCanvasHeightID=NULL; 
voidsetCanvasSizejintjintID=NULL; 
jintgetWindowPosXID=NULL; 
jintgetWindowPosYID=NULL; 
voidsetWindowPositionjintjintID=NULL; 
jintgetWindowWidthID=NULL; 
jintgetWindowHeightID=NULL; 
voidsetWindowSizejintjintID=NULL; 
voidsetInfoMessagejstringID=NULL; 
voidsetPixmapModejintID=NULL; 
jintgetPixmapModeID=NULL; 


}

// Generic methods


void DrawableFigureGL::synchronize() {
if (getCurrentEnv()->MonitorEnter(instance) != JNI_OK) {
std::cerr << "Fail to enter monitor." << std::endl;
exit(EXIT_FAILURE);
}
}


void DrawableFigureGL::endSynchronize() {
if ( getCurrentEnv()->MonitorExit(instance) != JNI_OK) {
std::cerr << "Fail to exit monitor." << std::endl;
exit(EXIT_FAILURE);
}
}

// Method(s)

void DrawableFigureGL::display (){

JNIEnv * curEnv = getCurrentEnv();

if (this->voiddisplayID == NULL)
{
this->voiddisplayID = curEnv->GetMethodID(this->instanceClass, "display", "()V" ) ;
if (this->voiddisplayID == NULL) {
std::cerr << "Could not access to the method " << "display" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voiddisplayID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void DrawableFigureGL::initializeDrawing (long figureIndex){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidinitializeDrawingjintID == NULL)
{
this->voidinitializeDrawingjintID = curEnv->GetMethodID(this->instanceClass, "initializeDrawing", "(I)V" ) ;
if (this->voidinitializeDrawingjintID == NULL) {
std::cerr << "Could not access to the method " << "initializeDrawing" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidinitializeDrawingjintID ,figureIndex);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void DrawableFigureGL::endDrawing (){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidendDrawingID == NULL)
{
this->voidendDrawingID = curEnv->GetMethodID(this->instanceClass, "endDrawing", "()V" ) ;
if (this->voidendDrawingID == NULL) {
std::cerr << "Could not access to the method " << "endDrawing" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidendDrawingID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void DrawableFigureGL::show (long figureIndex){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidshowjintID == NULL)
{
this->voidshowjintID = curEnv->GetMethodID(this->instanceClass, "show", "(I)V" ) ;
if (this->voidshowjintID == NULL) {
std::cerr << "Could not access to the method " << "show" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidshowjintID ,figureIndex);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void DrawableFigureGL::destroy (long parentFigureIndex){

JNIEnv * curEnv = getCurrentEnv();

if (this->voiddestroyjintID == NULL)
{
this->voiddestroyjintID = curEnv->GetMethodID(this->instanceClass, "destroy", "(I)V" ) ;
if (this->voiddestroyjintID == NULL) {
std::cerr << "Could not access to the method " << "destroy" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voiddestroyjintID ,parentFigureIndex);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void DrawableFigureGL::setFigureIndex (long figureIndex){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidsetFigureIndexjintID == NULL)
{
this->voidsetFigureIndexjintID = curEnv->GetMethodID(this->instanceClass, "setFigureIndex", "(I)V" ) ;
if (this->voidsetFigureIndexjintID == NULL) {
std::cerr << "Could not access to the method " << "setFigureIndex" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidsetFigureIndexjintID ,figureIndex);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void DrawableFigureGL::drawCanvas (){

JNIEnv * curEnv = getCurrentEnv();

if (this->voiddrawCanvasID == NULL)
{
this->voiddrawCanvasID = curEnv->GetMethodID(this->instanceClass, "drawCanvas", "()V" ) ;
if (this->voiddrawCanvasID == NULL) {
std::cerr << "Could not access to the method " << "drawCanvas" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voiddrawCanvasID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void DrawableFigureGL::closeRenderingCanvas (){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidcloseRenderingCanvasID == NULL)
{
this->voidcloseRenderingCanvasID = curEnv->GetMethodID(this->instanceClass, "closeRenderingCanvas", "()V" ) ;
if (this->voidcloseRenderingCanvasID == NULL) {
std::cerr << "Could not access to the method " << "closeRenderingCanvas" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidcloseRenderingCanvasID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void DrawableFigureGL::setBackgroundColor (long backgroundColor){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidsetBackgroundColorjintID == NULL)
{
this->voidsetBackgroundColorjintID = curEnv->GetMethodID(this->instanceClass, "setBackgroundColor", "(I)V" ) ;
if (this->voidsetBackgroundColorjintID == NULL) {
std::cerr << "Could not access to the method " << "setBackgroundColor" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidsetBackgroundColorjintID ,backgroundColor);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void DrawableFigureGL::setColorMapData (double * rgbmat, int rgbmatSize){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidsetColorMapDatajdoubleArrayID == NULL)
{
this->voidsetColorMapDatajdoubleArrayID = curEnv->GetMethodID(this->instanceClass, "setColorMapData", "([D)V" ) ;
if (this->voidsetColorMapDatajdoubleArrayID == NULL) {
std::cerr << "Could not access to the method " << "setColorMapData" << std::endl;
exit(EXIT_FAILURE);
}
}
jdoubleArray rgbmat_ = curEnv->NewDoubleArray( rgbmatSize ) ;
curEnv->SetDoubleArrayRegion( rgbmat_, 0, rgbmatSize, (jdouble*) rgbmat ) ;

  curEnv->CallVoidMethod( this->instance, voidsetColorMapDatajdoubleArrayID ,rgbmat_);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

double * DrawableFigureGL::getColorMapData (){

JNIEnv * curEnv = getCurrentEnv();

if (this->jdoubleArraygetColorMapDataID == NULL)
{
this->jdoubleArraygetColorMapDataID = curEnv->GetMethodID(this->instanceClass, "getColorMapData", "()[D" ) ;
if (this->jdoubleArraygetColorMapDataID == NULL) {
std::cerr << "Could not access to the method " << "getColorMapData" << std::endl;
exit(EXIT_FAILURE);
}
}
 jdoubleArray res =  (jdoubleArray) curEnv->CallObjectMethod( this->instance, jdoubleArraygetColorMapDataID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


jsize len = curEnv->GetArrayLength(res);
jboolean isCopy = JNI_FALSE;

/* faster than getXXXArrayElements */
jdouble *resultsArray = (jdouble *) curEnv->GetPrimitiveArrayCritical(res, &isCopy);
double * myArray= new double[len];

for (jsize i = 0; i < len; i++){
myArray[i]=resultsArray[i];
}
curEnv->ReleasePrimitiveArrayCritical(res, resultsArray, JNI_ABORT);

return myArray;

}

long DrawableFigureGL::getColorMapSize (){

JNIEnv * curEnv = getCurrentEnv();

if (this->jintgetColorMapSizeID == NULL)
{
this->jintgetColorMapSizeID = curEnv->GetMethodID(this->instanceClass, "getColorMapSize", "()I" ) ;
if (this->jintgetColorMapSizeID == NULL) {
std::cerr << "Could not access to the method " << "getColorMapSize" << std::endl;
exit(EXIT_FAILURE);
}
}
 jint res =  (jint) curEnv->CallIntMethod( this->instance, jintgetColorMapSizeID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


return res;

}

long DrawableFigureGL::getCanvasWidth (){

JNIEnv * curEnv = getCurrentEnv();

if (this->jintgetCanvasWidthID == NULL)
{
this->jintgetCanvasWidthID = curEnv->GetMethodID(this->instanceClass, "getCanvasWidth", "()I" ) ;
if (this->jintgetCanvasWidthID == NULL) {
std::cerr << "Could not access to the method " << "getCanvasWidth" << std::endl;
exit(EXIT_FAILURE);
}
}
 jint res =  (jint) curEnv->CallIntMethod( this->instance, jintgetCanvasWidthID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


return res;

}

long DrawableFigureGL::getCanvasHeight (){

JNIEnv * curEnv = getCurrentEnv();

if (this->jintgetCanvasHeightID == NULL)
{
this->jintgetCanvasHeightID = curEnv->GetMethodID(this->instanceClass, "getCanvasHeight", "()I" ) ;
if (this->jintgetCanvasHeightID == NULL) {
std::cerr << "Could not access to the method " << "getCanvasHeight" << std::endl;
exit(EXIT_FAILURE);
}
}
 jint res =  (jint) curEnv->CallIntMethod( this->instance, jintgetCanvasHeightID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


return res;

}

void DrawableFigureGL::setCanvasSize (long width, long height){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidsetCanvasSizejintjintID == NULL)
{
this->voidsetCanvasSizejintjintID = curEnv->GetMethodID(this->instanceClass, "setCanvasSize", "(II)V" ) ;
if (this->voidsetCanvasSizejintjintID == NULL) {
std::cerr << "Could not access to the method " << "setCanvasSize" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidsetCanvasSizejintjintID ,width, height);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

long DrawableFigureGL::getWindowPosX (){

JNIEnv * curEnv = getCurrentEnv();

if (this->jintgetWindowPosXID == NULL)
{
this->jintgetWindowPosXID = curEnv->GetMethodID(this->instanceClass, "getWindowPosX", "()I" ) ;
if (this->jintgetWindowPosXID == NULL) {
std::cerr << "Could not access to the method " << "getWindowPosX" << std::endl;
exit(EXIT_FAILURE);
}
}
 jint res =  (jint) curEnv->CallIntMethod( this->instance, jintgetWindowPosXID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


return res;

}

long DrawableFigureGL::getWindowPosY (){

JNIEnv * curEnv = getCurrentEnv();

if (this->jintgetWindowPosYID == NULL)
{
this->jintgetWindowPosYID = curEnv->GetMethodID(this->instanceClass, "getWindowPosY", "()I" ) ;
if (this->jintgetWindowPosYID == NULL) {
std::cerr << "Could not access to the method " << "getWindowPosY" << std::endl;
exit(EXIT_FAILURE);
}
}
 jint res =  (jint) curEnv->CallIntMethod( this->instance, jintgetWindowPosYID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


return res;

}

void DrawableFigureGL::setWindowPosition (long posX, long posY){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidsetWindowPositionjintjintID == NULL)
{
this->voidsetWindowPositionjintjintID = curEnv->GetMethodID(this->instanceClass, "setWindowPosition", "(II)V" ) ;
if (this->voidsetWindowPositionjintjintID == NULL) {
std::cerr << "Could not access to the method " << "setWindowPosition" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidsetWindowPositionjintjintID ,posX, posY);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

long DrawableFigureGL::getWindowWidth (){

JNIEnv * curEnv = getCurrentEnv();

if (this->jintgetWindowWidthID == NULL)
{
this->jintgetWindowWidthID = curEnv->GetMethodID(this->instanceClass, "getWindowWidth", "()I" ) ;
if (this->jintgetWindowWidthID == NULL) {
std::cerr << "Could not access to the method " << "getWindowWidth" << std::endl;
exit(EXIT_FAILURE);
}
}
 jint res =  (jint) curEnv->CallIntMethod( this->instance, jintgetWindowWidthID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


return res;

}

long DrawableFigureGL::getWindowHeight (){

JNIEnv * curEnv = getCurrentEnv();

if (this->jintgetWindowHeightID == NULL)
{
this->jintgetWindowHeightID = curEnv->GetMethodID(this->instanceClass, "getWindowHeight", "()I" ) ;
if (this->jintgetWindowHeightID == NULL) {
std::cerr << "Could not access to the method " << "getWindowHeight" << std::endl;
exit(EXIT_FAILURE);
}
}
 jint res =  (jint) curEnv->CallIntMethod( this->instance, jintgetWindowHeightID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


return res;

}

void DrawableFigureGL::setWindowSize (long width, long height){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidsetWindowSizejintjintID == NULL)
{
this->voidsetWindowSizejintjintID = curEnv->GetMethodID(this->instanceClass, "setWindowSize", "(II)V" ) ;
if (this->voidsetWindowSizejintjintID == NULL) {
std::cerr << "Could not access to the method " << "setWindowSize" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidsetWindowSizejintjintID ,width, height);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void DrawableFigureGL::setInfoMessage (char * infoMessage){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidsetInfoMessagejstringID == NULL)
{
this->voidsetInfoMessagejstringID = curEnv->GetMethodID(this->instanceClass, "setInfoMessage", "(Ljava/lang/String;)V" ) ;
if (this->voidsetInfoMessagejstringID == NULL) {
std::cerr << "Could not access to the method " << "setInfoMessage" << std::endl;
exit(EXIT_FAILURE);
}
}
jstring infoMessage_ = curEnv->NewStringUTF( infoMessage );

  curEnv->CallVoidMethod( this->instance, voidsetInfoMessagejstringID ,infoMessage_);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

void DrawableFigureGL::setPixmapMode (long onOrOff){

JNIEnv * curEnv = getCurrentEnv();

if (this->voidsetPixmapModejintID == NULL)
{
this->voidsetPixmapModejintID = curEnv->GetMethodID(this->instanceClass, "setPixmapMode", "(I)V" ) ;
if (this->voidsetPixmapModejintID == NULL) {
std::cerr << "Could not access to the method " << "setPixmapMode" << std::endl;
exit(EXIT_FAILURE);
}
}
  curEnv->CallVoidMethod( this->instance, voidsetPixmapModejintID ,onOrOff);

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


}

long DrawableFigureGL::getPixmapMode (){

JNIEnv * curEnv = getCurrentEnv();

if (this->jintgetPixmapModeID == NULL)
{
this->jintgetPixmapModeID = curEnv->GetMethodID(this->instanceClass, "getPixmapMode", "()I" ) ;
if (this->jintgetPixmapModeID == NULL) {
std::cerr << "Could not access to the method " << "getPixmapMode" << std::endl;
exit(EXIT_FAILURE);
}
}
 jint res =  (jint) curEnv->CallIntMethod( this->instance, jintgetPixmapModeID );

if (curEnv->ExceptionOccurred()) {
curEnv->ExceptionDescribe() ;
}


return res;

}

}
