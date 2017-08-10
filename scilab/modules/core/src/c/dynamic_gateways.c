/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
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
#include <stdlib.h>
#include "dynamic_gateways.h"
#include "gw_dynamic_generic.h"
#include "configvariable_interface.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
/* special_functions module */
#define SPECIAL_FUNCTIONS_MODULE_NAME "special_functions"
static DynLibHandle hSpecial_functionsLib = NULL;
static void* ptr_gw_special_functions = NULL;
static char* dynlibname_special_functions = NULL;
static char* gatewayname_special_functions = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_special_functions(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* optimization module */
#define OPTIMIZATION_MODULE_NAME "optimization"
static DynLibHandle hOptimizationLib = NULL;
static void* ptr_gw_optimization = NULL;
static char* dynlibname_optimization = NULL;
static char* gatewayname_optimization = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_optimization(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* UMFPACK module */
#define UMFPACK_MODULE_NAME "umfpack"
static DynLibHandle hUmfpackLib = NULL;
static void* ptr_gw_umfpack = NULL;
static char* dynlibname_umfpack = NULL;
static char* gatewayname_umfpack = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_umfpack(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* spreadsheet module */
#define SPREADSHEET_MODULE_NAME "spreadsheet"
static DynLibHandle hSpreadsheetLib = NULL;
static void* ptr_gw_spreadsheet = NULL;
static char* dynlibname_spreadsheet = NULL;
static char* gatewayname_spreadsheet = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_spreadsheet(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* sound module */
#define SOUND_MODULE_NAME "sound"
static DynLibHandle hSoundLib = NULL;
static void* ptr_gw_sound = NULL;
static char* dynlibname_sound = NULL;
static char* gatewayname_sound = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_sound(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* scicos module */
#define SCICOS_MODULE_NAME "scicos"
#define SCICOS_CLI_MODULE_NAME "scicos-cli"
static DynLibHandle hScicosLib = NULL;
static void* ptr_gw_scicos = NULL;
static char* dynlibname_scicos = NULL;
static char* gatewayname_scicos = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_scicos(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* randlib module */
#define RANDLIB_MODULE_NAME "randlib"
static DynLibHandle hRandlibLib = NULL;
static void* ptr_gw_randlib = NULL;
static char* dynlibname_randlib = NULL;
static char* gatewayname_randlib = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_randlib(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* arnoldi module */
#define ARNOLDI_MODULE_NAME "arnoldi"
static DynLibHandle hArnoldiLib = NULL;
static void* ptr_gw_arnoldi = NULL;
static char* dynlibname_arnoldi = NULL;
static char* gatewayname_arnoldi = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_arnoldi(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* symbolic module */
#define SYMBOLIC_MODULE_NAME "symbolic"
static DynLibHandle hSymbolicLib = NULL;
static void* ptr_gw_symbolic = NULL;
static char* dynlibname_symbolic = NULL;
static char* gatewayname_symbolic = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_symbolic(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* interpolation module */
#define INTERPOLATION_MODULE_NAME "interpolation"
static DynLibHandle hInterpolationLib = NULL;
static void* ptr_gw_interpolation = NULL;
static char* dynlibname_interpolation = NULL;
static char* gatewayname_interpolation = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_interpolation(void)
{
    return 0;
}

/*--------------------------------------------------------------------------*/
/* xcos module */
#define XCOS_MODULE_NAME "xcos"
#define XCOS_DISABLE_MODULE_NAME "xcos-disable"
static DynLibHandle hXcosLib = NULL;
static void* ptr_gw_xcos = NULL;
static char* dynlibname_xcos = NULL;
static char* gatewayname_xcos = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_xcos(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* scinotes module */
#define SCINOTES_MODULE_NAME "scinotes"
#define SCINOTES_DISABLE_MODULE_NAME "scinotes-disable"
static DynLibHandle hSciNotesLib = NULL;
static void* ptr_gw_scinotes = NULL;
static char* dynlibname_scinotes = NULL;
static char* gatewayname_scinotes = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_scinotes(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* graphic_exports module */
#define GRAPHIC_EXPORT_MODULE_NAME "graphic_export"
#define GRAPHIC_EXPORT_DISABLE_MODULE_NAME "graphic_export-disable"
static DynLibHandle hGraphic_exportLib = NULL;
static void* ptr_gw_graphic_export = NULL;
static char* dynlibname_graphic_export = NULL;
static char* gatewayname_graphic_export = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_graphic_export(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* graphic_exports module */
#define ACTION_BINDING_MODULE_NAME "action_binding"
#define ACTION_BINDING_DISABLE_MODULE_NAME "action_binding-disable"
static DynLibHandle hAction_bindingLib = NULL;
static PROC_GATEWAY ptr_gw_action_binding = NULL;
static char* dynlibname_action_binding = NULL;
static char* gatewayname_action_binding = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_action_binding(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* ui_data module */
#define UI_DATA_MODULE_NAME "ui_data"
#define UI_DATA_DISABLE_MODULE_NAME "ui_data-disable"
static DynLibHandle hUi_dataLib = NULL;
static void* ptr_gw_ui_data = NULL;
static char* dynlibname_ui_data = NULL;
static char* gatewayname_ui_data = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_ui_data(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* preferences module */
#define PREFERENCES_MODULE_NAME "preferences"
static DynLibHandle hPreferencesLib = NULL;
static void* ptr_gw_preferences = NULL;
static char* dynlibname_preferences = NULL;
static char* gatewayname_preferences = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_preferences(void)
{
    return 0;
}

/*--------------------------------------------------------------------------*/
/* MPI module */
#define MPI_MODULE_NAME "mpi"
static DynLibHandle hMpiLib = NULL;
static PROC_GATEWAY ptr_gw_mpi = NULL;
static char* dynlibname_mpi = NULL;
static char* gatewayname_mpi = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_mpi(void)
{
    return 0;
}

/*--------------------------------------------------------------------------*/
/* xml module */
#define XML_MODULE_NAME "xml"
static DynLibHandle hXmlLib = NULL;
static void* ptr_gw_xml = NULL;
static char* dynlibname_xml = NULL;
static char* gatewayname_xml = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_xml(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* graphic_objects module */
#define GRAPHIC_OBJECTS_MODULE_NAME "graphic_objects"
#define GRAPHIC_OBJECTS_DISABLE_MODULE_NAME "graphic_objects-disable"
static DynLibHandle hGraphic_objectsLib = NULL;
static void* ptr_gw_graphic_objects = NULL;
static char* dynlibname_graphic_objects = NULL;
static char* gatewayname_graphic_objects = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_graphic_objects(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* history_browser module */
#define HISTORY_BROWSER_MODULE_NAME "history_browser"
#define HISTORY_BROWSER_DISABLE_MODULE_NAME "history_browser-disable"
static DynLibHandle hHistory_browserLib = NULL;
static void* ptr_gw_history_browser = NULL;
static char* dynlibname_history_browser = NULL;
static char* gatewayname_history_browser = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_history_browser(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* external_objects module */
#define EXTERNAL_OBJECTS_MODULE_NAME "external_objects"
static DynLibHandle hExternal_ObjectsLib = NULL;
static void* ptr_gw_external_objects = NULL;
static char* dynlibname_external_objects = NULL;
static char* gatewayname_external_objects = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_external_objects(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
/* external_objects_java module */
#define EXTERNAL_OBJECTS_JAVA_MODULE_NAME "external_objects_java"
static DynLibHandle hExternal_Objects_JavaLib = NULL;
static PROC_GATEWAY ptr_gw_external_objects_java = NULL;
static char* dynlibname_external_objects_java = NULL;
static char* gatewayname_external_objects_java = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_external_objects_java(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
void freeAllDynamicGateways(void)
{
}
/*--------------------------------------------------------------------------*/

