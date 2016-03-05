/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
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

#include "configvariable.hxx"
#include "debugmanager.hxx"

extern "C"
{
#include "configvariable_interface.h"
#include "setenvvar.h"
}

int getSimpMode()
{
    return ConfigVariable::getSimpMode();
}

void setSimpMode(int _isimpmd)
{
    ConfigVariable::setSimpMode(_isimpmd);
}

int getStartProcessing()
{
    return ConfigVariable::getStartProcessing() ? 1 : 0;
}

int getEndProcessing()
{
    return ConfigVariable::getEndProcessing() ? 1 : 0;
}

void setFormatSize(int _iFormatSize)
{
    ConfigVariable::setFormatSize(_iFormatSize);
}

int getFormatSize(void)
{
    return ConfigVariable::getFormatSize();
}

void setFormatMode(int _iFormatMode)
{
    ConfigVariable::setFormatSize(_iFormatMode);
}

int getFormatMode(void)
{
    return ConfigVariable::getFormatMode();
}

void setConsoleWidth(int _iConsoleWidth)
{
    ConfigVariable::setConsoleWidth(_iConsoleWidth);
}

int getConsoleWidth(void)
{
    return ConfigVariable::getConsoleWidth();
}

void setConsoleLines(int _iConsoleLines)
{
    ConfigVariable::setConsoleLines(_iConsoleLines);
}

int getConsoleLines(void)
{
    return ConfigVariable::getConsoleLines();
}

int getPromptMode(void)
{
    return static_cast<int>(ConfigVariable::getPromptMode());
}

void setPromptMode(int _iMode)
{
    ConfigVariable::setPromptMode(_iMode);
}

int isPrintInput()
{
    return ConfigVariable::isPrintInput() ? 1 : 0;
}

void setPrintInput(int val)
{
    ConfigVariable::setPrintInput(val ? true : false);
}

int isPrintOutput()
{
    return ConfigVariable::isPrintOutput() ? 1 : 0;
}

void setPrintOutput(int val)
{
    ConfigVariable::setPrintOutput(val ? true : false);
}

int isPrintCompact()
{
    return ConfigVariable::isPrintCompact() ? 1 : 0;
}

void setPrintCompact(int val)
{
    ConfigVariable::setPrintCompact(val ? true : false);
}

int isPrintInteractive()
{
    return ConfigVariable::isPrintInteractive() ? 1 : 0;
}

void setPrintInteractive(int val)
{
    ConfigVariable::setPrintInteractive(val ? true : false);
}

int isSilentError(void)
{
    return ConfigVariable::isSilentError() ? 1 : 0;
}

void setSilentError(int _iSilent)
{
    ConfigVariable::setSilentError(_iSilent ? true : false);
}

int getieee()
{
    return ConfigVariable::getIeee();
}

void setieee(int _iIeee)
{
    ConfigVariable::setIeee(_iIeee);
}

scilabMode getScilabMode(void)
{
    return static_cast<scilabMode>(ConfigVariable::getScilabMode());
}

void setScilabMode(scilabMode newmode)
{
    ConfigVariable::setScilabMode(static_cast<int>(newmode));
}

const char * getScilabModeString(void)
{
    scilabMode smMode = getScilabMode();
    switch (smMode)
    {
        case SCILAB_API:
            return "API";
            break;
        case SCILAB_STD:
            return "STD";
            break;
        case SCILAB_NW:
            return "NW";
            break;
        case SCILAB_NWNI:
            return "NWNI";
            break;
        default:
            return "STD";
            break;
    }
}

int getWarningMode(void)
{
    return ConfigVariable::getWarningMode() ? 1 : 0;
}

void setWarningMode(int _iMode)
{
    ConfigVariable::setWarningMode(_iMode != 0);
}

int getWarningStop(void)
{
    return ConfigVariable::getWarningStop() ? 1 : 0;
}

void setWarningStop(int _iStop)
{
    ConfigVariable::setWarningStop(_iStop != 0);
}


int checkReferenceModule(const wchar_t* _module)
{
    return ConfigVariable::checkReferenceModule(_module) ? 1 : 0;
}

void addReferenceModule(const wchar_t* _module)
{
    ConfigVariable::addReferenceModule(_module);
}

void removeReferenceModule(const wchar_t* _module)
{
    ConfigVariable::removeReferenceModule(_module);
}

int getEntryPointPosition(wchar_t* _pwstEntryPointName)
{
    return ConfigVariable::getEntryPointPosition(_pwstEntryPointName);
}

dynlib_ptr getEntryPointFromPosition(int position)
{
    return ConfigVariable::getEntryPointFromPosition(position);
}

int getForceQuit()
{
    return ConfigVariable::getForceQuit();
}

int isEnableDebug()
{
    return ConfigVariable::getEnableDebug() ? 1 : 0;
}

int isDebugInterrupted()
{
    return debugger::DebuggerMagager::getInstance()->isInterrupted() ? 1 : 0;
}

int isExecutionBreak()
{
    return ConfigVariable::isExecutionBreak() ? 1 : 0;
}

void setExecutionBreak()
{
    ConfigVariable::setExecutionBreak();
}

void resetExecutionBreak()
{
    ConfigVariable::resetExecutionBreak();
}

int setRecursionLimit(int val)
{
    return ConfigVariable::setRecursionLimit(val);
}
