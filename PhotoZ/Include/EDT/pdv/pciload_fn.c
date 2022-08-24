/*
* pciload_fn.c
*
*/
#include "edtinc.h"
#include "edt_bitload.h"
#include "pciload.h"
#include <stdlib.h>
#include <ctype.h>

volatile char *throwaway;

static void parse_id(char *idstr, char *lcaname, u_int *date, u_int *time);
static void remove_char(char *str, char ch);


/* If the last character of string s is a newline (\r or \n), 
* that character is replaced with NULL. */
void
strip_newline(char *s)
{
    int len = (int) strlen(s);
    if (len && ((s[len-1] == '\n') || (s[len-1] == '\r')))
        s[len-1] = '\0';
}

/* Converts all characters in string s to upper case */
static void
toupper_str(char *s)
{
    unsigned int i;
    unsigned int len = (int) strlen(s);

    for (i=0; i<len; i++)
        s[i] = toupper(s[i]);
}

/* Returns true (1) if every character in string s is a digit. */
int
pciload_isdigit_str(char *s)
{
    unsigned int i;
    unsigned int len = (unsigned int) strlen(s);

    for (i=0; i<len; i++)
        if (!isdigit(s[i]))
            return 0;
    return 1;
}
/* Returns true (1) if every character in string s is a digit. */
int
pciload_isxdigit_str(char *s)
{
    unsigned int i;
    unsigned int len = (unsigned int) strlen(s);

    for (i=0; i<len; i++)
        if (!isxdigit(s[i]))
            return 0;
    return 1;
}

/* Returns true (1) if every character in string s is alphanumeric. */
int isalnum_str(char *s)
{
    unsigned int i;
    unsigned int len = (unsigned int) strlen(s);

    for (i=0; i<len; i++)
        if (!isalnum(s[i]))
            return 0;
    return 1;
}


/** Takes a part number portion of the embedded infrormation string, as retrieved from
* the PCI FPGA information header via edt_get_sns; reformats and returns it in a
* "human readable" form (dashes added). A simple subroutine mainly for use by pciload
* and other EDT utility programs, but available for use by user apps if desired.
*
* The string returned is valid until the next invocation of this function.
* @param pn the part number without dashes.
* @param str the output string, which should be at least 13 characters long
* @see edt_get_esn, edt_parse_esn
*/
char *edt_fmt_pn(char *pn, char *str)
{
    if (strlen(pn) == 10)
        sprintf(str, "%c%c%c-%c%c%c%c%c-%c%c",
        pn[0],pn[1],pn[2],pn[3],pn[4],pn[5],pn[6],pn[7],pn[8],pn[9]);
    else if (strlen(pn) < 10)
        strcpy(str, pn);
    else strcpy(str, "0");
    return str;
}


Edt_bdinfo *
edt_detect_boards(char *dev, int unit, int *nunits, int verbose)
{
    return(edt_detect_boards_ids(dev, unit, NULL, nunits, verbose));
}


static char *device_name[NUM_DEVICE_TYPES] = {"pcd", "pdv", "p11w", "p16d", "p53b"};

Edt_bdinfo *
edt_detect_boards_filter(EdtBdFilterFunction filter, void *data, int *nunits, int verbose) 
{
    int     board_count = 0;
    Edt_bdinfo buf[NUM_DEVICE_TYPES * MAX_BOARD_SEARCH];
    Edt_bdinfo *ret;
    u_short  stat;
    int i;
    int     d, unit;
    EdtDev *temp;

    for (d = 0; d < NUM_DEVICE_TYPES; d++)
    {
        for (unit = 0; unit < MAX_BOARD_SEARCH; unit++)
        {
            temp = edt_open_quiet(device_name[d], unit);
            if ((temp != NULL) && (filter(device_name[d], unit, temp->devid, data)))
            {
                buf[board_count].bd_id = temp->devid;
                buf[board_count].promcode = edt_flash_prom_detect(temp, &stat);
                printf("set buf[%d] promcode to %d (%x)\n", board_count, buf[board_count].promcode);
                if (verbose)
                    printf("  Found a device %s:%d\n", device_name[d], unit);
                strcpy(buf[board_count].type, device_name[d]);
                buf[board_count].id = unit;
                board_count++;
            }
            if (temp != NULL) {
                edt_close(temp);
            }

        }
    }
    buf[board_count].type[0] = 0;
    buf[board_count].id = -1;
    board_count++;
    ret = (Edt_bdinfo *)malloc(board_count * sizeof(Edt_bdinfo));
    for (i = 0; i < board_count; i++)
    {
        strcpy(ret[i].type, buf[i].type);
        ret[i].id = buf[i].id;
        ret[i].bd_id = buf[i].bd_id;
        ret[i].promcode = buf[i].promcode;
    }
    *nunits = board_count-1;
    return ret;
}


/* this is the old one that only took one id. leaving in (and modifying)
* for backwards compat. 
*/
Edt_bdinfo *
edt_detect_boards_id(char *dev, int unit, u_int id, int *nunits, int verbose)
{
    u_int ids[2];

    ids[0] = id;
    ids[1] = 0xffff;
    return edt_detect_boards_ids(dev, unit, ids, nunits, verbose);
}

Edt_bdinfo *
edt_detect_boards_ids(char *dev, int unit, u_int *idlist, int *nunits, int verbose)
{
    int     board_count = 0;
    Edt_bdinfo buf[NUM_DEVICE_TYPES * MAX_BOARD_SEARCH];
    Edt_bdinfo *ret;
    int     all_ids = 0;
    int     d, i, j, gotid;
    EdtDev *temp;

    if (idlist == NULL)
        all_ids = 1;

    /*
    * three possibilities: dev specified, unit specified, or neither
    */
    if (dev && *dev != 0)
    {
        for (i = 0; i < MAX_BOARD_SEARCH; i++)
        {
            if ((temp = edt_open_quiet(dev, i)) != NULL)
            {
                gotid = 0;
                j = 0;
                if (idlist) do
                {
                    if (temp->devid == idlist[j])
                    {
                        gotid = 1;
                        continue;
                    }
                } while (idlist[++j] != 0xffff) ;

                if (all_ids || gotid)
                {
                    u_short stat;

                    buf[board_count].bd_id = temp->devid;
                    buf[board_count].promcode = edt_flash_prom_detect(temp, &stat);
                    if (verbose)
                        printf("  Found a device %s:%d\n", dev, i);
                    strcpy(buf[board_count].type, dev);
                    buf[board_count].id = i;
                    board_count++;
                }
                edt_close(temp);
            }
        }
    }
    else if (unit != -1)
    {
        for (d = 0; d < NUM_DEVICE_TYPES; d++)
        {
            if ((temp = edt_open_quiet(device_name[d], unit)) != NULL)
            {
                gotid = 0;
                j = 0;
                if (idlist) do
                {
                    if (temp->devid == idlist[j])
                    {
                        gotid = 1;
                        continue;
                    }
                } while (idlist[++j] != 0xffff) ;

                if (all_ids || gotid)
                {
                    u_short  stat;
                    buf[board_count].bd_id = temp->devid;
                    buf[board_count].promcode = edt_flash_prom_detect(temp, &stat);
                    if (verbose)
                        printf("  Found a device %s:%d\n", device_name[d], unit);
                    strcpy(buf[board_count].type, device_name[d]);
                    buf[board_count].id = unit;
                    board_count++;
                }
                edt_close(temp);
            }
        }
    }
    else
    {
        for (d = 0; d < NUM_DEVICE_TYPES; d++)
        {
            for (i = 0; i < MAX_BOARD_SEARCH; i++)
            {
                if ((temp = edt_open_quiet(device_name[d], i)) != NULL)
                {
                    gotid = 0;
                    j = 0;
                    if (idlist) do
                    {
                        if (temp->devid == idlist[j])
                        {
                            gotid = 1;
                            continue;
                        }
                    } while (idlist[++j] != 0xffff) ;

                    if (all_ids || gotid)
                    {
                        u_short  stat;
                        buf[board_count].bd_id = temp->devid;
                        buf[board_count].promcode = edt_flash_prom_detect(temp, &stat);
                        if (verbose)
                            printf("  Found a device %s:%d\n", device_name[d], i);
                        strcpy(buf[board_count].type, device_name[d]);
                        buf[board_count].id = i;
                        board_count++;
                    }
                    edt_close(temp);
                }
            }
        }
    }
    buf[board_count].type[0] = 0;
    buf[board_count].id = -1;
    board_count++;
    ret = (Edt_bdinfo *)malloc(board_count * sizeof(Edt_bdinfo));
    for (i = 0; i < board_count; i++)
    {
        strcpy(ret[i].type, buf[i].type);
        ret[i].id = buf[i].id;
        ret[i].bd_id = buf[i].bd_id;
        ret[i].promcode = buf[i].promcode;
    }
    *nunits = board_count-1;
    return ret;
}


#if 0

void
getinfo(EdtDev *edt_p, int promcode, int segment, char *id, char *esn, char *osn, int verbose)
{
    edt_readinfo(edt_p, promcode, segment, id, esn, osn);

    if (*id)
        printf("  PROM  id: <%s>", id);
    printf("\n");

}

void
getinfonf(EdtDev *edt_p, 
          int promcode, 
          int segment, 
          char *id, 
          char *esn, 
          char *osn,
          int verbose)
{
    edt_readinfo(edt_p, promcode, segment, id, esn, osn);

    if (*id)
        printf(" <%s>", id);
    printf("\n");

}

#endif

char *
edt_dev_from_id (int id)
{
    if (id == P11W_ID)   return "p11w";
    if (id == P16D_ID)   return "p16d";
    if (ID_IS_PCD(id))   return "pcd";
    if (ID_IS_PDV(id))   return "pdv";
    if (ID_IS_1553(id))  return "p53b";
    if (ID_IS_DUMMY(id)) return "dummy";
    return "unknown";
}

void 
warnuser(EdtDev *edt_p, char *fname, int sector)
{
    char    s[EDT_STRBUF_SIZE];
    char    *foa = "file";
    char *dev;

    dev = edt_dev_from_id(edt_p->devid);

#ifdef NO_FS
    if ((strlen(fname) < 4) || (strcmp(&fname[strlen(fname)-4], ".bit") != 0))
        foa = "embedded array";
#endif

    if (strcmp(fname, "ERASE") == 0)
        return;

    printf("\n");
    printf("pciload is preparing to re-burn the system interface firmware on\n");
    printf("logical sector %d of %s unit %d from the FPGA configuration %s\n'%s'.\n", sector, dev, edt_p->unit_no, foa, fname);
    printf("\nTo confirm that you want to update the board's system interface\n");
    printf("firmware, press <enter> now. Otherwise, press 'q', then <enter> -> ");

    throwaway = fgets(s, EDT_STRBUF_SIZE-1, stdin);
    if ((tolower(s[0]) != 'y') && (s[1])) /* allow 'y' since its what a lot of people do automatically */
    {
        printf("exiting.\n");
#ifdef VXWORKS
        return;
#else
        exit(0);
#endif
    }
}



/*
* local defines for mainboard type, for internal checking/
* conflict warnings
*/
#define MBTYPE_OTHER 0
#define MBTYPE_SS 1
#define MBTYPE_GS 2

static int
check_idname_type(char *name)
{
    int ret = MBTYPE_OTHER;

    if (((strlen(name) > 5) && strncasecmp(name, "pciss", 5) == 0)
        || ((strlen(name) > 6) && strncasecmp(name, "pci_ss", 6) == 0)
        || ((strlen(name) > 6) && strncasecmp(name, "pci-ss", 6) == 0))
        ret = MBTYPE_SS;
    else if (((strlen(name) > 5) && strncasecmp(name, "pcigs", 5) == 0)
        || ((strlen(name) > 6) && strncasecmp(name, "pci_gs", 6) == 0)
        || ((strlen(name) > 6) && strncasecmp(name, "pci-gs", 6) == 0))
        ret = MBTYPE_GS;
    return ret;
}

/*
* check the bitfile ID against the PROM id.
* If verify_only flag is set just print a message saying they differ and
* return 1 (means different).
* If bitfile date is older than PROM date, print a warning.
* If bitfile id indicates SS and PROM id indicates GS (or vice versa), print a
* warning.
* 
*
* ARGUMENTS
*    bitfile_id	id string from bitfile
*    prom_id           id string from prom
*    verify_only       flag whether we're verifying (print just "differ")
*                      or burning (print warning if bitfile older than PROM
*
* RETURNS
*    0 if the same, 1 if different
*
*/
int 
check_id_stuff(char *bitfile_id, char *prom_id, int devid, int task, char *fname)
{
    u_int   fdate, ftime, pdate = 0, ptime = 0, flashed_type, bitfile_type;
    char    s[MAX_STRING], bfname[MAX_STRING], prname[MAX_STRING];

    if (task == VerifyOnly)
    {
        if (strncmp(bitfile_id, prom_id, MAX_STRING) != 0)
        {
            return 0;
        }
    }

    parse_id(bitfile_id, bfname, &fdate, &ftime);
    parse_id(prom_id, prname, &pdate, &ptime);
    bitfile_type = check_idname_type(bfname);
    flashed_type = check_idname_type(prname);

    /* erase */
    if (strcmp(fname, "ERASE") == 0)
    {
        printf("\nPreparing to ERASE all EEprom info including board ID! To confirm,\n");
        printf("press <enter> now.  Otherwise, press 'q', then <enter> -> ");
        fflush(stdout);
        throwaway = fgets(s, MAX_STRING - 1, stdin);
        if (s[1])
        {
            printf("exiting\n");
            exit(0);
        }
    }
    else
    {
        /* check /warn if trying to load a peXdva bitfile into a peXdv (or vice versa) */
        if ((strlen(bfname) > 6) && (strlen(prname) > 6))
        {
            char tmp_prname[32];
            char tmp_bfname[32];

            strncpy(tmp_prname, prname, 6);
            strncpy(tmp_bfname, bfname, 6);
            tmp_bfname[2] = 'X';
            tmp_prname[2] = 'X';

            /* check / warn on #lanes mismatch  */
            if ((strncmp(bfname, "pe", 2) == 0) && (bfname[2] > '0') && (bfname[2] < '9'))
            {
                int bf_lanes = bfname[2] - '0';
                int bd_lanes = 0;

                if ((bf_lanes < 1) || (bf_lanes > 8))
                    bf_lanes = 0;

                if (ID_IS_1LANE(devid)) bd_lanes = 1;
                if (ID_IS_4LANE(devid)) bd_lanes = 4;
                if (ID_IS_8LANE(devid)) bd_lanes = 8;

                if ((bd_lanes != 0) && (bf_lanes != 0) && (bd_lanes != bf_lanes))
                {
                    printf("\nWARNING: You appear to be attempting to reprogram a PCI Express %d lane\n", bd_lanes);
                    printf("board with with an FPGA configuration file for %d lane boards. Doing so\n", bf_lanes);
                    printf("can render your board inoperable.  If you are sure you want to do this,\n");
                    printf("press <enter> now. Otherwise quit by pressing 'q'<enter> -> ");
                    fflush(stdout);
                    throwaway = fgets(s, MAX_STRING - 1, stdin);
                    if (s[1])
                    {
                        printf("exiting\n");
                        exit(0);
                    }
                }
            }

            /* check / warn on 'a' versus non 'a' */ 
            if (strncmp(tmp_bfname, "peXdv", 5) == 0 && strncmp(tmp_prname, "peXdv", 5) == 0)
            {
                if ((tmp_bfname[5] == 'a') && (tmp_prname[5] != 'a'))
                {
                    printf("\nWARNING: You appear to be attempting to program an older PCIe DV board\n");
                    printf("with an FPGA file for PCIe DVa model boards. Reprogramming a non-'a' board\n");
                    printf("with 'a' board firmware will likely render the board inoperative. If you\n");
                    printf("are sure you want to do this, press <enter> now. Otherwise quit by pressing\n");
                    printf("'q'<enter>,  then check the FPGA names before trying again -> ");
                    fflush(stdout);
                    throwaway = fgets(s, MAX_STRING - 1, stdin);
                    if (s[1])
                    {
                        printf("exiting\n");
                        exit(0);
                    }
                }
                else if 
                ((tmp_prname[5] == 'a') && (tmp_bfname[5] != 'a'))
                {
                    printf("\nWARNING: You appear to be attempting to re-flash a PCIe DVa board with an\n");
                    printf("FPGA file for older PCIe DV model boards. Flashing 'a' boards with non-'a'\n");
                    printf("board firmware will likely render the board inoperative. If you are sure you\n");
                    printf("want to do this, press <enter> now. Otherwise quit by pressing 'q'<enter>,\n");
                    printf("then check the FPGA names before trying again -> ");
                    fflush(stdout);
                    throwaway = fgets(s, MAX_STRING - 1, stdin);
                    if (s[1])
                    {
                        printf("exiting\n");
                        exit(0);
                    }
                }
            }
        }

        /* check / warn if loading SS bitfile into GS board (or vice versa) */
        if (((devid == PSS4_ID) || (devid == PSS16_ID)) && (bitfile_type == MBTYPE_GS))
        {
            printf("\nYou appear to be attempting to re-flash the PROM with an SS bitfile,\n");
            printf("but the board's hardware ID indicates it is a GS board. To program\n");
            printf("anyway, press <enter> now.  Otherwise, press 'q', then <enter> -> ");
            fflush(stdout);
            throwaway = fgets(s, MAX_STRING - 1, stdin);
            if (s[1])
            {
                printf("exiting\n");
                exit(0);
            }
        }
        if (((devid == PGS4_ID) || (devid == PGS16_ID)) && (bitfile_type == MBTYPE_SS))
        {
            printf("\nYou appear to be attempting to re-flash the PROM with a GS bitfile,\n");
            printf("but the board's hardware ID indicates it is an SS board. To program\n");
            printf("anyway, press <enter> now.  Otherwise, press 'q', then <enter> -> ");
            fflush(stdout);
            throwaway = fgets(s, MAX_STRING - 1, stdin);
            if (s[1])
            {
                printf("exiting\n");
                exit(0);
            }
        }

        /* check / warn re. date */
        if ((task == AutoUpdate) && ((fdate < pdate) || ((fdate == pdate) && (ftime < ptime))))
        {
            printf("\nALERT: Data in file is older than data in PROM. If your intention is to\n");
            printf("update to newer FPGA configuration file (as opposed to making a change in\n");
            printf("functionality for example), check the firmware timestamps before proceeding.\n");
            fflush(stdout);
        }
    }

    return 1;
}


static void
remove_char(char *str, char ch)
{
    char    tmpstr[EDT_STRBUF_SIZE];
    char   *p = tmpstr;
    char   *pp = str;

    strcpy(tmpstr, str);

    while (*p)
    {
        if (*p != ch)
            *pp++ = *p;
        ++p;
    }
    *pp = '\0';
}

/*
* check for and fix reversed date string
* that is, mm/dd/yyyy becomes yyyy/mm/dd
* (to work around Xilinx tools funny-business -- hopefully
* they won't do some other thing like dd/mm/yyyy)
*/
void
check_reversed(char *datestr)
{
    char mm[4], dd[4], yyyy[6];

    if ((!datestr) || (strlen(datestr) != 10))
        return;

    if ((strrchr(datestr, '/') - datestr) == 5)
        if (sscanf(datestr, "%[^/]/%[^/]/%s", dd, mm, yyyy) == 3)
            sprintf(datestr, "%s/%s/%s", yyyy, dd, mm);
}
/**
* given a date string in the format yy/mm/dd or yyy/mm/dd or yyyy/mm/dd,
* correct for the millenium. Will only work for the next 100 years or
* less, depending on rollover.
* 
* @return 0 on success, -1 if format error
*/
int
edt_fix_millennium(char *str, int rollover)
{
    char    tmpstr[16];
    int     century=20;
    int     yr;

    if (strlen(str) > 15)
    {
        str[16] = '\0';
        return -1;
    }
        
    if (str[2] == '/')				/* yy/mm/dd */
    {
        yr = ((str[0] - '0') * 10) + (str[1] - '0');
        if (yr >= rollover)
            century = 19;
    }
    else if (str[3] == '/')			/* yyy/mm/dd */
    {
        yr = ((str[1] - '0') * 10) + (str[2] - '0');
        sprintf(str, "%02d", yr);
    }
    else if (str[4] == '/')			/* yyyy/mm/dd */
        return 0;
    else return  -1;

    sprintf(tmpstr, "%d%s", century, str);
    strcpy(str, tmpstr);
    return 0;
}


static void
parse_id(char *idstr, char *lcaname, u_int * date, u_int * time)
{
    int     ss;
    char    id[32];
    char    datestr[32];
    char    timestr[32];

    ss = sscanf(idstr, "%s %s %s %s", lcaname, id, datestr, timestr);

    if (ss == 4)
    {
#if 0 /* DEBUG */
        printf("read %d strings from <%s>:\n", ss, idstr);
        printf("'%s'\n", lcaname); printf("'%s'\n", id); printf("'%s'\n",
            datestr); printf("'%s'\n", timestr);
#endif

        check_reversed(datestr);

        edt_fix_millennium(datestr, 90);
        remove_char(datestr, '/');
        *date = (u_int) atoi(datestr);
        remove_char(timestr, ':');
        *time = (u_int) atoi(timestr);

    }
}



void
edt_print_flashstatus(u_short stat, int sector, int frdata)
{
    edt_msg(EDT_MSG_INFO_1,"  FLASHROM_DATA %08x\n", frdata);

    if (sector != IS_DEFAULT_SECTOR)
    {
        edt_msg(EDT_MSG_INFO_1,"  Using sector %d\n", sector);
    }
    edt_msg(EDT_MSG_INFO_1,"  Flash environment status = %02x %02x (flash id %02x straps %02x):\n", stat >> 8, stat & 0xff, (stat & 0x78) >>2, stat & 0x3);

    if (stat & EDT_ROM_JUMPER)
        edt_msg(EDT_MSG_INFO_1,"    - Protected boot jumper removed or on pin 2-3; using unprotected sector\n");
    else
        edt_msg(EDT_MSG_INFO_1,"    - Protected boot jumper installed on pins 1-2, using PROTECTED sector\n");

    if (stat & EDT_5_VOLT)
        edt_msg(EDT_MSG_INFO_1,"    - Installed in 5 volt slot\n");
    else
        edt_msg(EDT_MSG_INFO_1,"    - Installed in 3.3 volt slot\n");
    if (!(stat & EDT_ROM_JUMPER))
    {
        edt_msg(EDT_MSG_FATAL,"  WARNING: Detected boot jumper(s) in PROTECTED position. Protected\n");
        edt_msg(EDT_MSG_FATAL,"           sectors may not contain the most recently burned FPGA.\n");
    }
}

/*
* ask whether to reboot the xilinx, do so if yes
*
* RETURNS 1 on success, 0 on no, -1 on failure
*/
int
edt_ask_reboot(EdtDev *edt_p)
{
    char s[EDT_STRBUF_SIZE];

    printf("Done. ");
    while(1)
    {

        u_short stat;
        int promcode = edt_flash_prom_detect(edt_p, &stat);

        printf("Attempt to reboot the %s board now? (y/n/h)[y] > ", edt_idstring(edt_p->devid, promcode));
        throwaway = fgets(s,127,stdin);
        if (tolower(s[0] == 'y') || s[0] == '\0')
        {
            if (edt_pci_reboot(edt_p) != 0)
            {
                printf("Couldn't reboot the xilinx; probably no s/w reboot support in this\nparticular bitfile.\n");
                return -1;
            }
            return 1;
        }
        else if (tolower(s[0] == 'h'))
        {
            printf("\nAccepting this option will attempt to reboot the board, forcing it to reload from\n");
            printf("the newly-updated PROM. This capability is only present in some xilinx versions; if\n");
            printf("successful it precludes the necessity of cycling power after a firmware update.\n");
            printf("If you accept this option and it doesn't succeed, simply halt the system and cycle\n");
            printf("power (previously the only way to do it).\n\n");
        }
        else if (tolower(s[0] == 'n'))
            return 0;
    }
}



/*
* get the 5-10 digit serial number
*/
void
edt_ask_sn(char *tag, char *sn, int or_none)
{
    int ok = 0;
    int tries = 0;
    char s[EDT_STRBUF_SIZE];

    while (!ok)
    {
        if (or_none)
            printf("Enter %s serial number (5-10 characters, or 'n' for none) %s%s%s > ", tag,
                *sn?"[":"", sn, *sn?"]":"");
        else

            printf("Enter %s serial number (5-10 characters) %s%s%s > ", tag,
                *sn?"[":"", sn, *sn?"]":"");
        throwaway = fgets(s,127,stdin);
        strip_newline(s);

        if (*sn && !*s)
            return;

        if ((or_none) && (strlen(s) == 1) && (tolower(s[0]) == 'n'))
        {
            sn[0] = '\0';
            ok = 1;
        }

        else if ((strlen(s) < 5)
            || (strlen(s) > 10)
            || (!isalnum_str(s)))
        {
            if (++tries > 2)
            {
                printf("3 tries, giving up\n");
#ifdef VXWORKS
                return;
#else
                exit(1);
#endif
            }
            printf("\nInvalid %s serial number. Expecting 5-10 alphanumeric characters\n\n", tag);
        }
        else
        {
            strcpy(sn, s);
            toupper_str(sn);
            ok = 1;
        }
    }
}




/*
* get the 10 digit option string. allow either XXX-XXXXX or
* XXXXXXXX
*/
void
edt_ask_pn(char *pn)
{
    int i, j;
    int ok = 0;
    int tries = 0;
    char s[EDT_STRBUF_SIZE];
    char tmpstr[11];
    char pn_str[16];

    while (!ok)
    {
        printf("Enter 10-digit part # (with or without dashes) %s%s%s > ",
            *pn?"[":"", edt_fmt_pn(pn, pn_str), *pn?"]":"");
        throwaway = fgets(s,127,stdin);
        strip_newline(s);

        if (*pn && !*s)
            return;

        if (strlen(s) == 12)		/* XXX-XXXXX-XX */
        {
            /* reformat without the dashes */
            ok = 1;
            for (i=0, j=0; i<12; i++)
                if (s[i] != '-')
                    tmpstr[j++] = s[i];
            tmpstr[10] = '\0';
            strcpy(s, tmpstr);
        }
        else if (strlen(s) == 10)	/* XXXXXXXX */
        {
            if (pciload_isdigit_str(s))
                ok = 1;
        }

        if (!ok)
        {
            if (++tries > 2)
            {
                printf("3 tries, giving up\n");
#ifdef VXWORKS
                return;
#else
                exit(1);
#endif
            }
            printf("\nInvalid part number. Format is XXX-XXXXX-XX, all\n");
            printf("numeric (dashes optional)\n\n");
        }
        else
        {
            strcpy(pn, s);
            ok = 1;
        }
    }
}



void
edt_ask_clock(int *clock, char *extra_txt)
{
    int ok = 0;
    int tries = 0;
    int n;
    char s[EDT_STRBUF_SIZE];

    while (!ok)
    {
        printf ("Enter clock speed %s", extra_txt ? extra_txt : "");
        /* printf ("Enter clock speed (usually 10, 20, 30 or 40)"); */
        if (*clock)
            printf(" [%d] > ", *clock);
        else printf(" > ");

        throwaway = fgets(s,127,stdin);
        strip_newline(s);

        if (*clock && !*s)
            return;

        n = atoi(s);

        if (n >= 10 && n <= 120)
        {
            ok = 1;
            *clock = n;
        }
        else
        {
            if (++tries > 2)
            {
                printf("3 tries, giving up\n");
#ifdef VXWORKS
                return;
#else
                exit(1);
#endif
            }
            printf("\nInvalid entry -- must be in the range 10-120\n");
        }
    }
}



void
edt_ask_rev(int *rev)
{
    int ok = 0;
    int tries = 0;
    int n;
    char s[EDT_STRBUF_SIZE];

    while (!ok)
    {
        printf ("Enter rev no.");

        if (*rev)
            printf(" [%02d] > ", *rev);
        else printf(" > ");

        throwaway = fgets(s,127,stdin);
        strip_newline(s);

        if (*rev && !*s)
            return;

        n = atoi(s);

        if (n >= 0 && n <= 999)
        {
            ok = 1;
            *rev = n;
        }
        else
        {
            if (++tries > 2)
            {
                printf("3 tries, giving up\n");
#ifdef VXWORKS
                return;
#else
                exit(1);
#endif
            }
            printf("\nInvalid entry -- must be in the range 0-999\n");
        }
    }
}

int
edt_ask_nmacs(int *count, char *maclist)
{
    int ok = 0, tries = 0, tmp;
    char s[EDT_STRBUF_SIZE];

    while (!ok)
    {
        printf ("Enter number of mac addresses");

        if (*count)
            printf(" [%02d] > ", *count);
        else printf(" > ");

        throwaway = fgets(s,127,stdin);
        strip_newline(s);

        if ((*count >= 0) && (*count <= MAX_MACADDRS) && !*s)
            return *count;

        tmp = atoi(s);

        if (tmp >= 0 && tmp <= MAX_MACADDRS)
        {
            ok = 1;
            *count = tmp;
        }
        else
        {
            if (++tries > 2)
            {
                printf("3 tries, giving up\n");
#ifdef VXWORKS
                return;
#else
                exit(1);
#endif
            }
            else printf("\nInvalid entry -- must be in the range 0-%d\n", MAX_MACADDRS);
        }
    }

    /* write count into maclist */
    sprintf(s, "%02d", *count);
    maclist[0] = s[0];
    maclist[1] = s[1];
    maclist[2] = ',';

    return *count;
}


void
edt_ask_maclist(int count, char *maclist)
{
    int ok = 0;
    int tries = 0;
    int i, j, m;
    char s[EDT_STRBUF_SIZE];
    char tmpstr[EDT_STRBUF_SIZE];
    char macaddr[EDT_STRBUF_SIZE];
    char *mp = &maclist[3];

    for (m=0; m<count; m++) 
    {
        tries = 0;
        ok = 0;
        {
            printf ("Enter mac address %d/%d", m+1, count, m);
            if (sscanf(mp, "%12s,", macaddr) == 1)
                printf(" [%s] > ", macaddr);
            else printf(" > ");

            throwaway = fgets(s,127,stdin);
            strip_newline(s);

            if (*macaddr && !(*s))
                goto edt_ask_maclist_next;

            if (strlen(s) == 17)		/* XX:XX:XX:XX:XX:XX */
            {
                /* reformat without the colons */
                ok = 1;
                for (i=0, j=0; i<17; i++)
                    if (s[i] != ':')
                        tmpstr[j++] = s[i];
                tmpstr[12] = '\0';
                strcpy(s, tmpstr);
            }

            if ((strlen(s) == 12) && pciload_isxdigit_str(s))
            {
                memcpy(mp, s, 12);
                mp[12] = ',';
                ok = 1;
            }
            else if (++tries > 2)
            {
                printf("3 tries, giving up\n");
    #ifdef VXWORKS
                return;
    #else
                exit(1);
    #endif
            }
            else printf("\nInvalid entry. Format is XX:XX:XX:XX:XX:XX, all hex (colons optional)\n");
        }

edt_ask_maclist_next:
        mp += 13;
        ok = 1;
    }
}

/*
* get the 10 digit option string
*/
void
edt_ask_options(char *options)
{
    int ok = 0;
    int tries = 0;
    char s[EDT_STRBUF_SIZE];

    while (!ok)
    {
        printf("Enter options, if any (0-10 characters) %s%s%s > ",
            *options?"[":"", options, *options?"]":"");
        throwaway = fgets(s,127,stdin);
        strip_newline(s);

        if (*options && !*s)
            return;

        if (strlen(s) > 10 || strchr(s, ':')) /* no colons allowed */
        {
            if (++tries > 2)
            {
                printf("3 tries, giving up\n");
#ifdef VXWORKS
                return;
#else
                exit(1);
#endif
            }
            printf("\nInvalid option string -- must be 0-10 alphanumeric characters\n\n");
        }
        else
        {
            strcpy(options, s);
            ok = 1;
        }
    }
}



int
edt_ask_addinfo()
{
    char s[EDT_STRBUF_SIZE];

    printf("\n");
    printf("This board is either being programmed for the first time, or has a\n");
    printf("corrupted or erased FPROM. Entering the embedded info (clock speed,\n");
    printf("board part number, serial number, and options) is reccomended at\n");
    printf("this time. It is not required however, so if you don't have this\n");
    printf("information, you can skip this step by entering 'n'\n");

    *s = '\0';

    while (1)
    {
        char response;
        printf("\nDo you wish to enter the embedded information? [y/n/q] > ");
        throwaway = fgets(s,127,stdin);
        response = tolower(*s);

        if (response == 'y')
            return 1;
        else if (response == 'n')
            return 0;
        else if (response == 'q')
        {
            printf("exiting\n");
            exit(0);
        }
        printf("\nInvalid entry\n");
    }
}
