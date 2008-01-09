/*--------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <jni.h>
#include "machine.h"
extern "C" {
#include "./../../jvm/includes/getScilabObject.h"
#include "../../../jvm/includes/getScilabJNIEnv.h"
}
#include "MALLOC.h"
#include "GetCharWithoutOutput.hxx"
/*--------------------------------------------------------------------------*/
#include "org_scilab_modules_gui_bridge.hxx"
using namespace  org_scilab_modules_gui_bridge;
int GetCharWithoutOutput(void)
{
  return CallScilabBridge::getCharWithoutOutput(getScilabJavaVM());
}
