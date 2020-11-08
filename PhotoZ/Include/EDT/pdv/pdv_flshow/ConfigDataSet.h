
#ifndef _EDT_ConfigDataSet_H_
#define _EDT_ConfigDataSet_H_

#include <stdio.h>

/***************************************/
/* data types */
/***************************************/

#include "cfgfileset.h"

/* The ConfigDataSet contains all the data values in one .cfg file */
class ConfigDataSet {

    char *_internal_buffer;
    int max_line_length;
    int allocated_line_length;
    char * internal_buffer();


public:

    char *FileName;

    int nItems;
    int nItemsAllocated;

    int current_item;

    bool strip_quotes;
    bool copy_value; // assume value is string and strdup it

    unsigned char bIgnoreCase;

    CfgFileItem *pItems;

    char *comment;

    ConfigDataSet();
    ~ConfigDataSet();

    void Reset();

    void init();

    void reset();

    void destroy();

    CfgFileItem *new_item();
    void add_item(const char * Name, const char *Value, const char *Comment);
    int lookup_index(const char *Name);
    CfgFileItem * lookup_item(const char *Name);
    void delete_item(int n);
    void delete_named_item(const char *Name);
    void set_values(int nItem, const char *Name, 
					    const char *Value, const char *Comment);

    int get_int(const char  *Name, int *pValue);
    int get_float(const char  *Name, float *pValue);
    int get_double(const char  *Name, double *pValue);
    char *lookup(const char *Name);

    int set_string(const char  *Name, const char *Value);
    int set_int(const char  *Name, int iValue);
    int set_float(const char  *Name, float dValue);
    int set_double(const char  *Name, double dValue);

    /* modified flags */

    void set_modified(const char *Name);
    void clear_modified(const char *Name);
    bool is_modified(const char *Name);

    /* File access functions */

    bool read_line(char *line, bool continuation);

    int load_file(FILE *f);

    int write_line(FILE *f, int line);

    int save_file(FILE *f);

    int load(const char *Name);

    int save(const char *Name);

    CfgFileItem *FirstItem() { 
	current_item = 0;
	if (nItems)
	    return pItems;
	else
	    return NULL;
    }


    CfgFileItem *NextItem()
    {
	if (current_item < nItems)
	{
	    int i = current_item ++;
    
	    return pItems + i;
	}
	else
	{
	    return NULL;
	}
    }

    char * line_string(const int line, char *buf = NULL, int linemax = 0);
    char * line_string(CfgFileItem *pItem, char *buf = NULL, int linemax = 0);

    char * FirstLine(char *buf = NULL, int linemax = 0)
    { 
	current_item = 0;
	if (nItems)
	    return line_string(current_item, buf, linemax);
	else
	    return NULL;
    }

    char * NextLine(char *buf = NULL, int linemax = 0)
    {
	if (current_item < nItems)
	{
	    int i = current_item ++;
    
	    return line_string(current_item, buf, linemax);
	}
	else
	{
	    return NULL;
	}
    }

    int get_max_line_length();
    


};


/***************************************/
/* access functions */
/***************************************/




#endif
