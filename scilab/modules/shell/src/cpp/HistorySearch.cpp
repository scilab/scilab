/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
extern "C"
{
	#include <string.h>
}
#include "HistorySearch.hxx"
#include "MALLOC.h"
/*------------------------------------------------------------------------*/
HistorySearch::HistorySearch()
{
	this->my_token = NULL;
	this->my_lines = NULL;
	this->my_linenumbers = NULL;
	this->my_sizearray = 0;
	this->current_position = 0;
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

	if (!this->Commands.empty()) this->Commands.clear();

	for(it_commands=commands.begin(); it_commands != commands.end(); ++it_commands) 
	{
		char *line = (*it_commands).get();
		if (line)
		{
			CommandLine Line(line);
			this->Commands.push_back(Line);
			FREE(line);
			line = NULL;
		}
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::setToken(char *token)
{
	BOOL bOK = FALSE;
	if (token)
	{
		if (this->my_token)
		{
			if (strcmp(this->my_token,token))
			{
				FREE(this->my_token);
				this->my_token = (char*)MALLOC(sizeof(char)*(strlen(token)+1));
				if (this->my_token) 
				{
					strcpy(this->my_token,token);
					bOK = this->search();
				}
			}
		}
		else
		{
			this->my_token = (char*)MALLOC(sizeof(char)*(strlen(token)+1));
			if (this->my_token) 
			{
				strcpy(this->my_token,token);
				bOK = this->search();
			}
		}
	}
	else
	{
		FREE(this->my_token);
		this->my_token = NULL;
		bOK = this->search();
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
char *HistorySearch::getToken(void)
{
	char *line = NULL;
	if (this->my_token)
	{
		line = (char*)MALLOC(sizeof(char)*(strlen(this->my_token)+1));
		strcpy(line,this->my_token);
	}
	return line;
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::search(void)
{
	BOOL bOK = FALSE;

	if (this->my_token)
	{
		int line_indice = 0;
		int i = 0;
		list<CommandLine>::iterator it_commands;

		if (this->my_lines)
		{
			int i = 0;
			for (i = 0;i < this->my_sizearray; i++)
			{
				if (this->my_lines[i]) 
				{
					FREE(this->my_lines[i]);
					this->my_lines[i] = NULL;
				}
			}
			FREE(this->my_lines);
			this->my_lines = NULL;
		}

		if (this->my_linenumbers)
		{
			FREE(this->my_linenumbers);
			this->my_linenumbers = NULL;
		}

		this->my_sizearray = 0;

		for(it_commands=this->Commands.begin(); it_commands != this->Commands.end(); ++it_commands) 
		{
			char *line = (*it_commands).get();

			if ( strncmp(line,this->my_token,strlen(this->my_token)) == 0 )
			{
				i++;
				if (this->my_lines) this->my_lines = (char**)REALLOC(this->my_lines,i*(sizeof(char*)));
				else this->my_lines = (char**)MALLOC(i*(sizeof(char*)));
				this->my_lines[i-1] = line;

				if (this->my_linenumbers) this->my_linenumbers = (int*)REALLOC(this->my_linenumbers,i*(sizeof(int)));
				else this->my_linenumbers = (int*)MALLOC(i*(sizeof(int)));
				this->my_linenumbers[i-1] = line_indice;
			}
			else 
			{
				FREE(line);
				line = NULL;
			}

			line_indice++;
		}
		this->my_sizearray = i;

		this->current_position = i - 1;
	}
	else
	{
		int line_indice = 0;
		int i = 0;
		list<CommandLine>::iterator it_commands;

		if (this->my_lines)
		{
			int i = 0;
			for (i = 0;i < this->my_sizearray; i++)
			{
				if (this->my_lines[i]) 
				{
					FREE(this->my_lines[i]);
					this->my_lines[i] = NULL;
				}
			}
			FREE(this->my_lines);
			this->my_lines = NULL;
		}

		if (this->my_linenumbers)
		{
			FREE(this->my_linenumbers);
			this->my_linenumbers = NULL;
		}

		this->my_sizearray = 0;

		for(it_commands=this->Commands.begin(); it_commands != this->Commands.end(); ++it_commands) 
		{
			char *line = (*it_commands).get();
			{
				i++;
				if (this->my_lines) this->my_lines = (char**)REALLOC(this->my_lines,i*(sizeof(char*)));
				else this->my_lines = (char**)MALLOC(i*(sizeof(char*)));
				this->my_lines[i-1] = line;

				if (this->my_linenumbers) this->my_linenumbers = (int*)REALLOC(this->my_linenumbers,i*(sizeof(int)));
				else this->my_linenumbers = (int*)MALLOC(i*(sizeof(int)));
				this->my_linenumbers[i-1] = line_indice;
			}

			line_indice++;
		}
		this->my_sizearray = i;

		this->current_position = i - 1;
	}

	return bOK;
}
/*------------------------------------------------------------------------*/
char **HistorySearch::getSearchResult(void)
{
	char **lines = NULL;
	lines = (char**)MALLOC(this->my_sizearray*(sizeof(char*)));
	if (lines)
	{
		char *currentline = NULL;
		int i = 0;
		for (i = 0; i < this->my_sizearray;i++)
		{
			if (this->my_lines[i])
			{
				char *currentline = (char*)MALLOC((strlen(this->my_lines[i])+1)*sizeof(char));
				strcpy(currentline,this->my_lines[i]);
				lines[i] = currentline;
			}
		}
	}
	return lines;
}
/*------------------------------------------------------------------------*/
int *HistorySearch::getSearchResultIndice(void)
{
	int *line = NULL;

	line = (int*)MALLOC(sizeof(int)*this->my_sizearray);

	if (line)
	{
		int i = 0;
		for (i = 0;i < this->my_sizearray;i++) line[i] = this->my_linenumbers[i];
	}
	return line;
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
	BOOL check1 = FALSE,check2 = FALSE,check3 = FALSE,check4 = FALSE;

	if (!this->Commands.empty()) 
	{
		this->Commands.clear();
		check1 = TRUE;
	}

	if (this->my_token) 
	{
		FREE(this->my_token);
		this->my_token = NULL;
		check1 = TRUE;
	}

	if (this->my_lines)
	{
		int i = 0;
		for (i = 0;i < this->my_sizearray; i++)
		{
			if (this->my_lines[i]) 
			{
				FREE(this->my_lines[i]);
				this->my_lines[i] = NULL;
			}
		}
		FREE(this->my_lines);
		this->my_lines = NULL;
		check3 = TRUE;
	}

	if (this->my_linenumbers)
	{
		FREE(this->my_linenumbers);
		this->my_linenumbers = NULL;
		check4 = TRUE;
	}

	this->my_sizearray = 0;
	this->current_position = 0;

	if (check1 && check2 && check3 && check4) bOK = TRUE;

	return bOK;
}
/*------------------------------------------------------------------------*/
char * HistorySearch::getPreviousLine(void)
{
	char *line = NULL;

	if (my_lines)
	{
		if (my_lines[this->current_position])
		{
			line = (char *)MALLOC(sizeof(char)*(strlen(my_lines[this->current_position])+1));
			strcpy(line,my_lines[this->current_position]);
		}

		this->current_position--;
		if (this->current_position < 0) this->current_position = 0;
	}
	return line;
}
/*------------------------------------------------------------------------*/
char * HistorySearch::getNextLine(void)
{
	char *line = NULL;

	if (my_lines)
	{
		if (my_lines[this->current_position])
		{
			line = (char *)MALLOC(sizeof(char)*(strlen(my_lines[this->current_position])+1));
			strcpy(line,my_lines[this->current_position]);
		}

		this->current_position++;
		if (this->current_position >= this->my_sizearray ) this->current_position = this->my_sizearray - 1;
	}
	return line;
}
/*------------------------------------------------------------------------*/

