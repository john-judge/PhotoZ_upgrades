/* #pragma ident "@(#)pciload.h	1.9 07/11/01 EDT" */
#ifndef INCLUDE_pciload_h
#define INCLUDE_pciload_h

#ifdef _NT_
 #define DIRECTORY_CHAR '\\'
#else
 #define DIRECTORY_CHAR '/'
#endif

#define	MAX_STRING	128

#define MIN_BIT_SIZE_ANY	0x4000
#define MIN_BIT_SIZE_X		0x8000
#define MIN_BIT_SIZE_XLA	0x14000
#define MIN_BIT_SIZE_BT		0x20000
#define MIN_BIT_SIZE_SPI	0x60000
#define MIN_BIT_SIZE_AMD512	0x300000

#define FTYPE_X		0
#define FTYPE_BT	1
#define FTYPE_LTX	2
#define FTYPE_SPI	3
#define FTYPE_BT2	4
#define FTYPE_F16	5

#define	EDT_ROM_JUMPER	0x01
#define	EDT_5_VOLT	0x02
/*
 * status bits
 * bits 2-7 are Xilinx ID for 4028 and above.
 * shift down 2 and we get a number for the boot controller
 */
#define STAT_PROTECTED      	0x01
#define STAT_5V	            	0x02
#define STAT_IDMASK             0x7c
#define STAT_IDSHFT             2

/*
 * command bits for the 4028xla
 * boot controller
 */
#define	BT_READ		0x0100
#define	BT_WRITE	0x0200
#define BT_INC_ADD	0x0400
#define	BT_A0		0x0800
#define	BT_A1		0x1000
#define	BT_RSVD 	0x2000
#define	BT_REINIT	0x4000
#define BT_EN_READ	0x8000
#define	BT_LD_LO	BT_WRITE
#define	BT_LD_MID	BT_WRITE | BT_A0
#define	BT_LD_HI	BT_WRITE | BT_A1
#define	BT_LD_ROM	BT_WRITE | BT_A0 | BT_A1
#define BT_RD_ROM	BT_READ | BT_A0 | BT_A1 | BT_EN_READ
#define BT_RD_FSTAT	BT_READ | BT_EN_READ
#define BT_RD_PALVER	BT_READ | BT_A0 | BT_EN_READ
#define	BT_MASK		0xff00

#define IS_DEFAULT_SECTOR -6167


extern int sect;

#define MAX_BOARD_SEARCH 32
#define NUM_DEVICE_TYPES 5

typedef enum {
    UnknownMagic,
    XilinxMagic = 1,
    AlteraMagic = 2
} FPGAMagic;

/*tasks - 
  1. enumerate boards.
  2. load bitfile.
  a. from specified name
  b. auto update
  3. verify bitfile.
  4. set serial number values
  5. display one board's values
 */
typedef enum {
    Enumerate,
    LoadProm,
    AutoUpdate,
    VerifyOnly,
    SetSerial,
    Display,
    Erase
} PciloadState;


#define BAD_PARMS_BLOCK ((EdtPromParmBlock *) 0xffffffff)

#ifndef _KERNEL

#include "edt_bitload.h"

EDTAPI void   warnuser(EdtDev *edt_p, char *fname, int sector);
EDTAPI void   warnuser_ltx(EdtDev *edt_p, char *fname, int unit, int hub);

EDTAPI int    check_id_stuff(char *bid, char *pid, int devid, int verify_only, char *fname);
EDTAPI void   getinfo(EdtDev *edt_p, int promcode, int segment, char *pid, char *esn, char *osn, int verbose);
EDTAPI void   getinfonf(EdtDev *edt_p, int promcode, int segment, char *pid, char *esn, char *osn, int verbose);

EDTAPI void   program_sns(EdtDev *edt_p, int promtype, char *new_esn, char *new_osn, int sector, char *id, int verbose);
EDTAPI void   print_flashstatus(char stat, int sector, int frdata, int verbose);
EDTAPI void   edt_get_sns(EdtDev *edt_p, char *esn, char *osn);
EDTAPI void   edt_get_osn(EdtDev *edt_p, char *osn);
EDTAPI int    edt_flash_get_debug_fast();
EDTAPI u_int  edt_flash_writesize(int ftype);
EDTAPI int    edt_flash_set_debug_fast(int val);
EDTAPI int    edt_flash_get_do_fast();
EDTAPI void   edt_flash_set_do_fast(int val);
EDTAPI int    edt_flash_get_force_slow();
EDTAPI void   edt_flash_set_force_slow(int val);
EDTAPI void   edt_flash_block_program(EdtDev *edt_p, u_int addr, u_char *data, u_int nbytes, int ftype);
EDTAPI u_char *edt_flash_block_read(EdtDev *edt_p, u_int addr, u_char *buf, u_int size, int ftype);
EDTAPI char   *edt_flash_read_string(EdtDev *edt_p, u_int addr, char *buf, u_int size, int ftype);
EDTAPI void   edt_zero(void *s, size_t n);
EDTAPI void   edt_get_esn(EdtDev *edt_p, char *esn);
EDTAPI int    edt_get_max_promcode();

EDTAPI int    edt_ask_info(int devid, Edt_embinfo *si);
EDTAPI int    edt_ask_reboot(EdtDev *edt_p);
EDTAPI void   edt_ask_options(char *options);
EDTAPI void   edt_ask_rev(int *rev);
EDTAPI void   edt_ask_clock(int *clock, char *extra_txt);
EDTAPI void   edt_ask_pn(char *pn);
EDTAPI void   edt_ask_sn(char *tag, char *sn, int or_none);
EDTAPI int    edt_ask_nmacs(int *count, char *maclist);
EDTAPI void   edt_ask_maclist(int count, char *maclist);
EDTAPI int    edt_ask_addinfo();
EDTAPI void   edt_print_info(int devid, Edt_embinfo *si);

EDTAPI void   strip_newline(char *s);
EDTAPI int    pciload_isdigit_str(char *s);


typedef int (*EdtPciLoadVerify) (EdtDev *edt_p, 
                                 EdtBitfile *bitfile, 
                                 EdtPromData *pdata, 
                                 int promcode, 
                                 int segment, 
                                 int vfonly, 
                                 int warn, 
                                 int verbose);

int
program_verify_4013xla(EdtDev *edt_p, 
                        EdtBitfile *bitfile, 
                        EdtPromData *pdata, 
                        int promcode, 
                        int segment, 
                        int vfonly, 
                        int warn,
                        int verbose);
int
program_verify_XC2S300E(EdtDev *edt_p, 
                        EdtBitfile *bitfile, 
                        EdtPromData *pdata, 
                        int promcode, 
                        int segment, 
                        int vfonly, 
                        int warn,
                        int verbose);
int
program_verify_XC2S200(EdtDev *edt_p, 
                        EdtBitfile *bitfile, 
                        EdtPromData *pdata, 
                        int promcode, 
                        int segment, 
                        int vfonly, 
                        int warn,
                        int verbose);
int
program_verify_XC2S150(EdtDev *edt_p, 
                        EdtBitfile *bitfile, 
                        EdtPromData *pdata, 
                        int promcode, 
                        int segment, 
                        int vfonly, 
                        int warn,
                        int verbose);
int
program_verify_4028XLA(EdtDev *edt_p, 
                        EdtBitfile *bitfile, 
                        EdtPromData *pdata, 
                        int promcode, 
                        int segment, 
                        int vfonly, 
                        int warn,
                        int verbose);
int
program_verify_default(EdtDev *edt_p, 
                        EdtBitfile *bitfile, 
                        EdtPromData *pdata, 
                        int promcode, 
                        int segment, 
                        int vfonly, 
                        int warn,
                        int verbose);

int program_verify_SPI(EdtDev *edt_p, 
                       EdtBitfile *bitfile, 
                       EdtPromData *pdata, 
                       int promcode, 
                       int sector, 
                       int verify_only, 
                       int warn, 
                       int verbose);

int program_verify_4013E(EdtDev *edt_p, 
                       EdtBitfile *bitfile, 
                       EdtPromData *pdata, 
                       int promcode, 
                       int sector, 
                       int verify_only, 
                       int warn, 
                       int verbose);

int program_verify_4013XLA(EdtDev *edt_p, 
                       EdtBitfile *bitfile, 
                       EdtPromData *pdata, 
                       int promcode, 
                       int sector, 
                       int verify_only, 
                       int warn, 
                       int verbose);

#endif /* ! KERNEL */

#endif /* INCLUDE_pciload_h */
