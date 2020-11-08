
#ifndef	PDV_DEP_CONFIG_H
#define	PDV_DEP_CONFIG_H

#include "cfgfileset.h"

#ifdef __cplusplus

extern "C" {

#endif

extern CfgFileSet *pdv_get_config_fields();

int pdv_parse_cfg_file_input(CfgFileItem *pItem, CfgFileItem *fmt_item,	PdvDependent *dd_p);

int pdv_format_cfg_file_output(CfgFileItem *pItem, CfgFileItem *fmt_item, PdvDependent *dd_p);

int pdv_parse_xilinx_item(CfgFileItem *pItem, Dependent	* dd_p);

CfgFileSet *pdv_cfg_file_read(char *cfgfile, PdvDependent * dd_p, int nofs_cfg, CfgFileSet *cfg);

int pdv_cfg_mark_nondefault(PdvDependent *dd_p,	CfgFileSet *cfg_s);


void pdv_cfg_write_file(CfgFileSet *cfg, FILE *f, unsigned int exclude_mask);

int pdv_cfg_write(CfgFileSet *cfg, const char *fname, unsigned int exclude_mask);


#ifdef __cplusplus

}

#endif

#endif


