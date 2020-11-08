
#include <stdio.h>

#include <stdlib.h>

#ifndef __APPLE__
#include <malloc.h>
#endif

#include <ctype.h>

#include <string.h>


#include "ConfigDataSet.h"

#define ALLOC_GRAIN 16



void
ConfigDataSet::init()

{

    pItems = NULL;
    FileName = NULL;
    nItems = 0;
    nItemsAllocated = 0;
    strip_quotes = false;
    copy_value = true;
    bIgnoreCase = false;
    comment = NULL;

    _internal_buffer = NULL;
    max_line_length = 0;
    allocated_line_length = 0;

}

void
ConfigDataSet::destroy()

{
    int i;

    if (pItems)
    {
        for (i=0;i< nItems;i++)
            cfg_item_destroy(&pItems[i]);

        free(pItems);
        pItems = NULL;
        nItems = 0;
        nItemsAllocated = 0;
    }

    if (FileName)
    {
        free(FileName);
        FileName = NULL;
    }

}

ConfigDataSet::ConfigDataSet()

{
    init();
}

ConfigDataSet::~ConfigDataSet()

{
    destroy();
}


CfgFileItem *
ConfigDataSet::new_item()

{

    nItems++;
    if (nItems > nItemsAllocated)
    {

    int nAllocated = nItemsAllocated + ALLOC_GRAIN;


    if (pItems)
    {
        pItems = (CfgFileItem *) realloc(pItems, 
            sizeof(CfgFileItem) * nAllocated);

        memset(pItems + nItemsAllocated, 0, sizeof(CfgFileItem) * ALLOC_GRAIN);
        
    }
    else
    {
        pItems = (CfgFileItem *) calloc(nAllocated, sizeof(CfgFileItem));

    }

    nItemsAllocated += ALLOC_GRAIN;

    }

    if (copy_value)
	pItems[nItems - 1].copy_value = true;

    return pItems + nItems - 1;

}

void ConfigDataSet::add_item(const char  * Name, const char  *Value, const char  *Comment)

{
    CfgFileItem *pItem;

    if (pItem = lookup_item(Name))
    {
    cfg_item_set_value(pItem,Value);
    cfg_item_set_comment(pItem,Comment);
    }
    else
    {
    pItem = new_item();
    cfg_item_set_all_values(pItem,Name, Value, Comment);
    }

}

CfgFileItem * ConfigDataSet::lookup_item(const char  *TestName)

{
    int index;

    if ((index = lookup_index(TestName)) >= 0)
    {
     return pItems + index;
    }

    return NULL;
}

int ConfigDataSet::lookup_index(const char  *TestName)

{
    int index;

    if (TestName)
        for (index = 0;index < nItems; index++)
            if (pItems[index].ItemName)
	    {
		if (!strcmp(pItems[index].ItemName, TestName))
                    return index;
	    }

    return -1;
}

char  *ConfigDataSet::lookup(const char  *Name)

{
    CfgFileItem *pItem;

    if (pItem = lookup_item(Name))
        return pItem->ItemValue;
    else
        return NULL;

}

int ConfigDataSet::get_int(const char  *Name, int *pValue)

{
    char *value;

    if (value = lookup(Name))
    {
        *pValue = strtol(value,NULL,0);

        return 1;
    }
    else
        return 0;

}

int ConfigDataSet::get_double(const char  *Name, double *pValue)

{
    char *value;

    if (value = lookup(Name))
    {
        *pValue = atof(value);

        return 1;
    }
    else
        return 0;

}
int ConfigDataSet::get_float(const char  *Name, float *pValue)

{
    char *value;

    if (value = lookup(Name))
    {
        *pValue = (float) atof(value);

        return 1;
    }
    else
        return 0;

}
int  ConfigDataSet::set_string(const char  *Name, const char *Value)

{

    add_item( Name, Value, NULL);
    
    return 1;

}

int ConfigDataSet::set_int(const char  *Name, int iValue)

{
    char Value[256];

    sprintf(Value, "%d", iValue);

    add_item( Name, Value, NULL);
    
    return 1;
}

int ConfigDataSet::set_double(const char  *Name, double dValue)

{
    char Value[256];

    sprintf(Value, "%g", dValue);

    add_item( Name, Value, NULL);
    
    return 1;

}

int ConfigDataSet::set_float(const char  *Name, float dValue)

{
    char Value[256];

    sprintf(Value, "%g", dValue);

    add_item( Name, Value, NULL);
    
    return 1;

}
void ConfigDataSet::delete_item(int n)

{

    if (n >= 0 && n < nItems)
    {
    /* delete element of array */
	cfg_item_destroy(pItems+n);

	if (n < nItems - 1)
	{
        
	    memcpy(pItems + n, pItems + n + 1, 
		sizeof(CfgFileItem) * (nItems - n));
	}

	nItems--;

    }

}

void ConfigDataSet::delete_named_item(const char  *Name)

{
    int n;

    n = lookup_index( Name);

    delete_item(n);

}

void ConfigDataSet::set_values(int nItem, const char  *Name, const char  *Value, const char  *Comment)

{
    if ((nItem >= 0) && (nItem < nItems))
    {
	cfg_item_set_all_values(pItems + nItem, Name, Value, Comment);
    }
}


static 
char * strtrim(char *s)

{
    char *news = s;

    if (!s)
        return s;

    while (isspace(*news))
        news++;

    if (*news)
    {
        int last = strlen(news) -1;
    
        while (last && isspace(news[last]))
            last--;

        news[last+1] = 0;
    }

    return news;
}

static
char * str_dequote(char *s)

{
    char *news = s;

    if (!s)
        return s;

    if (news[0] == '"')
        news++;

    if (*news)
    {
        int last = strlen(news) -1;
    
	if (news[last] == '"')
	{
	    news[last] = 0;
	}
    }

    return news;

}


static int
parse_name_value(char *s, char **namep, char **valuep, int dequote)

{
    char *name;
    char *value;
    char *colon;

    if (s)
    {
        colon = strchr(s,':');

        if (colon)
        {
            name = s;

            *colon = 0;

            name = strtrim(name);
        
            value = colon + 1;

            value = strtrim(value);

	    if (dequote)
		value = str_dequote(value);

            *namep = name;
            *valuep = value;

            return 1;
        }
    }

    return 0;
}

static int check_continuation(char *s, char ** namep)

{
    char *name;
    char *open_char;

    if (s)
    {
	open_char = strchr(s,'{');

        if (open_char)
        {
            name = s;

            *open_char = 0;

            name = strtrim(name);
 
	    *namep = name;

            return 1;
        }
    }

    return 0;
}



bool ConfigDataSet::read_line(char *buffer, bool continuation)

{
    char *name;
    char *value;

    // in continuation
    // go until '}' seen - ignore comments?
    //

    if (continuation)
    {
	char * commentpos = strchr(buffer,'#');
	char * endpos = strchr(buffer, '}');

	if (endpos && (!commentpos || endpos < commentpos))
	{
	    return false;
	}
	else
	{
	    cfg_item_append_value(pItems+nItems-1, buffer);
	    return true;
	}

    }
    else
    {
	if (buffer[strlen(buffer)-1] == '\n')
	    buffer[strlen(buffer)-1] = 0;

	if (buffer[0] == '#')
	{
	    add_item(NULL,NULL,buffer + 1);
	}
	else if (check_continuation(buffer,&name))
	{
	    add_item(name, "\n", NULL);
	    return true;
	}
	else
	{
	    if (parse_name_value(buffer, &name, &value, strip_quotes))
	    {
		if (strlen(name) && strlen(value))
		    add_item( name, value, NULL);        
	    }
	    else
	    {
		add_item(NULL,NULL,buffer);

	    }
	}
    }

    return false;
}

int ConfigDataSet::load_file(FILE *f)

{
    bool continuation = false;

    char buffer[1024];

    while (fgets(buffer, 1023, f))
    {
        continuation = read_line(buffer, continuation);
    }

    return 0;

}

char * ConfigDataSet::line_string(const int line, char *buf, int linemax)

{


    if (line > 0 && line < nItems)
    {

	return line_string(pItems + line, buf, linemax);

    }

    return NULL;

}

char * ConfigDataSet::line_string(CfgFileItem *pItem, char *buf, int linemax)

{
    char *pbuf;
    
    if (!buf)
	pbuf = internal_buffer();

    if (pbuf && pItem)
    {
	pbuf[0] = 0;

        if (pItem->ItemName)
            sprintf(pbuf, "%s: ", pItem->ItemName);
        if (pItem->ItemValue)
            sprintf(pbuf + strlen(pbuf), "%s ", pItem->ItemValue);
        if (pItem->ItemComment)
            sprintf(pbuf + strlen(pbuf), "# %s", pItem->ItemComment);

        strcat(pbuf, "\n");
    }

    return pbuf;

}

int ConfigDataSet::write_line(FILE *f, int line)

{
    if (f && line < nItems)
    {
        if (pItems[line].ItemName)
            fprintf(f, "%s: ", pItems[line].ItemName);
        if (pItems[line].ItemValue)
            fprintf(f, "%s", pItems[line].ItemValue);
        if (pItems[line].ItemComment)
            fprintf(f, "# %s", pItems[line].ItemComment);

        fprintf(f, "\n");
    }

    return 0;
}

int ConfigDataSet::save_file(FILE *f)

{
    int i;
    int rc;

    for (i=0; i < nItems;i++)
        if ((rc = write_line( f, i)) != 0)
            return rc;

    return 0;

}

int ConfigDataSet::load(const char *Name)

{
    FILE *f;

    if (f = fopen(Name, "r"))
    {
        load_file(f);

        fclose(f);

        return 0;
    }
    else
        return -1;


}

int ConfigDataSet::save(const char *Name)

{
    FILE *f;


    if (f = fopen(Name, "wb+"))
    {

        save_file(f);

        fclose(f);

        return 0;

    }
    else
        return -1;

}

int ConfigDataSet::get_max_line_length()

{
    max_line_length = 0;
    int i;

    for (i=0;i<nItems; i++)
    {
	int l = cfg_item_line_length(pItems+i);

	if (l > max_line_length)
	    max_line_length = l;
    }

    return max_line_length;

}


char * ConfigDataSet::internal_buffer()

{
    
    if (max_line_length == 0)
    {
	get_max_line_length();
    }

    if (max_line_length == 0)
	return NULL;

    if (allocated_line_length < max_line_length)
    {
	free(_internal_buffer);
	_internal_buffer = NULL;

    }

    if (!_internal_buffer)
    {
	allocated_line_length = max_line_length + 80;
	_internal_buffer = (char *) malloc(allocated_line_length);
	_internal_buffer[0] = 0;
    }
 
    return _internal_buffer;

}
