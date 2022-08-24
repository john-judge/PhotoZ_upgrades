
#include <stdio.h>

#include <stdlib.h>

#ifndef __APPLE__
#include <malloc.h>
#endif

#include <ctype.h>

#include <string.h>


#include "cfgfileset.h"

#define ALLOC_GRAIN 16

#ifdef WIN32
#define strcasecmp stricmp
#endif

/**
 * cfg_item_set_value
 *
 * Set the ItemValue field. If copy_value is set, allocate a duplicate string, otherwise
 * copy the pointer (can be used to set opaque pointer as the value)
 *
 * @param cfg_i: item pointer to set value 
 *
 * @param Value: string to set on item
 */

void cfg_item_set_value(CfgFileItem *cfg_i, const char  *Value)

{
    if (cfg_i->ItemValue && cfg_i->copy_value)
    {
        free(cfg_i->ItemValue);
    }

    if (Value)
    {
	if (cfg_i->copy_value)
	    cfg_i->ItemValue = strdup(Value);
	else
	{
	    memcpy(&cfg_i->ItemValue,&Value, sizeof(char *));
	}
    }
    else
        cfg_i->ItemValue = NULL;

}

/**
 * cfg_item_append_value
 *
 * Append string to current ItemValue. If copy_value is set, allocate a new string, otherwise
 * return (could be flagged as error)
 *
 * @param cfg_i: item pointer to set value 
 *
 * @param Value: string to append
 */

void cfg_item_append_value(CfgFileItem *cfg_i, const char  *Value)

{

    if (Value)
    {
	// must copy_value for append


	if (!cfg_i->copy_value)
	    return;


	if (cfg_i->ItemValue)
	{
	    char *oldItem = cfg_i->ItemValue;

	    size_t len = strlen(cfg_i->ItemValue) + strlen(Value);

	    cfg_i->ItemValue = (char *) malloc(len+1);
	    strcpy(cfg_i->ItemValue, oldItem);
	    strcat(cfg_i->ItemValue, Value);

	    free(oldItem);
	    
	}
	else
	    cfg_i->ItemValue = strdup(Value);
    }
   
}

/**
 * cfg_item_set_name
 *
 * Set the ItemName field for an item. Always copied to allocated string
 *
 * @param cfg_i: item pointer to set name for 
 *
 * @param Name: new name for item
 */

void cfg_item_set_name(CfgFileItem *cfg_i, const char *Name)

{
    if (cfg_i->ItemName)
    {
        free(cfg_i->ItemName);
    }

    if (Name)
        cfg_i->ItemName = strdup(Name);
    else
        cfg_i->ItemName = NULL;

}

/**
 * cfg_item_set_comment
 *
 * Set the ItemComment field for an item. Always copied to allocated string
 *
 * @param cfg_i: item pointer to set name for 
 *
 * @param Comment: new comment for item
 */

void cfg_item_set_comment(CfgFileItem *cfg_i, const char  *Comment)
{
    if (cfg_i->ItemComment)
    {
        free(cfg_i->ItemComment);
    }

    if (Comment)
        cfg_i->ItemComment = strdup(Comment);
    else
        cfg_i->ItemComment = NULL;

}

/**
 * cfg_item_set_all_values
 *
 * Set the ItemName, ItemValue, and ItemComment fields for an item. Any of these can be null, in
 *	    which case that pointer will be set to NULL
 *
 * @param cfg_i: item pointer to set values for.
 *
 * @param Name: new name for item
 * @param Value: new value for item
 * @param Comment: new comment for item
 */

void cfg_item_set_all_values(CfgFileItem *cfg_i, const char  *Name, const char  *Value, const char *Comment)

{
    cfg_item_set_name(cfg_i, Name);
    cfg_item_set_value(cfg_i, Value);
    cfg_item_set_comment(cfg_i, Comment);
}

/**
 * cfg_item_new
 *
 * Allocates and initializes a new item
 *
 * @return the new item pointer
 *
 */

CfgFileItem *
cfg_item_new()

{
    CfgFileItem *cfg_i = (CfgFileItem *) calloc(1,sizeof(CfgFileItem));

     return cfg_i;

}

/**
 * cfg_item_init_values
 *
 * Creates, then sets values for a new item.
 *  
 * @param Name: new name for item
 * @param Value: new value for item
 * @param Comment: new comment for item
 * @param value_is_string: whether to copy value as a string or store pointer
 */

CfgFileItem *cfg_item_init_values(const char  *Name, const char  *Value, const char  *Comment, unsigned char value_is_string)

{    
    CfgFileItem *cfg_i;

    cfg_i = cfg_item_new();

    cfg_i->copy_value = value_is_string;
    cfg_i->status = FALSE;

    cfg_item_set_all_values(cfg_i, Name, Value, Comment);

    return cfg_i;

}

/**
 * cfg_item_init_destroy
 *
 * By setting all elements to null destroys any allocated strings
 *
 * @param cfg_i: item pointer to destroy values for
 */  

void cfg_item_destroy(CfgFileItem *cfg_i)

{
    cfg_item_set_name(cfg_i, NULL);
    cfg_item_set_value(cfg_i, NULL);    
    cfg_item_set_comment(cfg_i, NULL);    
}

/**
 * cfg_item_line_length
 *
 * Computes line length as sum of three string values + 2 characters for spaces and 
 * colons. Ignores value if copy_value not set.
 *
 * @param cfg_i: item pointer to compute line length for
 * 
 * @return length of formatted line
 */  


size_t cfg_item_line_length(CfgFileItem *cfg_i)

{
    size_t len = 1; 

    if (cfg_i->ItemName)
	len += strlen(cfg_i->ItemName) + 2;
    if (cfg_i->copy_value && cfg_i->ItemValue)
	len += strlen(cfg_i->ItemValue) + 2;
    if (cfg_i->ItemComment)
	len += strlen(cfg_i->ItemComment) + 2;

    return len;
}

static char * cfg_set_internal_buffer(CfgFileSet *cfg_s);

CfgFileSet *new_cfg_set()

{
    CfgFileSet *cfg_s;

    cfg_s = (CfgFileSet *) calloc(1,sizeof(CfgFileSet));

    cfg_set_init(cfg_s);

    return cfg_s;

}

void
cfg_set_init(CfgFileSet *cfg_s)

{

    cfg_s->pItems = NULL;
    cfg_s->FileName = NULL;
    cfg_s->nItems = 0;
    cfg_s->nItemsAllocated = 0;
    cfg_s->strip_quotes = FALSE;
    cfg_s->copy_value = TRUE;
    cfg_s->ignore_case = FALSE;
    cfg_s->check_continuation = FALSE;

    cfg_s->_internal_buffer = NULL;
    cfg_s->max_line_length = 0;
    cfg_s->allocated_line_length = 0;

}

void
cfg_set_destroy(CfgFileSet *cfg_s)

{
    int i;

    if (cfg_s->pItems)
    {
        for (i=0;i< cfg_s->nItems;i++)
            cfg_item_destroy(&cfg_s->pItems[i]);

        free(cfg_s->pItems);
        cfg_s->pItems = NULL;
    }
    cfg_s->nItems = 0;
    cfg_s->nItemsAllocated = 0;
    
    if (cfg_s->FileName)
    {
        free(cfg_s->FileName);
        cfg_s->FileName = NULL;
    }

}

cfg_set_ConfigDataSet(CfgFileSet *cfg_s)

{
    cfg_set_init(cfg_s);
}


CfgFileItem *
cfg_set_new_item(CfgFileSet *cfg_s)

{

    cfg_s->nItems++;
    if (cfg_s->nItems > cfg_s->nItemsAllocated)
    {

    int nAllocated = cfg_s->nItemsAllocated + ALLOC_GRAIN;


    if (cfg_s->pItems)
    {
        cfg_s->pItems = (CfgFileItem *) realloc(cfg_s->pItems, 
            sizeof(CfgFileItem) * nAllocated);

        memset(cfg_s->pItems + cfg_s->nItemsAllocated, 0, sizeof(CfgFileItem) * ALLOC_GRAIN);
        
    }
    else
    {
        cfg_s->pItems = (CfgFileItem *) calloc(nAllocated, sizeof(CfgFileItem));

    }

    cfg_s->nItemsAllocated += ALLOC_GRAIN;

    }

    if (cfg_s->copy_value)
	cfg_s->pItems[cfg_s->nItems - 1].copy_value = TRUE;

    return cfg_s->pItems + cfg_s->nItems - 1;

}

CfgFileItem * cfg_set_add_item(CfgFileSet *cfg_s, const char  * Name, const char  *Value, const char  *comment)

{
    CfgFileItem *pItem;

    if (pItem = cfg_set_lookup_item(cfg_s,Name))
    {
    cfg_item_set_value(pItem,Value);
    cfg_item_set_comment(pItem,comment);
    }
    else
    {
    pItem = cfg_set_new_item(cfg_s);
    cfg_item_set_all_values(pItem,Name, Value,comment);
    }

    return pItem;

}

CfgFileItem * cfg_set_lookup_item(CfgFileSet *cfg_s, const char  *TestName)

{
    int index;

    if ((index = cfg_set_lookup_index(cfg_s, TestName)) >= 0)
    {
     return cfg_s->pItems + index;
    }

    return NULL;
}

int cfg_set_lookup_index(CfgFileSet *cfg_s, const char  *TestName)

{
    int index;

    if (TestName)
    {
	if (cfg_s->ignore_case)
	{
	    for (index = 0;index < cfg_s->nItems; index++)
            if (cfg_s->pItems[index].ItemName)
	    {
		if (!strcasecmp(cfg_s->pItems[index].ItemName, TestName))
                    return index;
	    }
	}
	else
	{
	    for (index = 0;index < cfg_s->nItems; index++)
            if (cfg_s->pItems[index].ItemName)
	    {
		if (!strcmp(cfg_s->pItems[index].ItemName, TestName))
                    return index;
	    }
	}

    }
    return -1;
}

char  *cfg_set_lookup(CfgFileSet *cfg_s, const char  *Name)

{
    CfgFileItem *pItem;

    if (pItem = cfg_set_lookup_item(cfg_s, Name))
        return pItem->ItemValue;
    else
        return NULL;

}

int cfg_set_get_int(CfgFileSet *cfg_s, const char  *Name, int *pValue)

{
    char *value;

    if (value = cfg_set_lookup(cfg_s, Name))
    {
        *pValue = strtol(value,NULL,0);

        return 1;
    }
    else
        return 0;

}

int cfg_set_get_double(CfgFileSet *cfg_s, const char  *Name, double *pValue)

{
    char *value;

    if (value = cfg_set_lookup(cfg_s, Name))
    {
        *pValue = atof(value);

        return 1;
    }
    else
        return 0;

}
int cfg_set_get_float(CfgFileSet *cfg_s, const char  *Name, float *pValue)

{
    char *value;

    if (value = cfg_set_lookup(cfg_s, Name))
    {
        *pValue = (float) atof(value);

        return 1;
    }
    else
        return 0;

}
int  cfg_set_set_string(CfgFileSet *cfg_s, const char  *Name, const char *Value)

{

    cfg_set_add_item(cfg_s,  Name, Value, NULL);
    
    return 1;

}

int cfg_set_set_int(CfgFileSet *cfg_s, const char  *Name, int iValue)

{
    char Value[256];

    sprintf(Value, "%d", iValue);

    cfg_set_add_item(cfg_s,  Name, Value, NULL);
    
    return 1;
}

int cfg_set_set_double(CfgFileSet *cfg_s, const char  *Name, double dValue)

{
    char Value[256];

    sprintf(Value, "%g", dValue);

    cfg_set_add_item(cfg_s,  Name, Value, NULL);
    
    return 1;

}

int cfg_set_set_float(CfgFileSet *cfg_s, const char  *Name, float dValue)

{
    char Value[256];

    sprintf(Value, "%g", dValue);

    cfg_set_add_item(cfg_s,  Name, Value, NULL);
    
    return 1;

}
void cfg_set_delete_item(CfgFileSet *cfg_s, int n)

{

    if (n >= 0 && n < cfg_s->nItems)
    {
    /* delete element of array */
	cfg_item_destroy(cfg_s->pItems+n);

	if (n < cfg_s->nItems - 1)
	{
        
	    memcpy(cfg_s->pItems + n, cfg_s->pItems + n + 1, 
		sizeof(CfgFileItem) * (cfg_s->nItems - n));
	}

	cfg_s->nItems--;

    }

}

void cfg_set_delete_named_item(CfgFileSet *cfg_s, const char  *Name)

{
    int n;

    n = cfg_set_lookup_index(cfg_s, Name);

    cfg_set_delete_item(cfg_s, n);

}

void cfg_set_set_values(CfgFileSet *cfg_s, int nItem, const char  *Name, const char  *Value, const char  *Comment)

{
    if ((nItem >= 0) && (nItem < cfg_s->nItems))
    {
	cfg_item_set_all_values(cfg_s->pItems + nItem, Name, Value, Comment);
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
        size_t last = strlen(news) -1;
    
        while (last && isspace(news[last]))
            last--;

        news[last+1] = 0;
    }

    return news;
}

static
char * str_dequote(char *s, int *had_quote)

{
    char *news = s;
    int had = 0;

    if (!s)
        return s;

    if (news[0] == '"')
    {
	had = 1;
	news++;
    }

    if (*news)
    {
        size_t last = strlen(news) -1;
    
	if (news[last] == '"')
	{
	    news[last] = 0;
	    had = 1;
	}
    }

    if (had_quote)
	*had_quote = had;

    return news;

}


static int
parse_name_value(char *s, char **namep, char **valuep, int dequote, int *had_quotes)

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
		value = str_dequote(value, had_quotes);

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



u_char cfg_set_read_line(CfgFileSet *cfg_s, char *buffer, u_char continuation, int line_number)

{
    char *name;
    char *value;
    CfgFileItem *pItem = NULL;

    char *original = strdup(buffer);

    if (continuation)
    {
	char * commentpos = strchr(buffer,'#');
	char * endpos = strchr(buffer, '}');

	if (endpos && (!commentpos || endpos < commentpos))
	{
	    return FALSE;
	}
	else
	{
	    pItem = cfg_s->pItems+cfg_s->nItems-1;

	    cfg_item_append_value(pItem, buffer);
	    return TRUE;
	}

    }
    else
    {
	if (buffer[strlen(buffer)-1] == '\n')
	    buffer[strlen(buffer)-1] = 0;

	if (buffer[0] == '#')
	{
	    cfg_set_add_item(cfg_s,NULL,NULL,buffer + 1);
	}
	else if (cfg_s->check_continuation && check_continuation(buffer,&name))
	{
	    cfg_set_add_item(cfg_s,name, "\n", NULL);
	    return TRUE;
	}
	else
	{
	    int had_quotes = 0;

	    if (parse_name_value(buffer, &name, &value, cfg_s->strip_quotes, &had_quotes))
	    {

		if (strlen(name))
		{
		    pItem = cfg_set_add_item(cfg_s, name, value, NULL);    
		    if (had_quotes)
			pItem->format_flags |= CFG_FORMAT_QUOTES;

		}
	    }
	    else
	    {
		cfg_set_add_item(cfg_s,NULL,NULL,buffer);

	    }
	}
    }

    if (pItem)
    {
	pItem->original_string = original;
	pItem->line_number = line_number;
    }
    else
	free(original);

    return FALSE;
}

int cfg_set_load_file(CfgFileSet *cfg_s, FILE *f)

{
    u_char continuation = FALSE;

    char buffer[1024];
    int line_number = 0;

    while (fgets(buffer, 1023, f))
    {
        continuation = cfg_set_read_line(cfg_s, buffer, continuation, line_number++);
    }

    return 0;

}

char * cfg_set_line_string(CfgFileSet *cfg_s, const int line, char *buf, int linemax)

{


    if (line > 0 && line < cfg_s->nItems)
    {

	return  cfg_set_item_line_string(cfg_s, cfg_s->pItems + line, buf, linemax);

    }

    return NULL;

}

char * cfg_set_item_line_string(CfgFileSet *cfg_s, CfgFileItem *pItem, char *buf, int linemax)

{
    char *pbuf;
    
    if (!buf)
	pbuf = cfg_set_internal_buffer(cfg_s);

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

int cfg_set_write_line(CfgFileSet *cfg_s, FILE *f, int line)

{
    if (f && line < cfg_s->nItems)
    {
        if (cfg_s->pItems[line].ItemName)
            fprintf(f, "%s: ", cfg_s->pItems[line].ItemName);
        if (cfg_s->pItems[line].ItemValue)
            fprintf(f, "%s", cfg_s->pItems[line].ItemValue);
        if (cfg_s->pItems[line].ItemComment)
            fprintf(f, "# %s", cfg_s->pItems[line].ItemComment);

        fprintf(f, "\n");
    }

    return 0;
}

int cfg_set_save_file(CfgFileSet *cfg_s, FILE *f)

{
    int i;
    int rc;

    for (i=0; i < cfg_s->nItems;i++)
        if ((rc = cfg_set_write_line(cfg_s,  f, i)) != 0)
            return rc;

    return 0;

}

int cfg_set_load(CfgFileSet *cfg_s, const char *Name)

{
    FILE *f;

    if (f = fopen(Name, "r"))
    {
        cfg_set_load_file(cfg_s, f);

        fclose(f);

        return 0;
    }
    else
        return -1;


}

int cfg_set_save(CfgFileSet *cfg_s, const char *Name)

{
    FILE *f;

    char *mode = "wt+";

    if (f = fopen(Name, mode))
    {

        cfg_set_save_file(cfg_s, f);

        fclose(f);

        return 0;

    }
    else
        return -1;

}

int cfg_set_get_max_line_length(CfgFileSet *cfg_s)

{
    int i;

     cfg_s->max_line_length = 0;
    for (i=0;i<cfg_s->nItems; i++)
    {
	size_t l = cfg_item_line_length(cfg_s->pItems+i);

	if (l > (size_t) cfg_s->max_line_length)
	    cfg_s->max_line_length = (int) l;
    }

    return cfg_s->max_line_length;

}


static char * cfg_set_internal_buffer(CfgFileSet *cfg_s)

{
    
    if (cfg_s->max_line_length == 0)
    {
	cfg_set_get_max_line_length(cfg_s);
    }

    if (cfg_s->max_line_length == 0)
	return NULL;

    if (cfg_s->allocated_line_length < cfg_s->max_line_length)
    {
	free(cfg_s->_internal_buffer);
	cfg_s->_internal_buffer = NULL;

    }

    if (!cfg_s->_internal_buffer)
    {
	cfg_s->allocated_line_length = cfg_s->max_line_length + 80;
	cfg_s->_internal_buffer = (char *) malloc(cfg_s->allocated_line_length);
	cfg_s->_internal_buffer[0] = 0;
    }
 
    return cfg_s->_internal_buffer;

}

CfgFileItem * cfg_set_FirstItem(CfgFileSet *cfg_s) 

{ 
    cfg_s->current_item = 0;
    if (cfg_s->nItems)
	return cfg_s->pItems;
    else
	return NULL;
}


CfgFileItem * cfg_set_NextItem(CfgFileSet *cfg_s)

{
    if (cfg_s->current_item < cfg_s->nItems)
    {
	int i = cfg_s->current_item ++;

	return cfg_s->pItems + i;
    }
    else
    {
	return NULL;
    }
}
char * cfg_set_FirstLine(CfgFileSet *cfg_s, char *buf, int linemax)
{ 
    cfg_s->current_item = 0;
    if (cfg_s->nItems)
	return cfg_set_line_string(cfg_s, cfg_s->current_item, buf, linemax);
    else
	return NULL;
}

char * cfg_set_NextLine(CfgFileSet *cfg_s, char *buf, int linemax)
{
    if (cfg_s->current_item < cfg_s->nItems)
    {
	int i = cfg_s->current_item ++;

	return cfg_set_line_string(cfg_s, cfg_s->current_item, buf, linemax);
    }
    else
    {
	return NULL;
    }
}
