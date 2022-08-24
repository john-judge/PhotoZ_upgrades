
#ifndef _EDT_CfgFileSet_H_
#define _EDT_CfgFileSet_H_

#include <stdio.h>

/***************************************/
/* data types */
/***************************************/


#include "edtinc.h"

#ifdef __cplusplus

extern "C" {

#endif


/* for items with multiple string possibilities */

typedef struct ConfigKeyValue {
    char *idstr;
    int  value;
} ConfigKeyValue;

#define CFG_FORMAT_HEX  1
#define CFG_FORMAT_METHOD 2
#define CFG_FORMAT_PATH	4
#define CFG_FORMAT_QUOTES	8
#define CFG_FORMAT_INONLY	16
#define CFG_FORMAT_EMPTY_COMMENT	32

#define CFG_STATUS_UNKNOWN	1
#define CFG_STATUS_MODIFIED	2
#define CFG_STATUS_NEW		4
#define CFG_STATUS_DEFAULT      8


typedef struct CfgFileItem {
    char *ItemName;
    char *ItemValue;
    char *ItemComment;
    int  base_offset;
    int  size;
    unsigned char  type;
    unsigned char  format_flags;
    unsigned char  bit_offset;
    unsigned char  copy_value;
    unsigned char  status;
    ConfigKeyValue *possible_values;
    char *original_string; /* for debug */
    int line_number;
    short group_number; /* for many to one config file mappings */
    short group_next;
} CfgFileItem;

void cfg_item_set_value(CfgFileItem *cfg_i, const char  *Value);

void cfg_item_append_value(CfgFileItem *cfg_i, const char  *value);


void cfg_item_set_name(CfgFileItem *cfg_i, const char *Name);


void cfg_item_set_comment(CfgFileItem *cfg_i, const char  *Comment);

void cfg_item_set_all_values(CfgFileItem *cfg_i, const char  *Name, const char  *Value, const char *Comment);


CfgFileItem *cfg_item_new(void);


/* init: set values for preallocated pItem.
 *  pItem: configuration Item which has been allocated, and returned probably by 
 *  either lookup_item, or new_item */

CfgFileItem *cfg_item_init_values(const char  *Name, 
				     const char  *Value, 
				     const char  *Comment, 
				     unsigned char value_is_string);

void cfg_item_clear(CfgFileItem *cfg_i);


void cfg_item_destroy(CfgFileItem *cfg_i);


size_t cfg_item_line_length(CfgFileItem *cfg_i);

#ifndef false
#define false 0
#endif


/* The CfgFileSet contains all the data values in one .cfg file */
/* The file is read into an array of CfgFileItems */

typedef struct _CfgFileSet {

    char *_internal_buffer;


    int max_line_length;
    int allocated_line_length;

    char *FileName;

    int nItems;
    int nItemsAllocated;

    int current_item;

    u_char strip_quotes;
    u_char copy_value; /* assume value is string and strdup it */
    u_char check_continuation;

    unsigned char ignore_case;

    CfgFileItem *pItems;

} CfgFileSet;
    


CfgFileSet	*new_cfg_set(void);

void		cfg_set_Reset(CfgFileSet *cfg_s);

void		cfg_set_init(CfgFileSet *cfg_s);

void		cfg_set_reset(CfgFileSet *cfg_s);

void		cfg_set_destroy(CfgFileSet *cfg_s);

CfgFileItem *   cfg_set_new_item(CfgFileSet *cfg_s);
CfgFileItem *	cfg_set_add_item(CfgFileSet *cfg_s, const char * Name, const char *Value, const char *Comment);
int		cfg_set_lookup_index(CfgFileSet *cfg_s, const char *Name);
CfgFileItem *   cfg_set_lookup_item(CfgFileSet *cfg_s, const char *Name);
void		cfg_set_delete_item(CfgFileSet *cfg_s, int n);
void		cfg_set_delete_named_item(CfgFileSet *cfg_s, const char *Name);
void		cfg_set_set_values(CfgFileSet *cfg_s, int nItem, const char *Name, 
					const char *Value, const char *Comment);

int		cfg_set_get_int(CfgFileSet *cfg_s, const char  *Name, int *pValue);
int		cfg_set_get_float(CfgFileSet *cfg_s, const char  *Name, float *pValue);
int		cfg_set_get_double(CfgFileSet *cfg_s, const char  *Name, double *pValue);
char *		cfg_set_lookup(CfgFileSet *cfg_s, const char *Name);

int		cfg_set_set_string(CfgFileSet *cfg_s, const char  *Name, const char *Value);
int		cfg_set_set_int(CfgFileSet *cfg_s, const char  *Name, int iValue);
int		cfg_set_set_float(CfgFileSet *cfg_s, const char  *Name, float dValue);
int		cfg_set_set_double(CfgFileSet *cfg_s, const char  *Name, double dValue);

/* modified flags */

void		cfg_set_set_modified(CfgFileSet *cfg_s, const char *Name);

void		cfg_set_clear_modified(CfgFileSet *cfg_s, const char *Name);

u_char		cfg_set_is_modified(CfgFileSet *cfg_s, const char *Name);

/* File access functions */

u_char		cfg_set_read_line(CfgFileSet *cfg_s, char *line, u_char continuation, int line_number);

int		cfg_set_load_file(CfgFileSet *cfg_s, FILE *f);

int		cfg_set_write_line(CfgFileSet *cfg_s, FILE *f, int line);

int		cfg_set_save_file(CfgFileSet *cfg_s, FILE *f);

int		cfg_set_load(CfgFileSet *cfg_s, const char *Name);

int		cfg_set_save(CfgFileSet *cfg_s, const char *Name);

CfgFileItem *   cfg_set_FirstItem(CfgFileSet *cfg_s);

CfgFileItem *	cfg_set_NextItem(CfgFileSet *cfg_s);

char *		cfg_set_line_string(CfgFileSet *cfg_s, const int line, char *bufL, int linemax);

char *		cfg_set_item_line_string(CfgFileSet *cfg_s, CfgFileItem *pItem, char *buf, int linemax);

char *		cfg_set_FirstLine(CfgFileSet *cfg_s, char *buf, int linemax);

char *		cfg_set_NextLine(CfgFileSet *cfg_s, char *buf, int linemax);

int		cfg_set_get_max_line_length(CfgFileSet *cfg_s);

#ifdef __cplusplus

}

#endif

/***************************************/
/* access functions */
/***************************************/




#endif
