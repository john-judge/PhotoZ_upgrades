/*
 * dvinfo
 * 
 * C program that runs various EDT PCI DV applications and sends the
 * results to a file for use by EDT engineers in diagnosing the problem.
 *
 * (C) 1997-2009 Engineering Design Team, Inc.
 */
#include "edtinc.h"

#ifdef _NT_
#define TAKE "take.exe"
#define INITCAM "initcam.exe"
#define CAT "type"
#else
#define TAKE "take"
#define INITCAM "initcam"
#define CAT "cat"
#endif

#define OUTFILE "dvinfo.out"
#define TMPFILE "dvinfo.tmp"

#define CL_PCLOCK_RUNNING	0
#define CL_PCLOCK_NOTRUNNING	1
#define CL_PCLOCK_NA		2

static void usage(char *progname);
static char   *grepit(char *buf, char *pat);

int get_cfgfname(char *setupfile, char *cfgfname);
int get_bdid(int unit);
void invoke(char *cmd, char *fname);
void echoit(char *str, char *fname);
int is_simulator(int unit);
void do_simulator(int unit, int channel);
void do_framegrabber(int unit, int channel);
void do_systemstuff();

/*
 * main
 */
main(int argc, char **argv)
{
    int     unit = 0, channel = 0;
    char   *progname = argv[0];
    char    cmdstr[128];

    /*
     * process command line arguments
     */
    --argc;
    ++argv;
    while (argc && ((argv[0][0] == '-') || (argv[0][0] == '/')))
    {
        switch (argv[0][1])
        {
            case 'u':		/* device unit number */
                ++argv;
                --argc;
                if ((argc > 0) && (argv[0][0] >= '0') && (argv[0][0] <= '9'))
                    unit = atoi(argv[0]);
                else
                {
                    fprintf(stderr, "error: '-u' option requires a numeric argument\n", argv[0]);
                    usage(progname);
                    exit(1);
                }
                break;

            case 'c':	
                ++argv;
                --argc;
                if ((argc > 0) && (argv[0][0] >= '0') && (argv[0][0] <= '9'))
                    channel = atoi(argv[0]);
                else
                {
                    fprintf(stderr, "error: '-c' option requires a numeric argument\n", argv[0]);
                    usage(progname);
                    exit(1);
                }
                break;


            case '-':
                if (strcmp(argv[0], "--help") == 0) {
                    usage(progname);
                    exit(0);
                } else {
                    fprintf(stderr, "unknown option: %s\n", argv[0]);
                    usage(progname);
                    exit(1);
                }
                break;

            default:
                fprintf(stderr, "unknown flag -'%c'\n", argv[0][1]);
            case '?':
            case 'h':
                usage(progname);
                exit(0);
        }
        argc--;
        argv++;
    }

    /* first just echo to the file to overwrite or create it */
    sprintf(cmdstr, "echo OFF", OUTFILE);
    system(cmdstr);
    sprintf(cmdstr, "echo dvinfo output > %s", OUTFILE);
    system(cmdstr);

    if (is_simulator(unit))
        do_simulator(unit, channel);
    else do_framegrabber(unit, channel);
    do_systemstuff();

    printf("\nDone. Please include the file %s in any technical support\n", OUTFILE);
    printf("correspondence with EDT.\n\n");
}


void
do_systemstuff()
{
    char    cmdstr[128];

    /* system type: uname on unix, winapi on windows (done later) */
    /* now to windows specific stuff. */
#ifdef _NT_
    invoke("./win_sysinfo", OUTFILE);
#else
    invoke("uname -a", OUTFILE);
    invoke("dmesg |tail -30", OUTFILE);
#endif

    /*  cpu & memory info (from /proc on linux, psrinfo on sun,
     *  and windows api on NT)
     */
#ifdef __sun
    invoke("psrinfo -v", OUTFILE);
    invoke("prtconf -v -D", OUTFILE);
    invoke("isainfo -kv", OUTFILE);
#elif __linux__
    invoke("lspci -v", OUTFILE);
    sprintf(cmdstr, "%s /proc/cpuinfo", CAT);
    invoke(cmdstr, OUTFILE);
    sprintf(cmdstr, "%s /proc/meminfo", CAT);
    invoke(cmdstr, OUTFILE);
#endif

#ifdef _NT_
    sprintf(cmdstr, "%s ./%s", CAT, "install_pdv.log");
    invoke(cmdstr, OUTFILE);
#endif

}

void
do_simulator(int unit, int channel)
{
    char    cmdstr[128];

    printf("\npdv unit %d is configured as a SIMULATOR.\n");
    printf("\ndvinfo will run diagnostics on the board and dump the output into dvinfo.out.\n");
    if (unit == 0)
    {
        printf(" If there are multiple PDV boards in the\n");
        printf("system, you can run on a different board by using the '-u' <unit> option. For\n");
        printf("other options, see the users guide or run dvinfo --help.\n");

    }
    printf("\nMake sure that no other EDT DV simulator applications (e.g. edtsim) are\n");
    printf("running on this board, then press ENTER > "); fflush(stdout);
    getchar();

    sprintf(cmdstr, "./setdebug -v -u %d -c %d", unit, channel); 
    invoke(cmdstr, OUTFILE);

    invoke("./pciload", OUTFILE);

    sprintf(cmdstr, "./clsiminit -u %d -c %d -C -s -f camera_config/generic24cl.cfg", unit, channel);
    invoke(cmdstr, OUTFILE);
}

void
do_framegrabber(int unit, int channel)
{
    int    depth = 8;
    int    initialized = 0;
    int    cl_status = CL_PCLOCK_NA;
    int    cameralink = 0;
    char    cmdstr[128];
    char    cfgfname[128];
    char    setupfile[128];
    char    *countbits;
    PdvDev *pdv_p;

    printf("\ndvinfo will run diagnostics on pdv unit %d framegrabber, and dump the\n", unit);
    printf("output into the file dvinfo.out.\n");
    if (unit == 0)
    {
        printf(" If there are multiple PDV boards in the\n");
        printf("system, you can run on a different board by using the '-u' <unit> option. For\n");
        printf("other options, see the users guide or run dvinfo --help.");

    }
    printf("\nMake sure that your camera is connected and turned ON, the board has been\n");
    printf("initialized via initcam, camconfig or pdvshow, and that no other EDT DV\n");
    printf("applications are running (e.g. pdvshow). Then press ENTER > "); fflush(stdout);
    getchar();

    sprintf(cmdstr, "./setdebug -v -u %d", unit); 
    invoke(cmdstr, OUTFILE);

    invoke("./pciload", OUTFILE);

#ifdef _NT_
    sprintf(setupfile, "./camsetup%d_0.bat", unit);
#else
    sprintf(setupfile, "./pdvload");
#endif

    if (pdv_access(setupfile, 0) == 0)
    {
        sprintf(cmdstr, "%s %s", CAT, setupfile);
        invoke(cmdstr, OUTFILE);

        if (get_cfgfname(setupfile, cfgfname) == 0)
        {
            sprintf(cmdstr, "./initcam -V -O %s -f %s -u %d", TMPFILE, cfgfname, unit);
            invoke(cmdstr, OUTFILE);
            initialized = 1;

            sprintf(cmdstr, "%s ./%s", CAT, TMPFILE);
            invoke(cmdstr, OUTFILE);
        }
        else
        {
            sprintf(cmdstr, "error extracting cfg name from '%s' (?\?)", setupfile);
            echoit(cmdstr, OUTFILE);
        }
    }
    else
    {
        sprintf(cmdstr, "no %s, will try getting .cfg from pdv_p", setupfile);
        echoit(cmdstr, OUTFILE);
    }

    /* open the board  and gather some info */
    if ((pdv_p = pdv_open_channel(EDT_INTERFACE, unit, channel)) == NULL)
    {
        sprintf(cmdstr, "dvinfo couldn't open pdv%d_%d", unit, channel);
        echoit(cmdstr, OUTFILE);
        exit(1);
    }

    depth = pdv_p->dd_p->depth;
    strcpy(cfgfname, pdv_p->dd_p->cfgname);

    if (strlen(cfgfname) < 5)
    {
        echoit("no config name found, using generic", OUTFILE);
        if (pdv_is_cameralink(pdv_p))
            strcpy(cfgfname, "camera_config/generic16cl.cfg");
        else strcpy(cfgfname, "camera_config/generic16.cfg");
    }

    if (pdv_is_cameralink(pdv_p))
    {
        cameralink = 1;
        if (pdv_cl_camera_connected(pdv_p))
            cl_status = CL_PCLOCK_RUNNING;
        else cl_status = CL_PCLOCK_NOTRUNNING;
    }

    pdv_close(pdv_p);

    if (!initialized)
    {
        if (!cfgfname[0])
            echoit("can not find cfg name, skipping initcam", OUTFILE);

        else
        {
            sprintf(cmdstr, "./initcam -V -O %s -f %s -u %d", TMPFILE, cfgfname, unit);
            invoke(cmdstr, OUTFILE);
            initialized = 1;

        }
    }

    if (strlen(cfgfname) > 4)
    {
        sprintf(cmdstr, "%s %s", CAT, cfgfname);
        invoke(cmdstr, OUTFILE);
    }

    if (cameralink)
    {
        switch(cl_status)
        {
            case CL_PCLOCK_NA:
                echoit("Pixel clock register not implemented", OUTFILE);
                break;

            case CL_PCLOCK_NOTRUNNING:
                printf("Pixel clock not detected, make sure the camera is connected and ON.\n");
                printf("(NOTE: The pclock counter register is only implemented on some EDT boards/FPGAs.)\n");
                echoit("No pixel clock OR pixel clock register not implemented", OUTFILE);
                break;

            case CL_PCLOCK_RUNNING:
                echoit("Pixel clock OK", OUTFILE);
                break;

            default:
                echoit("Unknown pixel clock status (?)", OUTFILE);
                break;
        }
    }

    if (depth == 8)	  countbits = "./countbits -o taketest.raw";
    else if (depth == 24) countbits = "./countbits -c -o taketest.raw";
    else if (depth > 24)  countbits = "./countbits -i -o taketest.raw";
    else		  countbits = "./countbits -w -o taketest.raw";


    /* some takes; 1 buffer then 4 buffers */
    sprintf(cmdstr, "./take -v -u %d", unit);
    invoke(cmdstr, OUTFILE);


    sprintf(cmdstr, "./setdebug -u %d -g", unit); 
    invoke(cmdstr, OUTFILE);

    /* take 2 images with 2 buffers */
    sprintf(cmdstr, "./take -v -u %d -N 2 -l 2", unit); 
    invoke(cmdstr, OUTFILE);

    /* take 5 images with 4 buffers */
    sprintf(cmdstr, "./take -v -u %d -N 4 -l 5", unit); 
    invoke(cmdstr, OUTFILE);

    /* take to a file, default exposure, then countbits */
    sprintf(cmdstr, "./take -N 1 -u %d -f taketest.raw", unit); 
    invoke(cmdstr, OUTFILE);
    invoke(countbits, OUTFILE);

    /* take to a file with exposure 10 then countbits */
    sprintf(cmdstr, "./take -N 1 -e 10 -u %d -f taketest.raw", unit); 
    invoke(cmdstr, OUTFILE);
    invoke(countbits, OUTFILE);

    /* take to a file with exposure 500 then countbits */
    sprintf(cmdstr, "./take -N 1 -e 500 -u %d -f taketest.raw", unit); 
    invoke(cmdstr, OUTFILE);
    invoke(countbits, OUTFILE);


    sprintf(cmdstr, "./setdebug -d 0 -u %d", unit); 
    invoke(cmdstr, OUTFILE);


    if (cameralink)
    {
        sprintf(cmdstr, "./cl_speed -s -u %d", unit);
        invoke(cmdstr, OUTFILE);

#if 0 /* can't know which boards/FW has this and which not so skip it for now... rh 02/2012 */
        if (cl_status == CL_PCLOCK_RUNNING)
        {
            sprintf(cmdstr, "./check_gap_cl -u %d", unit);
            invoke(cmdstr, OUTFILE);

            sprintf(cmdstr, "./check_gap_cl -F 33 -u %d", unit);
            invoke(cmdstr, OUTFILE);
        }
#endif
    }


    if (*cfgfname)
    {
        sprintf(cmdstr, "./initcam -V -O %s -f %s -u %d", TMPFILE, cfgfname, unit);
        invoke(cmdstr, OUTFILE);
    }

}

void
invoke(char *cmd, char *outfile)
{
    char tmpcmd[256];
    char cmdstr[256];

    /* temporary buffer for correct_slashes */
    strcpy(tmpcmd, cmd);
    edt_correct_slashes(tmpcmd);
    printf("running: %s\n", tmpcmd);

    /* label */
    sprintf(cmdstr, "echo ------------------------------------------------------ >> %s", outfile);
    system(cmdstr);
    sprintf(cmdstr, "echo %s >> %s", tmpcmd, outfile);
    system(cmdstr);
    sprintf(cmdstr, "echo ------------------------------------------------------ >> %s", outfile);
    system(cmdstr);

    /* do the command */
    sprintf(cmdstr, "%s >> %s", tmpcmd, outfile);
    system(cmdstr);
}

void
echoit(char *str, char *outfile)
{
    char cmdstr[256];

    sprintf(cmdstr, "echo \"==> (%s)\" >> %s", str, outfile);
    system(cmdstr);

}

int
get_bdid(int unit)
{
    int ret;
    PdvDev *pdv_p = pdv_open(EDT_INTERFACE, unit);

    if (pdv_p == NULL)
        return 0;  
    ret = pdv_p->devid;
    pdv_close(pdv_p);
    return ret;
}


int
get_cfgfname(char *setupfile, char *cfgfname)
{
    char s[256];
    FILE *fp;

    if ((fp = fopen(setupfile, "r")) == NULL)
    {
        edt_perror(setupfile);
        return -1;
    }

    while (fgets(s, 255, fp))
    {
        char *p, *q;

        if (((p = grepit(s, "initcam ")) != NULL)
                && ((q = grepit(p, "-f ")) != NULL)
                && ((sscanf(q, "%s.cfg", cfgfname) == 1)))
            return 0;
    }

    cfgfname[0] = '\0';
    return 1;
}

int
is_simulator(int unit)
{
    PdvDev *pdv_p;
    char    cmdstr[128];
    int     ret = 0;

    if ((pdv_p = pdv_open(EDT_INTERFACE, unit)) == NULL)
    {
        sprintf(cmdstr, "dvinfo couldn't open pdv%d", unit);
        echoit(cmdstr, OUTFILE);
        exit(1);
    }
    if (pdv_is_simulator(pdv_p))
        ret = 1;

    pdv_close(pdv_p);
    return ret;
}

/*
 * search for a pattern in a char buffer, return pointer to next char if
 * found, NULL if not
 */
char   *
grepit(char *buf, char *pat)
{
    int     i;

    for (i = 0; i < (int) strlen(buf); i++)
    {
        if (buf[i] == pat[0])
        {
            if (strncmp(&buf[i], pat, strlen(pat)) == 0)
                return &buf[i + strlen(pat)];
        }
    }
    return NULL;
}

void
usage(char *progname)
{
    puts("");
    printf("usage: %s [-u unit]\n", progname);
    printf("  -u unit         %s unit number (default 0)\n", EDT_INTERFACE);
}

