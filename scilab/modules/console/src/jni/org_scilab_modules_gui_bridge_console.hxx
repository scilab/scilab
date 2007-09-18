
#include <string>
#include <iostream>
#include <stdlib.h>
#include <jni.h>

namespace org_scilab_modules_gui_bridge_console {


class SwingScilabConsole {
private:
JavaVM * jvm;
jobject instance;

jclass instanceClass; // cache class
jmethodID voiddisplayjstringID; // cache method id
jmethodID jstringreadLineID; // cache method id
jmethodID voidclearID; // cache method id
jmethodID voidclearjintID; // cache method id
jmethodID jintgetCharWithoutOutputID; // cache method id
jmethodID voidtoHomeID; // cache method id
jmethodID voidscilabLinesUpdateID; // cache method id
jmethodID voidsetPromptjstringID; // cache method id


/**
* Get the environment matching to the current thread.
*/
JNIEnv * getCurrentEnv();

public:
// Constructor
/**
* Create a wrapping of the object from a JNIEnv.
* It will call the default constructor
* @param JEnv_ the Java Env
*/
SwingScilabConsole(JavaVM * jvm_);
/**
* Create a wrapping of an already existing object from a JNIEnv.
* The object must have already been instantiated
* @param JEnv_ the Java Env
* @param JObj the object
*/
SwingScilabConsole(JavaVM * jvm_, jobject JObj);

// Destructor
~SwingScilabConsole();

// Generic method
// Synchronization methods
/**
* Enter monitor associated with the object.
* Equivalent of creating a "synchronized(obj)" scope in Java.
*/
void synchronize();

/**
* Exit monitor associated with the object.
* Equivalent of ending a "synchronized(obj)" scope.
*/
void endSynchronize();

// Methods
void display(char * dataToDisplay);

char * readLine();

void clear();

void clear(long nbLines);

long getCharWithoutOutput();

void toHome();

void scilabLinesUpdate();

void setPrompt(char * promptToSet);


};



}

