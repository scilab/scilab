/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
extern "C"
{
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
}
#include "HistorySearch.hxx"
#include "MALLOC.h"
/*------------------------------------------------------------------------*/
HistorySearch::HistorySearch()
{
    this->my_token.erase();
    this->my_lines = NULL;
    this->my_linenumbers = NULL;
    this->my_sizearray = 0;
    this->current_position = 0;
    moveOnNext = FALSE;
}
/*------------------------------------------------------------------------*/
HistorySearch::~HistorySearch()
{
    this->reset();
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::setHistory(list<CommandLine> commands)
{
    BOOL bOK = FALSE;
    list<CommandLine>::iterator it_commands;

    if (!this->Commands.empty())
    {
        this->Commands.clear();
    }

    for (it_commands = commands.begin(); it_commands != commands.end(); ++it_commands)
    {
        std::string line = (*it_commands).get();
        if (!line.empty())
        {
            CommandLine Line(line);
            this->Commands.push_back(Line);
        }
    }
    return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::setToken(std::string token)
{
    BOOL bOK = FALSE;
    if (!token.empty())
    {
        if (!this->my_token.empty())
        {
            if (this->my_token.compare(token))
            {
                this->my_token.erase();
                this->my_token = token;
                bOK = this->search();
            }
        }
        else
        {
            this->my_token = token;
            bOK = this->search();
        }
    }
    else
    {
        freeMyToken();
        bOK = this->search();
    }
    return bOK;
}
/*------------------------------------------------------------------------*/
std::string HistorySearch::getToken(void)
{
    return my_token;
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::search(void)
{
    BOOL bOK = FALSE;

    if (!this->my_token.empty())
    {
        int line_indice = 0;
        int i = 0;
        list<CommandLine>::iterator it_commands;

        freeMylines();
        freeMylinenumbers();

        this->my_sizearray = 0;

        for (it_commands = this->Commands.begin(); it_commands != this->Commands.end(); ++it_commands)
        {
            std::string line = (*it_commands).get();

            if ( strncmp(line.c_str(), this->my_token.c_str(), strlen(this->my_token.c_str())) == 0 )
            {
                i++;
                if (this->my_lines)
                {
                    this->my_lines = (char**)REALLOC(this->my_lines, i * (sizeof(char*)));
                }
                else
                {
                    this->my_lines = (char**)MALLOC(i * (sizeof(char*)));
                }

                if (this->my_lines)
                {
                    this->my_lines[i - 1] = strdup(line.c_str());
                }

                if (this->my_linenumbers)
                {
                    this->my_linenumbers = (int*)REALLOC(this->my_linenumbers, i * (sizeof(int)));
                }
                else
                {
                    this->my_linenumbers = (int*)MALLOC(i * (sizeof(int)));
                }
                if (this->my_linenumbers)
                {
                    this->my_linenumbers[i - 1] = line_indice;
                }
            }
            line_indice++;
        }
        this->my_sizearray = i;

        //this->current_position = i - 1;
        this->current_position = i ;
    }
    else
    {
        int line_indice = 0;
        int i = 0;
        list<CommandLine>::iterator it_commands;

        freeMylines();
        freeMylinenumbers();

        this->my_sizearray = 0;

        for (it_commands = this->Commands.begin(); it_commands != this->Commands.end(); ++it_commands)
        {
            std::string line = (*it_commands).get();
            {
                i++;
                if (this->my_lines)
                {
                    this->my_lines = (char**)REALLOC(this->my_lines, i * (sizeof(char*)));
                }
                else
                {
                    this->my_lines = (char**)MALLOC(i * (sizeof(char*)));
                }

                if (this->my_lines)
                {
                    this->my_lines[i - 1] = strdup(line.c_str());
                }

                if (this->my_linenumbers)
                {
                    this->my_linenumbers = (int*)REALLOC(this->my_linenumbers, i * (sizeof(int)));
                }
                else
                {
                    this->my_linenumbers = (int*)MALLOC(i * (sizeof(int)));
                }

                if (this->my_linenumbers)
                {
                    this->my_linenumbers[i - 1] = line_indice;
                }
            }

            line_indice++;
        }
        this->my_sizearray = i;
        this->current_position = i ;
    }

    moveOnNext = FALSE;
    return bOK;
}
/*------------------------------------------------------------------------*/
int HistorySearch::getSize(void)
{
    return this->my_sizearray;
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::reset(void)
{
    BOOL bOK = FALSE;
    BOOL check1 = FALSE, check2 = FALSE, check3 = FALSE, check4 = FALSE;

    if (!this->Commands.empty())
    {
        this->Commands.clear();
        check1 = TRUE;
    }

    check2 = freeMyToken();
    check3 = freeMylines();
    check4 = freeMylinenumbers();

    this->my_sizearray = 0;
    this->current_position = 0;

    moveOnNext = FALSE;

    if (check1 && check2 && check3 && check4)
    {
        bOK = TRUE;
    }

    return bOK;
}
/*------------------------------------------------------------------------*/
std::string HistorySearch::getPreviousLine(void)
{
    std::string line;

    if (my_lines)
    {
        if  (moveOnNext)
        {
            this->current_position++;
        }

        if (this->current_position <= 0)
        {
            this->current_position = 0;
        }
        else
        {
            this->current_position--;
        }

        if (my_lines[this->current_position])
        {
            line.assign(my_lines[this->current_position]);
        }
    }

    moveOnNext = FALSE;
    return line;
}
/*------------------------------------------------------------------------*/
std::string HistorySearch::getNextLine(void)
{
    std::string line;

    if (my_lines)
    {
        if (this->current_position < this->my_sizearray)
        {
            this->current_position++;
        }

        if ( (this->current_position < this->my_sizearray) && my_lines[this->current_position] && (strlen(my_lines[this->current_position]) > 0))
        {
            line.assign(my_lines[this->current_position]);
        }

        if (this->current_position == this->my_sizearray)
        {
            line = this->getToken();
            this->current_position--;
        }
    }

    moveOnNext = TRUE;
    return line;
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::freeMyToken(void)
{
    BOOL bOK = FALSE;
    if (!this->my_token.empty())
    {
        this->my_token.erase();
        bOK = TRUE;
    }
    return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::freeMylines(void)
{
    BOOL bOK = FALSE;
    if (this->my_lines)
    {
        int i = 0;
        for (i = 0; i < this->my_sizearray; i++)
        {
            if (this->my_lines[i])
            {
                FREE(this->my_lines[i]);
                this->my_lines[i] = NULL;
            }
        }
        FREE(this->my_lines);
        this->my_lines = NULL;
        bOK = TRUE;
    }
    return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::freeMylinenumbers(void)
{
    BOOL bOK = FALSE;
    if (this->my_linenumbers)
    {
        FREE(this->my_linenumbers);
        this->my_linenumbers = NULL;
        bOK = TRUE;
    }
    return bOK;
}
/*------------------------------------------------------------------------*/

