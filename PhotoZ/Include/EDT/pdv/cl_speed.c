/*
 * pdvspeed
 *
 * check bus throughput using a PCI DV c-link board, using the
 * camera link channel 2 simulator
 *
 * works only on the pci dv c-link board, not applicable (so far)
 * on dv, dvk or dva.
 */
#include "edtinc.h"

void usage(char *progname);
void intro(char *progname);

#ifdef _NT_
#define TAKE "take.exe"
#define INITCAM "initcam.exe"
#define CAT "type"
#define SLASH "\\"
#else
#define TAKE "take"
#define INITCAM "initcam"
#define CAT "cat"
#define SLASH "/"
#endif

    static int
grepit(char *buf, char *pat)
{
    int     i;

    for (i = 0; i < (int) strlen(buf); i++)
    {
        if (buf[i] == pat[0])
        {
            if (strncmp(&buf[i], pat, strlen(pat)) == 0)
                return 1;
        }
    }
    return 0;
}

main(int argc, char **argv)
{
    double fake_speed = -1.0; /* for debugging only */
    int skip_intro = 0;
    int unit=0;
    double bps;
    char progname[64];
    char edt_devname[64];
    char xilinx_name[128];
    char errstr[320];
    char cmdstr[128];
    char s[128];
    FILE *fp;
    PdvDev *pdv_p;

    strcpy(progname, argv[0]);
    puts("");

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
                if ((argv[0][0] >= '0') && (argv[0][0] <= '9'))
                    unit = atoi(argv[0]);
                else
                    strncpy(edt_devname, argv[0], sizeof(edt_devname) - 1);
                break;

            case 's':		/* skip introductory message and pause for ENTER */
                skip_intro = 1;
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
            case 'f':
                ++argv;
                --argc;
                if ((argv[0][0] >= '0') && (argv[0][0] <= '9'))
                    fake_speed = atoi(argv[0]);
                else
                    strncpy(edt_devname, argv[0], sizeof(edt_devname) - 1);
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

    if ((edt_access(TAKE, 0) != 0)
            ||  (edt_access(INITCAM, 0) != 0))
    {
        sprintf(errstr, "\nERROR: Couldn't find required executables %s and/or %s. Make sure these\nprograms are both in the current directory and try again\n\n", TAKE, INITCAM);
        edt_msg(EDTAPP_MSG_FATAL, errstr);
        exit (1);
    }

    if (!skip_intro)
    {
        intro(progname);
        printf("Press ENTER to continue > ");
        fflush(stdout);
        fgets(s, 127, stdin);
    }

    /*
     * open the device, test for proper type
     */
    strcpy(edt_devname, EDT_INTERFACE);
    if ((pdv_p = pdv_open(edt_devname, unit)) == NULL)
    {
        sprintf(errstr, "pdv_open(%s%d)\n", edt_devname, unit);
        pdv_perror(errstr);
        exit (1);
    }

    if (!edt_is_dvcl(pdv_p)) /* not pdv_is_cameralink since that includes FOX */
    {
        sprintf(errstr, "exiting cl_speed (does not support this device type)\n");
        edt_msg(EDTAPP_MSG_FATAL, errstr);
        pdv_close(pdv_p);
        exit (1);
    }

    edt_flash_get_fname_auto(pdv_p, xilinx_name);
    /* printf("xilinx_name %s\n", xilinx_name); */
    if ((strcmp(xilinx_name, "pdvcamlk") != 0)
            && (strcmp(xilinx_name, "pdvcamlk2") != 0)
            && (strcmp(xilinx_name, "pedvcamlk32") != 0)
            && (strcmp(xilinx_name, "pe8dvcamlk") != 0)
            && (strcmp(xilinx_name, "pe4dvacamlk") != 0)
            && (strcmp(xilinx_name, "pe8dvacamlk") != 0)
            && (strcmp(xilinx_name, "pe8dvcamlk_fm") != 0)
            && (strcmp(xilinx_name, "pe8dvcamlk2") != 0)
            && (strcmp(xilinx_name, "pe8dvcamlk2_fm") != 0))
    {
        sprintf(errstr, "\nWARNING: -- %s requires standard base mode firmware, found\n%s. If cl_speed fails, reload firmware by running running\n'pciload pdvcamlk' (rev 51 and earlier boards) or 'pciload pdvcamlk2'\n(rev 52 and later)' and try again.\n\n", progname, xilinx_name);
        edt_msg(EDTAPP_MSG_WARNING, errstr);
    }

    pdv_close(pdv_p);

    sprintf(cmdstr, ".%sinitcam -u %d -c 2 -f camera_config%sgeneric16cl.cfg", SLASH, unit, SLASH);
    system(cmdstr);
    sprintf(cmdstr, ".%stake -j -u %d -c 2 -N 4 -l 100 > cl_speed.out", SLASH, unit);
    system(cmdstr);
    sprintf(cmdstr, "%s cl_speed.out", CAT);
    system(cmdstr);

    if ((fp = fopen("cl_speed.out", "r")) == NULL)
        printf("error opening output file -- check directory permissions\n");
    else
    {
        while (fgets(s, 127, fp))
        {
            if (grepit(s, "bytes/sec"))
            {
                sscanf(s, "%lf", &bps);
                break;
            }
        }
        fclose(fp);
    }

    if (fake_speed >= 0)
        bps = fake_speed * 1000000;

    printf("\nThroughput was measured at %1.2lf Megabytes per second.\n\n", bps/1000000.0);

    if (bps == 0)
        printf("This indicates a problem. check for misconfiguration, bad PCI slot, etc.\n");
    else if ((bps > 0) && (bps < 45000000))
    {
        printf("This indicates a very slow bus, or a heavily loaded 33MHz bus. Data loss\n");
        printf("(timeouts) are likely with all but the slowest cameras; remove other\n");
        printf("bandwidth \"hogs\" or move the board (or upgrade to a system with) a 66 MHz\n");
        printf("or faster PCI bus\n");
    }
    else if ((bps >= 45000000) && (bps < 65000000))
    {
        printf("This indicates a slow bus, or a moderately loaded 33MHz bus. Data loss may\n");
        printf("occur if your camera is very fast; if timeouts or image breakup is seen,\n");
        printf("remove other bandwidth \"hogs\" or move the board to (or migrate to a system\n");
        printf("with) a 66 MHz or faster PCI bus.\n");
    }
    else if ((bps >=65000000) && (bps < 130000000))
    {
        printf("This indicates the board is on a normally loaded 33 MHz PCI bus, or a 66+\n");
        printf("MHz bus that's heavily loaded or is being shared with some other 33 MHz\n");
        printf("Board. This can work for a cameras of slow to moderate speed, but if image\n");
        printf("breakup or timeouts are seen, it is recommended that you optimize by either\n");
        printf("removing other bandwidth \"hogs\" or migrating to a system with a 66 MHz or\n");
        printf("faster PCI bus.\n");
    }
    else if ((bps >= 130000000) && (bps < 180000000))
    {
        printf("This indicates the board is on a 66MHz or faster PCI bus but is saturated,\n");
        printf("possibly because the bus is being shared by some other board that generates\n");
        printf("significtant bus traffic. Not optimal, but may be okay unless the camera is\n");
        printf("relatively fast or high-bandwidth. If image breakup or timeouts are seen,\n");
        printf("remove any other bandwidth \"hogs\", or otherwise attempt to isolate the\n");
        printf("EDT board or migrate to a faster or more lightly-loaded bus.\n");
    }
    else if ((bps >= 180000000) && (bps < 250000000))
    {
        printf("Indicates the board has most of the bandwidth on a 66MHz or faster PCI\n");
        printf("bus, or PCI express. This is the optimal condition for an EDT PCI or 4-lane\n");
        printf("PCIe board. If it's an 8-lane board then it's working but slow.\n\n");
    }
    else if (bps >= 250000000)
    {
        printf("This must be a 4 or 8-lane PCI express board. Lookin' good.\n");
    }
}

    void
usage(char *progname)
{
    puts("");
    printf("usage: %s [-u unit]\n", progname);
    printf("  -u unit     %s unit number (default 0)\n", EDT_INTERFACE);
    printf("  -s          skip introductory message and pause for ENTER\n");
}

    void
intro(char *progname)
{
    printf("%s measures the bus bandwidth by pushing PCI DV C-Link built-in\n", progname);
    printf("sumulator data as fast as possible across the PCI bus. Note that the\n");
    printf("results represent only a 'snapshot' of the conditions present while the\n");
    printf("test is being run, using internally generated simulator data. Processing,\n");
    printf("displaying, and saving data will all impact the actual sustainable max\n");
    printf("throughput in a given system.\n\n");
    printf("IMPORTANT: Exit all other PCI DV acquisition applications before continuing.\n\n");
}
