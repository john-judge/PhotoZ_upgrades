/* #pragma ident "@(#)pciload_main.c	1.118 06/13/07 EDT" */

/*
 * pciload_main.c
 *
 * main module for the EDT pciload program, the PCI Interface
 * FPGA code loader for EDT boards
 *
 * Copyright (C) 1997-2002 EDT, Inc.
 */

#include "edtinc.h"

#include "pciload.h"

#include <stdlib.h>
#include <ctype.h>

extern volatile char *throwaway;

int read_info_file(u_int devid, char *esn, int esn_size);
int save_info_file(u_int devid, char *info, int vb); 
void increment_sn(char *sn);
void print_maclist(char *label, char *list);
char *format_number_string(char *fmt, char *src, char *dest);
char *merge_esns(char *new_esn, char *esn, char *new_sn, char *new_opts, char *new_pn, char *new_rev, char *new_clock);
char *make_esn_string(char *str, Edt_embinfo *ei);
void copy_maclist(char *dest, char *src);
u_short edt_print_prom_summary(EdtDev *edt_p, int sect);

void
edt_read_prom_data(EdtDev *edt_p, int promcode, int segment, 
        EdtPromData *pdata);


int
check_ask_info(EdtDev *edt_p, 
        int promcode, 
        int sect,
        EdtPromData *pdata,
        int vfonly, 
        int proginfo, 
        int nofs);


#ifdef NO_FS
#include "nofs_flash.h"
#else
#define  MAPFLASHARRAY(name,ar, size) (ar = NULL, size = 0)
#endif

typedef struct {
    char fname[MAX_PATH];
    int sector;
} EdtLoadNames;


/* DEBUG */ int Option_hack = 0;

/* ALERT: return should only be called from main, otherwise this could break things.... */
#ifdef NO_MAIN
#define return(x) return(x)
#endif

    char   *
fix_fname(char *fname, 
        char *fname_ret, 
        int promcode, 
        int is_5_volt,
        char *flash_dir,
        int nofs)
{
    int     i;
    char    buf[EDT_PATHBUF_SIZE];
    char    buf2[EDT_PATHBUF_SIZE];
    char    extbuf[EDT_STRBUF_SIZE];
    char   *promdir;
    char   *end;
    char   *ext = 0;
    int     haspath = 0;
    FILE   *fp;
    Edt_prominfo *pi = edt_get_prominfo(promcode);

    if (strcmp(fname, "ERASE") == 0)
    {
        strcpy(fname_ret, fname);
        return fname_ret;
    }

    strcpy(buf, fname);
    promdir = pi->fpga;

    for (i = 0; buf[i] != 0; i++)
    {
        if (buf[i] == DIRECTORY_CHAR)
            haspath = 1;
        else if (buf[i] == '.' && i != 0)
            ext = &buf[i];
    }
    end = &buf[i];

    if (ext)
    {
        strcpy(extbuf, ext);
        *ext = 0;
        end = ext;
    }
    else
    {
        strcpy(extbuf, ".bit");
    }

    if (is_5_volt != -1)
    {
        if (strcmp(end - 3, "_3v") != 0 &&
                strcmp(end - 3, "_5v") != 0)
        {
            end[0] = '_';
            end[1] = (is_5_volt == 1) ? '5' : '3';
            end[2] = 'v';
            end[3] = 0;
        }
        else
        {
            if ((end[-2] == '3' && is_5_volt == 1) ||
                    (end[-2] == '5' && is_5_volt == 0))
            {
                printf("file contains a '_%cv' extension, while trying to program %svolt sector.\n",
                        end[-2], is_5_volt ? "5" : "3.3");
                return NULL;
            }
        }
        /* printf("after voltage add, fname = '%s%s'\n", buf, extbuf); */
    }

    sprintf(buf2, "%s%s", buf, extbuf);

    if (nofs)
        sprintf(fname_ret, "%s/%s%s", promdir, buf, extbuf);
    else
    {
        if ((fp = fopen(buf2, "r")) == NULL)
        {

            if (!haspath)
            {
                sprintf(buf2, "%s%c%s%c%s%s", flash_dir, DIRECTORY_CHAR, promdir, DIRECTORY_CHAR, buf, extbuf);
                if ((fp = fopen(buf2, "r")) == NULL)
                {

                    sprintf(buf2, "%s%c%s%s", flash_dir, DIRECTORY_CHAR, buf, extbuf);
                    if ((fp = fopen(buf2, "r")) == NULL)
                    {

                        printf("Couldn't find the file in any of these locations:\n");
                        printf("%s%s\n", buf, extbuf);
                        printf("%s%c%s%c%s%s\n", flash_dir, DIRECTORY_CHAR, promdir, DIRECTORY_CHAR, buf, extbuf);
                        printf("%s%c%s%s\n", flash_dir, DIRECTORY_CHAR, buf, extbuf);
                        return NULL;
                    }
                }
            }
            else
            {
                perror(buf2);
                return NULL;
            }
        }
        fclose(fp);

        strcpy(fname_ret, buf2);
    }

    edt_msg(EDT_MSG_INFO_1, "Using bitfile %s\n", fname_ret);
    return fname_ret;
}

/* load bitfiles and program */

int get_names_to_load(int promcode, 
        char *fname, 
        char *flashdir,
        EdtLoadNames *names_to_load, 
        int sect,
        int nofs)

{
    int sector;
    Edt_prominfo *ep = edt_get_prominfo(promcode);
    char *newfname;

    sector = (sect == IS_DEFAULT_SECTOR)? ep->defaultseg : sect;


    if (sect == IS_DEFAULT_SECTOR)
    {
        if (ep->load_seg1 != -1)
        {
            if ((newfname = fix_fname(fname, names_to_load[0].fname, 
                            promcode, 0, flashdir, nofs)) == NULL)
                return (0);
            names_to_load[0].sector = ep->load_seg0;
            if ((newfname = fix_fname(fname, names_to_load[1].fname, 
                            promcode, 1, flashdir, nofs)) == NULL)
                return (0);
            names_to_load[1].sector = ep->load_seg1;
            return 2;

        }
        else
        {
            if ((newfname = fix_fname(fname, names_to_load[0].fname, 
                            promcode, -1, flashdir, nofs)) == NULL)
                return (0);
            names_to_load[0].sector = ep->load_seg0;
            return 1;
        }
    }
    else
    {
        if ((newfname = fix_fname(fname, names_to_load[0].fname, 
                        promcode, -1, flashdir, nofs)) == NULL)
            return (0);

        names_to_load[0].sector = sector;

        return 1;

    }
}



EdtPciLoadVerify get_program_verify(int promcode)

{

    EdtPciLoadVerify pvf = NULL;

    switch (promcode)
    {
        case AMD_4013E:
            return program_verify_4013E;

        case AMD_4013XLA:
            return program_verify_4013XLA;

        case SPI_XC3S1200E:
            return program_verify_SPI;

#if 0 /* shouldn't need any of this, default has all the smarts from EPinfo */
      /* (and should also be fixed in the above cases!!!) */
        case AMD_XC2S200_4M:
        case AMD_XC2S100_8M:
        case AMD_XC2S200_8M:
            return program_verify_XC2S200;

        case AMD_XC2S150:
            return program_verify_XC2S150;

        case AMD_4028XLA:
            return program_verify_4028XLA;

        case AMD_XC5VLX30T:
        case AMD_XC5VLX30T_A:
        case AMD_XC5VLX50T:
        case AMD_XC5VLX70T:
        case AMD_EP2SGX30D:
        case AMD_EP2SGX30D_A:
        case AMD_XC6SLX45:
        case AMD_EP2AGX45D:
#endif
        default:
            return program_verify_default;

    }

    return pvf;
}


/*
 * extract the prom type / name for the array if embedded bitfile
 */


    int
load_bitfile(char *fname, 
        EdtBitfile *bitfile, 
        int promcode, 
        int nofs)
{
    /* this code borrowed from main() */
    Edt_prominfo *ep = edt_get_prominfo(promcode);
    long   offset;
    /*
     * if ERASE don't do anything except copy the filename to the struct
     */
    if (strcmp(fname, "ERASE") != 0)
    {

        printf("  Bitfile:  %s\n", fname);


        /* get the header. opens and closes the file, and returns the
         * offset to the start of data 
         */
        edt_bitfile_init(bitfile);

        if (nofs)
        {
            char *p;
            u_char *ba;
            char tmpname[EDT_STRBUF_SIZE];
            char bname[EDT_STRBUF_SIZE];
            char pname[EDT_STRBUF_SIZE];
            char hname[EDT_STRBUF_SIZE];
            int size;

            strcpy(tmpname, fname);
            if ((p = strrchr(tmpname, '/')) != NULL)
            {
                strcpy(bname, p+1);
                *p = '\0';
                if ((p = strrchr(tmpname, '/')) != NULL)
                    strcpy(pname, p+1);
                else strcpy(pname, tmpname);

                sprintf(hname, "%s_%s", pname, bname);
                if ((p = strrchr(hname, '.')) != NULL)
                    *p = '\0';
            }

            MAPFLASHARRAY(hname, ba, size); /* see autogen header file nofs_flash.h */

            if (ba == NULL)
            {
                fprintf(stderr, "error: no array referenced to %s in nofs_flash.h\n", hname);
                return 1;
            }

            edt_bitfile_load_array(bitfile, ba, size);

            bitfile->filename = strdup(hname);

        }
        else
        {
            if (edt_bitfile_load_file(bitfile, fname) != 0)
            {
                edt_msg_perror(DEBUG2, fname);
                return -1;
            }

        }
        offset = edt_get_bitfile_header(bitfile, &bitfile->hdr);

        if (bitfile->hdr.magic != ep->magic)
        {
            printf("Error -- invalid magic number for this device (%d sb %d)\n", bitfile->hdr.magic, ep->magic);
            return 2;
        }

    }

    printf("  File  id: <%s>\n", bitfile->hdr.promstr);

    return 0;
}


    static int
usage(char *s)
{
    printf("\n");
    printf("pciload: update, verify or query FPGA firmware on EDT PCI/PMC/cPCI boards\n\n");
    if (s && (*s))
        printf("%s\n\n", s);

    printf("usage:\n");
    printf("  pciload [options] [keyword | filename]\n");
    printf("\n");
    printf("  If no arguments are given, pciload finds all EDT boards in the system\n");
    printf("  and prints the prom ID for each.\n");
    printf("\n");
    printf("options:\n");
    printf("  -u specifies the unit number (default 0). alternately you can specify\n");
    printf("     device type and board at the same time, e.g. -u %s1\n", EDT_INTERFACE);
    printf("  -i program new embedded ID info (if loading flash with 'update' or filename')\n");
    printf("  -I program new embedded ID info; get defaults from file if present\n");
    printf("  -iX or -Ix <arg> -- same as -i or -I, but X can be any of:\n");
    printf("      s <arg> sets serial number (4-10 digits)\n");
    printf("      p <arg> sets part number (10 digits)\n");
    printf("      v <arg> sets version number (1-4 digits)\n");
    printf("      c <arg> sets clock speed (1-4 digits)\n");
    printf("      o <arg> sets options (4-10 characters)\n");
    printf("      O <arg> sets OSN (4-10 digits)\n");
    printf("      t <arg> sets option serial number (4-10 digits)\n");
    printf("      m <arg> sets mac address list, format nn,addr,addr1,...addr15 where nn is number of mac addresses\n");
    printf("  -d specifies directory to look for files (default ./flash)\n");
    printf("  -p prints a summary of installed boards\n");
    printf("  -q sets quiet mode\n");
    printf("  -s <0-5> sets the PROM sector number on boards using XLA or Alterra parts\n");
    printf("     If no sector is specified (recommended), pciload will program default sector(s)\n");
    printf("     with the specified file (or the default file if none specified)\n");
    printf("  -v verify on-board firmware against file (will not burn new f/w)\n");
    printf("  -V sets verbose mode\n");
    printf("  -F <0-8> adds various rewrites/sleeps; works around problems with some systems'\n");
    printf("     chipsets. If programming fails, try -F 2, 4 or 3. See also -S\n");
    printf("  -S sets 'slow' mode -- necessary on some computers; try this if the load or\n"); 
    printf("     verify seems to hang or indicates corrupt firmware and -F doesn't fix it\n");
#ifdef NO_FS
    printf("  -r force read FPGA bitfile instead of array: only applicable with NO_FS\n");
    printf("  -e force read from array (default) only applicable with NO_FS\n");
#endif
    printf("  -h this help/usage message ('pciload help' will also work)\n");
    printf("\n");
    printf("  The last argument can be one of the keywords 'update', 'verify', 'help' or\n");
    printf("  a full or partial filename. When updating, avoid using a filename unless\n");
    printf("  you know for sure what file you need; instead use the keyword 'update'.\n");
    printf("\n");
    printf("  If the keyword 'update' is present, pciload will attempt to find an up-\n");
    printf("  dated version of the board's current firmware under the flash/ directory\n");
    printf("  and update the PROM with the contents of that file.\n");
    printf("\n");
    printf("  If the keyword 'verify' is present, pciload will compare the firmware in\n");
    printf("  the PROM to the one in the file (optionally the '-v' flag can be used\n");
    printf("  with a filename to compare to a specific firmware file)\n");
    printf("\n");
    printf("  The keyword 'help' prints out this message (equivalent to '-h').\n");
    printf("\n");
    printf("  If a partial filename is specified (e.g. 'pcd',  or 'pci11w', pciload will\n");
    printf("  search for the appropriate version of the firmware file, and update to\n");
    printf("  that version (or compare if '-v' is specified). Complete/absolute pathnames\n");
    printf("  can also be used for filename (but with caution).\n");

    printf("\n");
    if (s == NULL)
        return 0;
    return 1;
}

/*
 * print the serial numbers
 */
void print_id_info(EdtPromData *pdata)
{
    Edt_embinfo ei;
    char pn_str[16];
    char mac_str[32];
    int nmacs;
    int i, j;

    if ((!pdata->maclist[0]) || (sscanf(pdata->maclist, "%2d,", &nmacs) != 1) || (nmacs < 0) || (nmacs > 16))
        nmacs = 0;

    memset(&ei,0,sizeof(ei)) ;


    if (pdata->esn[0])
    {
        if (edt_parse_devinfo(pdata->esn, &ei) == 0)
        {
            printf("  s/n %s, p/n %s, i/f fpga %s, rev %d, clock %d Mhz", 
                    ei.sn, 
                    edt_fmt_pn(ei.pn, pn_str), 
                    strlen(ei.ifx) > 2?ei.ifx:"(none)", 
                    ei.rev, ei.clock);
            if (*ei.opt)
                printf(", opt %s", ei.opt);

            if (pdata->osn[0])
                printf(", oem s/n %s", pdata->osn);

            if ((*pdata->optsn) || (nmacs))
            {
                printf("\n  option sn %s, %d mac addrs", pdata->optsn[0]?pdata->optsn:"none", nmacs);
                for (i=0, j=3; i<nmacs; i++, j+=13)
                {
                    if (sscanf(&pdata->maclist[j], "%12s,", mac_str) == 1)
                        printf("%s%s", i == 0? ": ":", ", format_number_string("XX:XX:XX:XX:XX:XX", mac_str, mac_str));
                }
            }
            printf("\n");
        }
    }

}

char Promcode_str[64];
    char *
fmt_promcode(int promcode)
{
    Edt_prominfo *pi = edt_get_prominfo(promcode);

    if (promcode < 0 || promcode > edt_get_max_promcode())
        return "Unknown Flash ROM!";
    sprintf(Promcode_str, "%s %s FPGA, %s FPROM", pi->fpga, pi->busdesc, pi->promdesc);
    return Promcode_str;
}

    void
print_segment_info(EdtDev *edt_p, int promcode, int segment,
        EdtPromData *pdata)
{
    edt_read_prom_data(edt_p, promcode, segment, pdata);

    if (pdata->id[0])
        printf(" <%s>", pdata->id);
    printf("\n");

}


/* this should really be table-driven... */
    void
print_prominfo(EdtDev *edt_p, int promcode, u_short stat)
{
    int i;
    EdtPromData pdata;
    u_char jumpers = stat & 0x3;
    u_char idbits = (stat >>2) & 0x1f;
    u_char xidbits = stat >> 8;
    Edt_prominfo *ep = edt_get_prominfo(promcode);

    if (promcode > edt_get_max_promcode())
        printf("  Unknown flash ROM [%02x %02x] - no information available\n", xidbits, idbits);


    /* should all be table driven but first need to make sure all EPinfo values are correct */
    /* (and also this boot / non-boot stuff for earlier devices...? */
    switch (promcode)
    {
        case AMD_4013E:
            printf("  Sector 0");
            print_segment_info(edt_p, promcode, 0, &pdata);
            break;
        case AMD_4013XLA:
            printf("  Sector 1");
            print_segment_info(edt_p, promcode, 1, &pdata);
            printf("  Sector 3");
            print_segment_info(edt_p, promcode, 3, &pdata);
            break;
        case AMD_XC2S200_4M:
            printf("  Sector 0");
            print_segment_info(edt_p, promcode, 0, &pdata);
            printf("  Sector 1");
            print_segment_info(edt_p, promcode, 1, &pdata);
            break;
        case AMD_4028XLA:
        case AMD_XC2S150:
        case AMD_XC2S100_8M:
        case AMD_XC2S200_8M:
            printf("  Sector 0");
            print_segment_info(edt_p, promcode, 0, &pdata);
            printf("  Sector 1");
            print_segment_info(edt_p, promcode, 1, &pdata);
            printf("  Sector 2");
            print_segment_info(edt_p, promcode, 2, &pdata);
            printf("  Sector 3");
            print_segment_info(edt_p, promcode, 3, &pdata);
            break;
        case SPI_XC3S1200E:
            printf("  Sector 0");
            print_segment_info(edt_p, promcode, 0, &pdata);
            printf("  Sector 1");
            print_segment_info(edt_p, promcode, 1, &pdata);
            /* ALERT -- may be more if different bitfiles for 4/8 lane etc. */
            break;

        case AMD_XC5VLX70T:
            printf("  Sector 0");
            print_segment_info(edt_p, promcode, 0, &pdata);
            break;

        default:
            for (i=0; i<(int)ep->nsegments; i++)
            {
                printf("  Sector %d (%s) ", i, (jumpers == i)?"BOOT":(i==0)?"Prot":"User");
                print_segment_info(edt_p, promcode, i, &pdata);
            }
            break;
    }
}


    u_short
edt_print_prom_summary(EdtDev *edt_p, int sect)

{
    int sector;
    int     promcode;
    u_short  stat;
    u_int   frdata;
    Edt_prominfo *ep;
    EdtPromData pdata;

    promcode = edt_flash_prom_detect(edt_p, &stat);
    ep = edt_get_prominfo(promcode);
    sector = (sect == IS_DEFAULT_SECTOR)? ep->defaultseg:sect;
    edt_read_prom_data(edt_p, promcode, sector, &pdata);
    printf("  ID 0x%02x, %s\n", edt_p->devid, fmt_promcode(promcode));
    print_id_info(&pdata);
    print_prominfo(edt_p, promcode, stat);
    frdata = edt_reg_read(edt_p,EDT_FLASHROM_DATA) ;
    edt_print_flashstatus(stat, sector, frdata);
    printf("\n");

    return stat;
}




    int
edt_enumerate_and_summarize(char *dev, int unit, int sect)

{
    int nunits = 0;
    int i;
    EdtDev *edt_p;

    Edt_bdinfo *boards = edt_detect_boards(dev, unit, &nunits, 0);

    if (boards[0].id == -1)
    {
        printf("No board detected\n");
        return (1);
    }

    if (boards[1].id != -1)
        printf("\nMultiple EDT PCI boards detected:\n\n");

    for (i = 0; boards[i].id != -1; i++)
    {
        printf("%s unit %d (%s):\n", 
                boards[i].type, 
                boards[i].id,
                edt_idstring(boards[i].bd_id, boards[i].promcode));

        if (boards[i].bd_id != P53B_ID) 
        {
            edt_p = edt_open(boards[i].type, boards[i].id);
            edt_print_prom_summary(edt_p, sect);
        }
    }

    free(boards);

    return 0;
}

    int
check_load_values(EdtDev *edt_p, 
        int promcode, 
        EdtBitfile *bitfile, 
        EdtPromData *pdata,
        int task,
        int warn,
        int segment)

{
    /* check for ok values */
    int idmatch = 0;
    int quiet = (edt_get_verbosity() == 0)?1:0;

    if (strlen(pdata->id) > MAX_STRING)
    {
        printf("  prom ID size out of range (%d)\n", (int)strlen(pdata->id));
        return 0;
    }

    if (warn || task == VerifyOnly)
        idmatch = check_id_stuff(bitfile->hdr.promstr, pdata->id, edt_p->devid, task, bitfile->filename);

    if (!idmatch)
    {
        if (task == VerifyOnly)
        {
            printf("\n  file ID and prom ID differ; skipping remainder of verify\n\n");
            return 0;
        }
        else printf("  (file ID and prom ID differ)\n");
    }

    if (!(task == VerifyOnly))
    {
        if (warn && !quiet) {
            warnuser(edt_p, bitfile->filename, segment);
        }
    }
    return 1;
}



int     quiet = 0;

#ifdef NO_MAIN
#include "opt_util.h"
char *argument ;
int option ;
pciload(char *command_line)
#else
    int
main(int argc, char *argv[])
#endif
{
    int     i, ret = 0;
    int     sector;
    int     nofs = 0;
    int     ver = 0, vb = 0, vfonly = 0;
    int     summary_only = 0;
    char    fname[MAX_STRING+1];
    int     promcode;
    int     saveinfo = 0;
    u_short  stat;
    EdtDev *edt_p = NULL;
    Edt_prominfo *ep;
    char    errbuf[EDT_STRBUF_SIZE];
    char   *unitstr = "";

    char   *argv0;
    char   *iarg;
    char    dev[EDT_STRBUF_SIZE];
    char    flash_dir[EDT_PATHBUF_SIZE];
    int     force_fast = 0;
    int     unit = -1;
    int     sect;
    int     proginfo = 0;
    Edt_embinfo embinfo;

    EdtPromData existing;
    EdtPromData modified;

    PciloadState task;
    EdtBitfile    bitfile;
    char new_sn[16];
    char new_pn[16];
    char new_rev[8];
    char new_clock[8];
    char new_opts[16];
    char new_esn[EDT_STRBUF_SIZE];
    char new_osn[EDT_STRBUF_SIZE];
    char new_optsn[OPTSN_SIZE];
    char new_maclist[MACLIST_SIZE];

    int clear_extra = 0;

#ifdef NO_MAIN
    char **argv  = 0 ;
    int argc = 0 ;
    opt_create_argv("pciload",command_line,&argc,&argv);
#endif

#ifdef NO_FS
    nofs = 1;
#endif

    errbuf[0] = 0;

    flash_dir[0] = '\0';
    sect = IS_DEFAULT_SECTOR;
    argv0 = argv[0];
    fname[0] = '\0';
    dev[0] = '\0';

    new_esn[0] = new_osn[0] = new_sn[0] = new_pn[0] = new_opts[0] = new_rev[0] = new_clock[0] = new_optsn[0] = new_maclist[0] = '\0';

#ifdef OLD
    esn[0] = osn[0] = '\0';
#endif

    memset(&embinfo,0,sizeof(embinfo)) ;
    edt_bitfile_init(&bitfile);

    /*
     * COMMAND LINE ARGUMENTS
     */

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            if (i != argc - 1)
                usage("the fname must be the last argument");
            strcpy(fname, argv[i]);
        }
        else
        {
            if (argv[i][1] == '-')
            {
                if (strcmp(argv[i], "--help") == 0)
                {
                    usage(NULL);
                    return(0);
                }
                else
                {
                    sprintf(errbuf, "unknown option \"%s\"", argv[i]);
                    usage(errbuf);
                    return(1);
                }
            }
            if ((argv[i][1] == 0)
                    && (strcmp(&argv[i][1], "Ossp") != 0)
                    && (strcmp(&argv[i][1], "Odrs") != 0))
            {
                usage(NULL);
                return(1);
            }

            switch (argv[i][1])
            {
                case 'd':
                    if (++i == argc)
                        usage("unfinished -d option");
                    strcpy(flash_dir, argv[i]);
                    break;
                case 'u':
                    if (++i == argc)
                        usage("unfinished -u option");
                    unitstr = argv[i];
                    break;
                case 's':
                    if (++i == argc)
                        usage("unfinished -s option");
                    sect = atoi(argv[i]);
                    break;
                case 'e':
#ifdef NO_FS
                    nofs = 1;
#else
                    sprintf(errbuf, "-e specified but not compiled with embedded bitfiles.\nrecompile with -DNO_FS and try again\n");
                    usage(errbuf);
                    ret = 1;
#endif
                    break;
                case 'v':
                    vfonly = vfonly ? 0 : 1;
                    break;
                case 'r':
#ifdef NO_FS
                    nofs = 0;
#else
                    sprintf(errbuf, "-r specified but not compiled with embedded bitfiles.\nrecompile with -DNO_FS and try again\n");
                    usage(errbuf);
                    ret = 1;
#endif
                    break;
                case 'V': /* verbose */
                    vb = vb ? 0 : 1;
                    break;

                case 'I':
                case 'i':
                    if (strlen(iarg = argv[i]) > 2)
                    {
                        if (++i == argc)
                        {
                            printf("unfinished %s option. takes one argument\n", iarg);
                            ret = 1;
                        }
                        else switch(iarg[2])
                        {
                            case 's': strncpy(new_sn, argv[i], 11); 
                                      break;
                            case 'p': strncpy(new_pn, argv[i], 11); 
                                      break;
                            case 'v': strncpy(new_rev, argv[i], 5); 
                                      break;
                            case 'c': strncpy(new_clock, argv[i], 5); 
                                      break;
                            case 'o': strncpy(new_opts, argv[i], 11); 
                                      break;
                            case 'O': strncpy(new_osn, argv[i], 11); 
                                      break;
                            case 'n': strncpy(new_optsn, argv[i], 11); 
                                      break;
                            case 'm': strncpy(new_maclist, argv[i], MACLIST_SIZE); 
                                      break;
                            default:
                                      usage("invalid -Ix  or -ix option\n");
                        }
                    }
                    else if (iarg[0] == 'I')
                        proginfo = 2;
                    else proginfo = 1;
                    break;
                case 'p':		/* print a summary of installed boards */
                    summary_only=1;
                    break;
                case 'q':
                    quiet=1;
                    break;
                case 'f':
                    force_fast=1;
                    edt_flash_set_do_fast(1) ;
                    break;

                    /* DEBUG add delays, rewrites or rereads to debug fast mode */
                case 'F':
                    if (++i == argc)
                    {
                        printf("unfinished -F option. takes 1 int arg, set bits as follows:\n");
                        printf("0 (0x1): add extra writes in tst_write\n");
                        printf("1 (0x2): add extra dummy reads in tst_write\n");
                        printf("2 (0x4): add extra reads in tst_read\n"); 
                        printf("3 (0x8): unassigned\n");
                        printf("4 (0x10): msleep(1) before every tst_write\n");
                        printf("5 (0x20): msleep(1) before every tst_read\n");
                        ret = 1;
                    }
                    edt_flash_set_debug_fast(strtol(argv[i], NULL, 16));
                    break;

#ifdef PSN
#include "pciload_sn.c"
#endif

                case 'S':
                    edt_flash_set_force_slow(1) ;
                    edt_flash_set_do_fast(0) ;
                    break;

                case 'H':
                    Option_hack= 1;
                    break;

                case 'h':
                    usage(errbuf);
                    break;

                case 'X':
                    clear_extra = 1;
                    break;

                default:
                    sprintf(errbuf, "\nunknown option \"-%c\"", argv[i][1]);
                    usage(errbuf);
            }
        }
    }

    if (ret)
        return (ret);


    /* check for valid values */
    if (sect != IS_DEFAULT_SECTOR &&
            (sect < 0 || sect > 3))
    {
        printf("Sector number %d should be between 0 and 3\n", sect);
        return(1);
    }

    /* Determine task */

    if (strcmp(fname, "update") == 0)
    {
        task = AutoUpdate;
    } 
    else if (strcmp(fname, "erase") == 0)
    {
        task = Erase;
    }
    else if (vfonly || (strcmp(fname, "verify") == 0))
    {
        task = VerifyOnly;
        vfonly = 1;
    }
    else if (fname[0])
    {
        task = LoadProm;
    }
    else if (strcmp(fname, "help") == 0)
    {
        usage(errbuf);
        return(0);
    }
    else
        task = Enumerate;


    /* create critical defaults */
    if (task == Enumerate)
    {
        edt_enumerate_and_summarize(dev, unit, sect);

        return 0;
    }

    if (dev[0] == 0)
        strcpy(dev, EDT_INTERFACE); 

    if (*unitstr)
        unit = edt_parse_unit(unitstr, dev, NULL);
    else
        unit = 0;

    /* DO SOMETHING FOR ONLY ONE BOARD */
    /* if no name, and task == update or verify */

    if ((task == VerifyOnly || task == AutoUpdate) || (!fname[0]))
    {
        if (unit == -1)
            unit = 0 ;

        if (dev[0] == 0)
            strcpy(dev, EDT_INTERFACE); 

        if ((edt_p = edt_open(dev, unit)) == NULL)
        {
            char str[EDT_STRBUF_SIZE];
            sprintf(str, "%s unit %d", EDT_INTERFACE, unit) ;
            edt_perror(str) ;
            return (1) ;
        }

        if (edt_flash_get_fname(edt_p, fname) != 0)
        {
            edt_msg(EDT_MSG_FATAL,"Unable to \n");
            return (1) ;

        }

        edt_close(edt_p);
    }

    if ((edt_p = edt_open(dev, unit)) == NULL)
    {
        char    str[64];

        sprintf(str, "edt_open(%s%d)", dev, unit);
        edt_perror(str);
        return(2);
    }

    promcode = edt_flash_prom_detect(edt_p, &stat);
    printf("\n%s unit %d (%s):\n", dev, unit, edt_idstring(edt_p->devid, promcode));

    /* get & print board info */
    ep = edt_get_prominfo(promcode);
    sector = (sect == IS_DEFAULT_SECTOR)? ep->defaultseg:sect;
    edt_read_prom_data(edt_p, promcode, sector, &existing);

    if (clear_extra)
    {
        existing.extra_size = 0;
        memset(existing.extra_buf,0, sizeof(existing.extra_buf));
    }

    edt_print_prom_summary(edt_p, sect);

    /* older devices can't program with jumper in protected position */
    if ((task != VerifyOnly) && ((stat & EDT_ROM_JUMPER) == 0) && (promcode < AMD_XC2S150))
    {
        printf("Protected ROM jumper is in the protected position pin 1-2\n");
        printf("Verify only allowed\n");
        printf("To program, remove jumper and run pciload again\n");
        task = VerifyOnly;
    }


    modified = existing;

    if (merge_esns(new_esn, modified.esn, new_sn, new_opts, new_pn, new_rev, new_clock) != NULL)
        strcpy(modified.esn, new_esn);
    if (new_osn[0])
        strcpy(modified.osn, new_osn);
    if (new_optsn[0])
        strcpy(modified.optsn, new_optsn);
    if (new_maclist[0])
        copy_maclist(modified.maclist, new_maclist);

    /*
     * things to take care of in the next switch statement:
     * 
     * 1) if no fname, just get info.  otherwise, 2) program the sectors as
     * needed--- fix_fname's middle arg is 0 for 3.3volt file, 1 for 5volt
     * file, and -1 for non-XLA boards(no voltage) otherwise,
     * program_verify_PROMTYPE()
     * 
     * things like making sure values are within range should be taken care of
     * inside the getinfo and program_verify functions.
     */

    if (fname[0])
    {
        EdtLoadNames names_to_load[2];
        EdtPciLoadVerify pvf;


        int n_names = 0;

        /* dflt to fast with this xilinx */
        if (edt_flash_get_force_slow()) 
            edt_flash_set_do_fast(0) ;
        else 
            edt_flash_set_do_fast(1) ;

        /* get list of file names and sectors to load */
        if (!flash_dir[0])
            sprintf(flash_dir, "%s%cflash", edt_home_dir(edt_p), DIRECTORY_CHAR);

        n_names = get_names_to_load(promcode, fname, flash_dir, names_to_load, sect, nofs);

        pvf = get_program_verify(promcode);

        /* ask for serial # if needed */

        sector = (sect == IS_DEFAULT_SECTOR)? ep->defaultseg:sect;

        if (task == LoadProm)
        {
            if ((saveinfo = check_ask_info(edt_p,
                            promcode,
                            sector,
                            &modified, 
                            0,
                            proginfo,
                            nofs)) < 0)
                return -1;
        }

        for (i=0;i<n_names;i++)
        {
            if ((ret = load_bitfile(names_to_load[i].fname, &bitfile, promcode, nofs)) != 0)
                return(ret);     


            /* make sure it makes sense */
            if (task == VerifyOnly)
                edt_read_prom_data(edt_p, promcode, names_to_load[i].sector, &modified);


            if (check_load_values(edt_p, 
                        promcode, 
                        &bitfile, 
                        &modified, 
                        task,
                        (i== 0),
                        names_to_load[i].sector))
            {
                if (task != VerifyOnly)
                    strncpy(modified.id, bitfile.hdr.promstr, sizeof(modified.id)-1);

                ver += pvf(edt_p, &bitfile,  &modified, promcode, 
                        names_to_load[i].sector,
                        (task == VerifyOnly), (i==0), vb);
            }

        }

    }

    if (nofs)
        saveinfo = 0;

    /* save the serial # info to a file if changed and programmed */
    if ((!vfonly) && (ver == 0) && (saveinfo))
    {
        save_info_file(edt_p->devid, modified.esn, vb);
    }

    edt_close(edt_p);

    if (*fname && !vfonly)
    {
        if (ver)
        {
            printf("One or more sectors failed verification. Errors may be due to slow host\n");
            printf("writes/reads. Try -F <level> or -S (pciload --help to see all options)\n");
        }
        else if (strcmp(fname, "ERASE") == 0)
        {
            printf("This board will be non-functional after the next power-down, and will remain \n");
            printf("so until the appropriate EDT PCI FPGA firmware is loaded back into the\n");
            printf("Board's EEPROM.\n\n");
            printf("To reload the firmware, power the system down, move the protected mode jumper\n");
            printf("on the board to the \"protected sector\" positon (pins 1-2). Power the system\n");
            printf("up again, move the jumper back to the non-protected position (pins 2-3),\n");
            printf("then load the appropriate firmware using the pciload program.\n");
        }
        else printf("The firmware update will take effect after the next time you cycle power.\n");
    }
    return(0);
}

/*
 * copy a maclist would use strncpy but do this instead to ensure format; i.e.
 * all unused entries are 000000000000
 */
    void
copy_maclist(char *dest, char *src)
{
    int i, nmacs;
    char *sp = &src[2];
    char mac[MACADDR_SIZE];

    if (sscanf(src, "%02d,", &nmacs) != 1)
        nmacs = 0;

    sprintf(dest, "%02d", nmacs);

    for (i=0; i<16; i++, sp+=13)
    {
        if ((i < nmacs) && (sscanf(sp, ",%12s", mac) == 1))
        {
            strcat(dest, ",");
            strcat(dest, mac);
        }
        else strcat(dest, ",000000000000");
    }
    dest[MACLIST_SIZE-2] = dest[MACLIST_SIZE-1] = '\0';
}

    char *
merge_esns(char *new_esn, char *esn, char *new_sn, char *new_opts, char *new_pn, char *new_rev, char *new_clock)
{
    Edt_embinfo ei;

    edt_zero(new_esn, ESN_SIZE);
    edt_parse_devinfo(esn, &ei);

    if (!(*new_sn || *new_pn || *new_rev || *new_clock || *new_opts))
        return NULL;

    if (*new_sn)
        strncpy(ei.sn, new_sn, 11);
    if (*new_pn)
        strncpy(ei.pn, new_pn, 11);
    if (*new_opts)
        strncpy(ei.opt, new_opts, 15);
    if (*new_rev)
        ei.rev = atoi(new_rev);
    if (*new_clock)
        ei.clock = atoi(new_clock);
    make_esn_string(new_esn, &ei);
    return new_esn;
}

/*
 * go from edt info struct to ':' formatted string.
 * return the string
 */
    char *
make_esn_string(char *str, Edt_embinfo *ei)
{
    sprintf(str, "%s:%s:%d:%s:%d:%s:", ei->sn, ei->pn, ei->clock, ei->opt, ei->rev, ei->ifx);
    return str;
}



    int
match_ifx_dir(char *base, char *name)
{
    DIRHANDLE  dirp = (HANDLE) 0;
    char    d_name[EDT_PATHBUF_SIZE];
    int     ret = 0;

    if ((dirp = edt_opendir(base)) == (HANDLE)0)
        return 0;

    while (edt_readdir(dirp, d_name))
    {
        if (strcasecmp(name, d_name) == 0)
        {
            ret = 1;
            break;
        }
    }
    edt_closedir(dirp);
    return ret;
}

/*
 * check IF xilinx name against dir names and return 1 if match, 0 if not
 * ADD new dv devices as they come along
 */
    int
valid_ifxname(int devid, char *name)
{
    if (ID_HAS_COMBINED_FPGA(devid))
        return 0;

    if (   match_ifx_dir("./camera_config/bitfiles/dv", name)
            || match_ifx_dir("./camera_config/bitfiles/dva", name)
            || match_ifx_dir("./camera_config/bitfiles/dvaero", name)
            || match_ifx_dir("./camera_config/bitfiles/dvfox", name)
            || match_ifx_dir("./camera_config/bitfiles/dvk", name))
        return 1;

    if (match_ifx_dir("./bitfiles", name))
        return 1;
    return 0;
}


    int
edt_ask_ifxilinx(int devid, char *pn, int rev, char *ifx)
{
    int ok = 0;
    int tries = 0;
    char resp[EDT_STRBUF_SIZE];
    char tmppn[32];

    /* if xxx-xxxxx-00, replace last 2 digits with rev */
    if ((strcmp(&(pn[8]), "00") == 0))
        sprintf(&(tmppn[8]), "%02d", rev);
    else strcpy(tmppn, pn);

    if (ID_HAS_COMBINED_FPGA(devid))
    {
        strcpy(ifx, "none");
        return 0;
    }

    if ((strlen(ifx) < 2) && (strlen(tmppn) > 7))
        edt_find_xpn(tmppn, ifx);

    while (!ok)
    {
        printf("Enter interface Xilinx FPGA, (0-10 characters) %s%s%s > ",
                *ifx?"[":"", ifx, *ifx?"]":"");
        throwaway = fgets(resp,127,stdin);
        strip_newline(resp);

        if (*ifx && !*resp)
            return 0;

        if (strlen(resp) > 10 || strchr(resp, ':')) /* no colons allowed */
        {
            if (++tries > 2)
            {
                printf("3 tries, giving up\n");
                return 1;
            }
            printf("\nInvalid I/F Xilinx string -- must be 0-10 alphanumeric characters\n\n");
        }
        else
        {
            strcpy(ifx, resp);
            ok = 1;
        }
    }
    /* printf("\n"); */
    return 0;
}

/*
 * format a string per the fmt argument, place the result in
 * the dest string and return a pointer to that. format string
 * should be upper or lower-case Xs -- anything else is treated
 * as a format character that will be inserted into the dest string.
 * For example xx-xxxxx-xx to add dashes in the dest string. May
 * use dest as src (overwriting the unformatted with the formatted
 * string); in that case src/dest string must be at least as long
 * as the fmt string.
 */
    char *
format_number_string(char *fmt, char *src, char *dest)
{
    u_int i, j=0;
    char *tmpstr = (char *)edt_alloc((int)strlen(fmt)+1);

    for (i=0; i<strlen(fmt) && j<strlen(src); i++)
    {
        if (tolower(fmt[i]) == 'x')
            tmpstr[i] = src[j++];
        else tmpstr[i] = fmt[i];
    }
    tmpstr[i] = '\0';
    strcpy(dest, tmpstr);
    edt_free(tmpstr);
    return dest;
}


/*
 * prompt for info to put into the serial number, etc. part of
 * the xilinx PROM
 * return 0 on success, 1 on failure or quit
 */
    int
ask_info(int devid, Edt_embinfo *si)
{
    char resp[EDT_STRBUF_SIZE];
    int ok = 0, n, ret, nmacs = 0;

    while (!ok)
    {   
        printf("\n");
        edt_ask_sn("board", si->sn, 0);

        edt_ask_pn(si->pn);
        edt_ask_rev(&(si->rev));
        if ((ret = edt_ask_ifxilinx(devid, si->pn, si->rev, si->ifx)) != 0)
            return ret;
        edt_ask_clock(&(si->clock), "(usually 10, 20, 30, or 40)");
        edt_ask_options(si->opt);

        edt_ask_sn("option", si->optsn, 1);
        if (si->maclist[0])
            if ((n = sscanf(si->maclist, "%02d,", &nmacs)) != 1)
                nmacs = 0;

        if ((n = edt_ask_nmacs(&nmacs, si->maclist)) != 0)
            edt_ask_maclist(nmacs, si->maclist);

        printf("\n\n");
        edt_print_info(devid, si);
        printf("\n");

        *resp = '\0';
        while (tolower(*resp) != 'y' && (tolower(*resp) != 'n'))
        {
            printf("\nOkay? (y/n/q) [y]> ");
            throwaway = fgets(resp,127,stdin);

            if ((*resp == '\n') || (*resp == '\r') || (*resp == '\0'))
                *resp = 'y'; /* default to 'yes' */

            if (*resp == 'y')
                ok = 1;
            else if (*resp == 'q')
            {
                printf("exiting\n");
                return 1;
            }
            else if (*resp != 'n')
                printf("\nEnter 'y' if the above information is correct, 'n' to change, q to quit\n");
        }
    }
    return 0;
}

    void
edt_print_info(int devid, Edt_embinfo *si)
{
    char pn_fmt[32];
    int clock_alert = 0;
    int nmacs;

    if (si->clock != 10 && si->clock != 20
            && si->clock != 30 && si->clock != 40)
        clock_alert = 1;

    if ((sscanf(si->maclist, "%02d,", &nmacs) != 1) || (nmacs < 0) || (nmacs > MAX_MACADDRS))
        nmacs = 0;

    printf("Serial no:        %s\n", si->sn);
    printf("Part number:      %s\n", format_number_string("XXX-XXXXX-XX", si->pn, pn_fmt));
    printf("I/F FPGA:         %s %s", si->ifx, valid_ifxname(devid, si->ifx)?"\n":" (WARNING: no matching FPGA dir found)\n");
    printf("Rev:              %02d\n", si->rev);
    printf("Clock:            %d Mhz\n", si->clock);
    printf("Options:          %s\n", si->opt[0]?si->opt:"<none>");
    printf("Option serial no: %s\n", si->optsn);
    printf("MAC addresses:    ");
    if (nmacs)
    {
        int i, idx = 3;

        for (i=0; i<nmacs; i++)
        {
            char mac_only[16], mac_fmt[24];

            strncpy(mac_only, &si->maclist[idx], 12);
            mac_only[12] = '\0';
            printf("%s%s ", format_number_string("XX:XX:XX:XX:XX:XX", mac_only, mac_fmt), i < nmacs-1?",":"");
            idx +=13;
        }
    }
    else printf("none\n");
}

    void
pad_sn(char *sn)
{
    char *p;
    char tmpsn[EDT_STRBUF_SIZE];
    int num;

    strcpy(tmpsn, sn);

    p = &tmpsn[strlen(sn)];

    while (p > tmpsn && (*(p-1) >= '0') && (*(p-1) <= '9'))
        --p;

    num = atoi(p);
    *p = '\0';
    if (Option_hack)
        sprintf(sn, "PDVF%04d", num);
    else sprintf(sn, "%s%04d", tmpsn, num);
}

/*
 * check whether to ask for the serial number and do so if indicated
 * return nonzero if saving of new info is indicated, 0 if not, -1 on error
 */
    int
check_ask_info(EdtDev *edt_p, 
        int promcode, 
        int sect,
        EdtPromData *pdata,
        int vfonly, 
        int proginfo, 
        int nofs)
{
    /* proginfo > 1 means use info in file for
     * defaults instead of whats already on the board
     */
    Edt_embinfo ei;

    edt_parse_devinfo(pdata->esn, &ei);
    strcpy(ei.optsn, pdata->optsn);
    strcpy(ei.maclist, pdata->maclist);

    if ((!nofs) && (((proginfo) > 1) || (!*pdata->esn)))
    {
        char str[ESN_SIZE+2];

        if (read_info_file(edt_p->devid, str, ESN_SIZE+2) == 0)
        {
            edt_parse_devinfo(str, &ei);
            if (*ei.sn)
                increment_sn(ei.sn);
        }
    }

    if (!vfonly)
    {
        if (!proginfo && (!*pdata->esn) && (!*pdata->id))
            proginfo = edt_ask_addinfo();

        if (proginfo)
        {
            if (ask_info(edt_p->devid, &ei) != 0)
                return -1;
            edt_zero(pdata->esn, ESN_SIZE);
            make_esn_string(pdata->esn, &ei);
            edt_zero(pdata->optsn, OPTSN_SIZE);
            strcpy(pdata->optsn, ei.optsn);
            edt_zero(pdata->maclist, MACLIST_SIZE);
            strcpy(pdata->maclist, ei.maclist);
        }
        else if (Option_hack)
        {
            strcpy(ei.opt, "lvds");
            pad_sn(ei.sn);
            make_esn_string(pdata->esn, &ei);
        }
    }
    return proginfo;
}


    int
save_info_file(u_int devid, char *info, int vb)
{
    FILE *fp;
    char fname[32];

    sprintf(fname, "pciload_%02x.esn",  devid);

    if (edt_access(fname, 2) == 0)
    {
        /* if (vb) printf("overwriting existing device info file '%s'\n", fname); */
    }
    else if ((edt_access(fname, 0) == 0) || (edt_access(".", 2) != 0))
    {
        if (vb) printf("can't write or create device info file '%s'\n", fname);
        return -1;
    }

    if ((fp = fopen(fname, "w")) == NULL )
    {
        if (vb) edt_perror(fname);
        return -1;
    }

    fprintf(fp, "%s", info);
    fclose(fp);
    return 0;
}

/*
 * return length of string, or 0 if too big or small 
 */
int read_info_file(u_int devid, char *esn, int size)
{
    FILE *fp;
    int  ret = 0;
    size_t n;
    char fname[32];

    sprintf(fname, "pciload_%02x.esn",  devid);

    if ((fp = fopen(fname, "r")) == NULL )
    {
        /* edt_perror(str) ; */
        return -1;
    }

    /* read the ESN string */
    if ((!fgets(esn, size, fp)) || ((n = strlen(esn)) >= (size_t)size-1) || (n < 16)) /* arbitrary min. */
    {
        ret = -1;
        esn[0] = '\0';
    }

    fclose(fp);
    return ret;
}

    void
increment_sn(char *sn)
{
    char *p;
    char tmpsn[EDT_STRBUF_SIZE];
    int num;

    strcpy(tmpsn, sn);
    p = &tmpsn[strlen(sn)];

    while (p > tmpsn && (*(p-1) >= '0') && (*(p-1) <= '9'))
        --p;

    num = atoi(p);
    *p = '\0';
    sprintf(sn, "%s%04d", tmpsn, ++num);
}

