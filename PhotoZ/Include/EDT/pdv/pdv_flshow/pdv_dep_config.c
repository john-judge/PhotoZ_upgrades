

#include "pdv_dep_config.h"
#include "edtinc.h"
#include "pdv_dependent.h"

#include "stddef.h"

#include "edt_types.h"

#define FAIL PDVLIB_MSG_FATAL
#define WARN PDVLIB_MSG_WARNING

#ifdef WIN32
#define strcasecmp stricmp
#endif


#define PD_NAME(field) #field

#define PD_SIZE(field)   (sizeof(default_dep.field))


#define PD_INT_ITEM(field, flags) {PD_NAME(field), NULL, NULL, offsetof(PdvDependent, field), sizeof(default_dep.field), TYPE_INT, flags}
#define PD_INT_TAG_ITEM(name, field, flags) {name, NULL, NULL, offsetof(PdvDependent, field), sizeof(default_dep.field), TYPE_INT, flags}

#define PD_STRING_ITEM(field, flags) {PD_NAME(field), NULL, NULL, offsetof(PdvDependent, field), sizeof(default_dep.field), TYPE_CHAR,flags}
#define PD_STRING_TAG_ITEM(name, field, flags) {name, NULL, NULL, offsetof(PdvDependent, field), sizeof(default_dep.field), TYPE_CHAR,flags}

#define PD_UINT_ITEM(field, flags) {PD_NAME(field), NULL, NULL, offsetof(PdvDependent, field), sizeof(default_dep.field), TYPE_UINT, flags}

#define PD_USHORT_ITEM(field, flags) {PD_NAME(field), NULL, NULL, offsetof(PdvDependent, field), sizeof(default_dep.field), TYPE_USHORT, flags}
#define PD_USHORT_TAG_ITEM(name, field, flags) {name, NULL, NULL, offsetof(PdvDependent, field), sizeof(default_dep.field), TYPE_USHORT, flags}

#define PD_FLOAT_ITEM(field, flags) {PD_NAME(field), NULL, NULL, offsetof(PdvDependent, field), sizeof(default_dep.field), TYPE_FLOAT, flags}
#define PD_FLOAT_TAG_ITEM(name, field, flags) {name, NULL, NULL, offsetof(PdvDependent, field), sizeof(default_dep.field), TYPE_FLOAT, flags}

#define PD_BYTE_ITEM(field, flags) {PD_NAME(field), NULL, NULL, offsetof(PdvDependent, field), sizeof(default_dep.field), TYPE_BYTE, flags}
#define PD_BYTE_TAG_ITEM(name, field, flags) {name, NULL, NULL, offsetof(PdvDependent, field), sizeof(default_dep.field), TYPE_BYTE, flags}

#define PD_BIT_ITEM(field, bits, bit_offset) \
    {PD_NAME(field), NULL, NULL, offsetof(PdvDependent, field), bits, TYPE_BIT, 0, bit_offset}
#define PD_BIT_TAG_ITEM(name, field, bits, bit_offset) \
    {name, NULL, NULL, offsetof(PdvDependent, field), bits, TYPE_BIT, 0, bit_offset}

#define PD_METHOD_ITEM(field, method_list) \
    {PD_NAME(field), NULL, NULL, offsetof(PdvDependent, field), \
    sizeof(default_dep.field), TYPE_INT, CFG_FORMAT_METHOD,0,0,0, method_list}

#define PD_METHOD_TAG_ITEM(name, field, method_list) \
    {name, NULL, NULL, offsetof(PdvDependent, field), \
    sizeof(default_dep.field), TYPE_INT, CFG_FORMAT_METHOD,0,0,0, method_list}

static PdvDependent default_dep;


static ConfigKeyValue pdv_method_args[] = {

    /* Translate the method_arg to a method number */
    { "AIA_MCL_100US",AIA_MCL_100US },
    { "AIA_MCL",AIA_MCL },
    { "AIA_TRIG",AIA_TRIG },
    { "KODAK_AIA_MCL",KODAK_AIA_MCL },
    { "AIA_MC4",AIA_MC4 },
    { "HAMAMATSU_4880_8X",HAM_4880_8X },
    { "HAMAMATSU_4880_SER",HAM_4880_SER },
    { "SU320_SERIAL",SU320_SERIAL },
    { "BASLER202K_SERIAL",BASLER202K_SERIAL },
    { "ADIMEC_SERIAL",ADIMEC_SERIAL },
    { "TIMC1001_SERIAL",TIMC1001_SERIAL },
    { "PTM6710_SERIAL",PTM6710_SERIAL },
    { "PTM1020_SERIAL",PTM1020_SERIAL },
    { "PULNIX_TM1000",PULNIX_TM1000 },
    { "PULNIX_TM9700",PULNIX_TM9700 },
    { "IRC_160",IRC_160 },
    { "KODAK_AIA_SER_CTRL",KODAK_AIA_SER_CTRL },
    { "AIA_SER_CTRL",AIA_SER_CTRL },
    { "KODAK_AIA_SER",KODAK_AIA_SER },
    { "SMD_SERIAL",SMD_SERIAL },
    { "AIA_SER_ES40",AIA_SERIAL_ES40 },
    { "AIA_SER",AIA_SERIAL },
    { "AIA_SERIAL",AIA_SERIAL },
    { "KODAK_SER_14I",KODAK_SER_14I },
    { "SER_14I",KODAK_SER_14I },
    { "SPECINST_SERIAL",SPECINST_SERIAL },
    { "KODAK_XHF_INTLC",PDV_BYTE_INTLV },
    { "BYTE_INTLV",PDV_BYTE_INTLV },
    { "FIELD_INTLC",PDV_FIELD_INTLC },
    { "BYTE_TEST1",PDV_BYTE_TEST1 },
    { "BYTE_TEST2",PDV_BYTE_TEST2 },
    { "BYTE_TEST3",PDV_BYTE_TEST3 },
    { "WORD_INTLV_HILO",PDV_WORD_INTLV_HILO },
    { "WORD_INTLV_ODD",PDV_WORD_INTLV_ODD },
    { "ES10_WORD_INTLC",PDV_WORD_INTLV },
    { "WORD_INTLV",PDV_WORD_INTLV },
    { "DALSA_4CH_INTLV",PDV_DALSA_4CH_INTLV },
    { "PIRANHA_4CH_INTLV",PDV_PIRANHA_4CH_INTLV },
    { "PIRANHA_4CH_HWINTLV",PDV_PIRANHA_4CH_HWINTLV },
    { "SPECINST_4PORT_INTLV",PDV_SPECINST_4PORT_INTLV },
    { "ILLUNIS_INTLV",PDV_ILLUNIS_INTLV },
    { "ES10_BGGR_INTLV",PDV_ES10_BGGR },
    { "ES10_WORD_BGGR_INTLV",PDV_ES10_WORD_BGGR },
    { "ES10_WORD_ODD_BGGR_INTLV",PDV_ES10_WORD_ODD_BGGR },
    { "ILLUNIS_BGGR",PDV_ILLUNIS_BGGR },
    { "QUADRANT_INTLV",PDV_QUADRANT_INTLV },
    { "DALSA_2CH_INTLV",PDV_DALSA_2CH_INTLV },
    { "INVERT_RIGHT_INTLV_24_12",PDV_INV_RT_INTLV_24_12 },
    { "INTLV_24_12",PDV_INTLV_24_12 },
    { "INTLV_1_8_MSB7",PDV_INTLV_1_8_MSB7 },
    { "INTLV_1_8_MSB0",PDV_INTLV_1_8_MSB0 },
    { "INVERT_RIGHT_INTLV",PDV_INVERT_RIGHT_INTLV },
    { "INVERT_RIGHT_BGGR_INTLV",PDV_INVERT_RIGHT_BGGR_INTLV },
    { "DALSA_2M30_INTLV",PDV_DALSA_2M30_INTLV },
    { "EVEN_RIGHT_INTLV",PDV_EVEN_RIGHT_INTLV },
    { "BGGR_DUAL",PDV_BGGR_DUAL },
    { "BGGR_WORD",PDV_BGGR_WORD },
    { "BGGR",PDV_BGGR },
    { "BGR_2_RGB",PDV_INTLV_BGR_2_RGB },
    { "KODAK_XHF_SKIP",PDV_BYTE_INTLV_SKIP },
    { "BYTE_INTLV_SKIP",PDV_BYTE_INTLV_SKIP },
    { "HW_ONLY",HW_ONLY },
    { "FMRATE_ENABLE",PDV_FMRATE_ENABLE },
    { "FVAL_ADJUST",PDV_FVAL_ADJUST },
    { "BASLER_202K",BASLER_202K },
    { "DUNCAN_2131",DUNCAN_2131 },

    { "FOI_REMOTE_AIA",FOI_REMOTE_AIA },

    { "DALSA_CONTINUOUS",DALSA_CONTINUOUS },

    { "EDT_ACT_NEVER",EDT_ACT_NEVER },
    { "EDT_ACT_ONCE",EDT_ACT_ONCE },
    { "EDT_ACT_ALWAYS",EDT_ACT_ALWAYS },
    { "EDT_ACT_ONELEFT",EDT_ACT_ONELEFT },
    { "EDT_ACT_CYCLE",EDT_ACT_CYCLE },
    { "EDT_ACT_KBS",EDT_ACT_KBS },

    { "BINARY",SERIAL_BINARY },
    { "ASCII_HEX",SERIAL_ASCII_HEX },
    { "ASCII",SERIAL_ASCII },
    { "DALSA_LS",PDV_DALSA_LS },
    { "HEADER_BEFORE",PDV_HEADER_BEFORE },
    { "HEADER_AFTER",PDV_HEADER_AFTER },
    { "HEADER_WITHIN",PDV_HEADER_WITHIN },
    { "DDCAM",PDV_DDCAM },
    { "RS232",PDV_SERIAL_RS232 },
    { "BASLER_FRAMING",SERIAL_BASLER_FRAMING },
    { "DUNCAN_FRAMING",SERIAL_DUNCAN_FRAMING },
    {NULL}
};

static ConfigKeyValue pdv_download_method_args[] = {
    { "IRC_160",IRC_160 },
    { "SPECINST_SERIAL",SPECINST_SERIAL },
    { NULL }
};

static ConfigKeyValue pdv_continuous_method_args[] = {
    { "AIA_MCL",AIA_MCL },
    { "AIA_SER",AIA_SERIAL },
    { "IRC_160",IRC_160 },
    { NULL }
};

static ConfigKeyValue pdv_shutter_method_args[] = {
    { "AIA_MCL_100US",AIA_MCL_100US },
    { "AIA_MCL",AIA_MCL },
    { "SU320_SERIAL",SU320_SERIAL },
    { "BASLER202K_SERIAL",BASLER202K_SERIAL },
    { "ADIMEC_SERIAL",ADIMEC_SERIAL },
    { "TIMC1001_SERIAL",TIMC1001_SERIAL },
    { "PTM6710_SERIAL",PTM6710_SERIAL },
    { "PTM1020_SERIAL",PTM1020_SERIAL },
    { "KODAK_AIA_SER_CTRL",KODAK_AIA_SER_CTRL },
    { "AIA_SER_CTRL",AIA_SER_CTRL },
    { "KODAK_AIA_SER",KODAK_AIA_SER },
    { "SMD_SERIAL",SMD_SERIAL },
    { "AIA_SER_ES40",AIA_SERIAL_ES40 },
    { "AIA_SER",AIA_SERIAL },
    { "AIA_SERIAL",AIA_SERIAL },
    { "KODAK_SER_14I",KODAK_SER_14I },
    { "SER_14I",KODAK_SER_14I },
    { "SPECINST_SERIAL",SPECINST_SERIAL },
    { "DALSA_LS",PDV_DALSA_LS },
    { "DALSA_CONTINUOUS",DALSA_CONTINUOUS },
    { "HAMAMATSU_4880_8X",HAM_4880_8X },
    { "HAMAMATSU_4880_SER",HAM_4880_SER },
    { "BASLER_202K",BASLER_202K },
    { "DUNCAN_2131",DUNCAN_2131 },
    { "PULNIX_1010",SERIAL_PULNIX_1010 },
    { "IRC_160",IRC_160 },
    { NULL}
};

static ConfigKeyValue pdv_intlv_method_args[] = {
    { "BYTE_INTLV",PDV_BYTE_INTLV },
    { "FIELD_INTLC",PDV_FIELD_INTLC },
    { "BYTE_TEST1",PDV_BYTE_TEST1 },
    { "BYTE_TEST2",PDV_BYTE_TEST2 },
    { "BYTE_TEST3",PDV_BYTE_TEST3 },
    { "WORD_INTLV_HILO",PDV_WORD_INTLV_HILO },
    { "WORD_INTLV_ODD",PDV_WORD_INTLV_ODD },
    { "ES10_WORD_INTLC",PDV_WORD_INTLV },
    { "WORD_INTLV",PDV_WORD_INTLV },
    { "DALSA_4CH_INTLV",PDV_DALSA_4CH_INTLV },
    { "PIRANHA_4CH_INTLV",PDV_PIRANHA_4CH_INTLV },
    { "PIRANHA_4CH_HWINTLV",PDV_PIRANHA_4CH_HWINTLV },
    { "SPECINST_4PORT_INTLV",PDV_SPECINST_4PORT_INTLV },
    { "ILLUNIS_INTLV",PDV_ILLUNIS_INTLV },
    { "ES10_BGGR_INTLV",PDV_ES10_BGGR },
    { "ES10_WORD_BGGR_INTLV",PDV_ES10_WORD_BGGR },
    { "ES10_WORD_ODD_BGGR_INTLV",PDV_ES10_WORD_ODD_BGGR },
    { "ILLUNIS_BGGR",PDV_ILLUNIS_BGGR },
    { "QUADRANT_INTLV",PDV_QUADRANT_INTLV },
    { "DALSA_2CH_INTLV",PDV_DALSA_2CH_INTLV },
    { "INVERT_RIGHT_INTLV_24_12",PDV_INV_RT_INTLV_24_12 },
    { "INTLV_24_12",PDV_INTLV_24_12 },
    { "INTLV_1_8_MSB7",PDV_INTLV_1_8_MSB7 },
    { "INTLV_1_8_MSB0",PDV_INTLV_1_8_MSB0 },
    { "INVERT_RIGHT_INTLV",PDV_INVERT_RIGHT_INTLV },
    { "INVERT_RIGHT_BGGR_INTLV",PDV_INVERT_RIGHT_BGGR_INTLV },
    { "DALSA_2M30_INTLV",PDV_DALSA_2M30_INTLV },
    { "EVEN_RIGHT_INTLV",PDV_EVEN_RIGHT_INTLV },
    { "BGGR_DUAL",PDV_BGGR_DUAL },
    { "BGGR_WORD",PDV_BGGR_WORD },
    { "BGGR",PDV_BGGR },
    { "BGR_2_RGB",PDV_INTLV_BGR_2_RGB },
    { "KODAK_XHF_SKIP",PDV_BYTE_INTLV_SKIP },
    { "BYTE_INTLV_SKIP",PDV_BYTE_INTLV_SKIP },
    { NULL }
};

static ConfigKeyValue pdv_act_method_args[] = {
    { "EDT_ACT_NEVER",EDT_ACT_NEVER },
    { "EDT_ACT_ONCE",EDT_ACT_ONCE },
    { "EDT_ACT_ALWAYS",EDT_ACT_ALWAYS },
    { "EDT_ACT_ONELEFT",EDT_ACT_ONELEFT },
    { "EDT_ACT_CYCLE",EDT_ACT_CYCLE },
    { "EDT_ACT_KBS",EDT_ACT_KBS },
    { NULL }
};

static ConfigKeyValue pdv_header_method_args[] = {
    { "HEADER_BEFORE",PDV_HEADER_BEFORE },
    { "HEADER_AFTER",PDV_HEADER_AFTER },
    { "HEADER_WITHIN",PDV_HEADER_WITHIN },
    { NULL }
};

static ConfigKeyValue pdv_serial_method_args[] = {
    { "BINARY",SERIAL_BINARY },
    { "ASCII_HEX",SERIAL_ASCII_HEX },
    { "ASCII",SERIAL_ASCII },
    { "PULNIX_1010",SERIAL_PULNIX_1010 },
    { "BASLER_FRAMING",SERIAL_BASLER_FRAMING },
    { "DUNCAN_FRAMING",SERIAL_DUNCAN_FRAMING },
    { NULL }
};

static ConfigKeyValue pdv_serial_mode_args[] = {
    { "RS232",PDV_SERIAL_RS232 },
    { NULL }
};

static ConfigKeyValue pdv_frame_timing_args[] = {
    { "FVAL_ADJUST",PDV_FVAL_ADJUST },
    { NULL }
};



int nConfigKeyValues = (sizeof(pdv_method_args)/sizeof(ConfigKeyValue))-1;

static CfgFileItem pdv_dep_fields[] = {
    PD_INT_ITEM(width, 0),
	PD_INT_ITEM(height, 0),
	PD_INT_ITEM(depth, 0),
	PD_INT_ITEM(imagesize, 0),
	PD_INT_ITEM(extdepth, 0),
	PD_INT_ITEM(slop, 0),
	PD_INT_ITEM(hwpad, 0),
	PD_INT_TAG_ITEM("rgb30", hwpad, 0),
	PD_INT_TAG_ITEM("irris_strip", hwpad, 0),
	PD_INT_ITEM(byteswap, 0),
	PD_INT_ITEM(image_offset, 0),
	PD_INT_ITEM(interlace, 0),
	PD_INT_ITEM(shutter_speed, 0),
	PD_INT_ITEM(shutter_speed_frontp, 0),
	PD_INT_ITEM(shutter_speed_min, 0),
	PD_INT_ITEM(shutter_speed_max, 0),
	PD_INT_ITEM(default_shutter_speed, 0),
	PD_INT_ITEM(aperture, 0),
	PD_INT_ITEM(aperture_min, 0),
	PD_INT_ITEM(aperture_max, 0),
	PD_INT_ITEM(default_aperture, 0),
	PD_INT_ITEM(gain_frontp, 0),
	PD_INT_ITEM(gain_min, 0),
	PD_INT_ITEM(gain_max, 0),
	PD_INT_ITEM(default_gain, 0),
	PD_INT_ITEM(offset_frontp, 0),
	PD_INT_ITEM(offset_min, 0),
	PD_INT_ITEM(offset_max, 0),
	PD_INT_ITEM(continuous, 0),
	PD_INT_ITEM(pclock_speed, 0),
	PD_INT_ITEM(hwinterlace, 0),
	PD_INT_ITEM(inv_shutter, 0),
	PD_INT_ITEM(trig_pulse, 0),
	PD_INT_ITEM(dis_shutter, 0),
	PD_INT_ITEM(mode_cntl_norm, CFG_FORMAT_HEX),
	PD_INT_ITEM(trigdiv, CFG_FORMAT_HEX),
	PD_INT_ITEM(maxdmasize, CFG_FORMAT_HEX),
	PD_INT_ITEM(direction, CFG_FORMAT_HEX),
	PD_INT_ITEM(cameratest, 0),
	PD_INT_ITEM(genericsim, 0),
	PD_INT_ITEM(sim_width, 0),
	PD_INT_ITEM(sim_height, 0),
	PD_INT_ITEM(line_delay, 0),
	PD_INT_ITEM(frame_delay, 0),
	PD_INT_ITEM(frame_height, 0),
	PD_INT_ITEM(fv_once, 0),
	PD_INT_ITEM(enable_dalsa, 0),
	PD_METHOD_ITEM(lock_shutter, pdv_shutter_method_args),
	PD_METHOD_ITEM(camera_shutter_timing, pdv_shutter_method_args),
	PD_METHOD_TAG_ITEM("shutter_speed",camera_shutter_timing, pdv_shutter_method_args),
	PD_METHOD_ITEM(camera_continuous, pdv_shutter_method_args),
	PD_METHOD_ITEM(camera_binning, pdv_shutter_method_args),
	PD_METHOD_ITEM(camera_data_rate, pdv_shutter_method_args),
	PD_METHOD_ITEM(camera_download, pdv_shutter_method_args),
	PD_METHOD_ITEM(get_gain, pdv_shutter_method_args),
	PD_METHOD_ITEM(get_offset, pdv_shutter_method_args),
	PD_METHOD_ITEM(set_gain, pdv_shutter_method_args),
	PD_METHOD_ITEM(set_offset, pdv_shutter_method_args),
	PD_INT_ITEM(default_offset, 0),
	PD_METHOD_ITEM(first_open, pdv_shutter_method_args),
	PD_METHOD_ITEM(last_close, pdv_shutter_method_args),
	PD_INT_ITEM(pingpong_varsize, 0),
	PD_INT_ITEM(image_depth, 0),
	PD_INT_ITEM(interlace_offset, 0),
	PD_INT_ITEM(get_aperture, 0),
	PD_INT_ITEM(set_aperture, 0),
	PD_INT_ITEM(timeout_multiplier, 0),
	PD_INT_TAG_ITEM("simulator_speed",sim_speed, 0),
	PD_METHOD_TAG_ITEM("interlace",swinterlace, pdv_intlv_method_args),
	PD_INT_ITEM(started, 0),
	PD_INT_ITEM(foi_unit, 0),
	PD_INT_ITEM(disable_mdout, 0),
	PD_INT_ITEM(fixedlen, 0),
	PD_METHOD_TAG_ITEM("framesync", framesync_mode, 0),
	PD_METHOD_ITEM(camera_shutter_speed, pdv_shutter_method_args),
	PD_INT_ITEM(dual_channel, 0),
	PD_INT_ITEM(gain, 0),
	PD_INT_ITEM(level, 0),
	PD_INT_ITEM(gendata, 0),
	PD_INT_ITEM(skip, 0),
	PD_INT_ITEM(shortswap, 0),
	PD_INT_ITEM(markras, 0),
	PD_INT_ITEM(markbin, 0),
	PD_INT_ITEM(rascnt, 0),
	PD_INT_ITEM(markrasx, 0),
	PD_INT_ITEM(markrasy, 0),
	PD_INT_ITEM(xilinx_rev, 0),
	PD_INT_ITEM(xilinx_opts, 0),
	PD_INT_ITEM(util2, CFG_FORMAT_HEX),
	PD_INT_ITEM(shift, CFG_FORMAT_HEX),
	PD_INT_ITEM(mask, CFG_FORMAT_HEX),
	PD_INT_ITEM(linerate, 0),
	PD_INT_ITEM(header_size, 0),
	PD_INT_ITEM(photo_trig, 0),
	PD_INT_ITEM(fieldid_trig, 0),
	PD_INT_ITEM(acquire_mult, 0),
	PD_INT_ITEM(timeout, 0),
	PD_INT_ITEM(sim_enable, 0),
	PD_INT_ITEM(startup_delay, 0),
	PD_INT_ITEM(last_raw, 0),
	PD_INT_ITEM(last_image, 0),
	PD_BYTE_ITEM(datapath_reg, 0),
	PD_BYTE_ITEM(config_reg, 0),
	PD_STRING_ITEM(cameratype, CFG_FORMAT_INONLY),
	PD_STRING_ITEM(rbtfile, 0),
	PD_STRING_ITEM(foi_remote_rbtfile, 0),
	PD_STRING_ITEM(interlace_module, 0),
	PD_STRING_ITEM(serial_init, 0),
	PD_STRING_TAG_ITEM("serial_init_baslerf",serial_init, 0),
	PD_STRING_TAG_ITEM("serial_init_duncanf",serial_init, 0),
	PD_STRING_TAG_ITEM("serial_init_hex",serial_init, 0),
	PD_STRING_TAG_ITEM("serial_binit",serial_init, 0),
	PD_STRING_ITEM(foi_init, 0),
	PD_STRING_ITEM(xilinx_init, 0),
	PD_STRING_ITEM(serial_exposure, 0),
	PD_STRING_ITEM(serial_gain, 0),
	PD_STRING_ITEM(serial_offset, 0),
	PD_STRING_ITEM(serial_prefix, 0),
	PD_STRING_ITEM(serial_binning, 0),
	PD_STRING_ITEM(RESERVED1, 0),
	PD_STRING_ITEM(RESERVED2, 0),
	PD_STRING_ITEM(camera_model, 0),
	PD_STRING_ITEM(camera_info, 0),
	PD_STRING_ITEM(serial_response, 0),
	PD_STRING_ITEM(RESERVED4, 0),
	PD_STRING_ITEM(serial_term, 0),
	PD_STRING_ITEM(idstr, 0),
	PD_INT_ITEM(serial_timeout, 0),
	PD_INT_ITEM(serial_respcnt, 0),
	PD_METHOD_ITEM(serial_format, pdv_serial_method_args),
	PD_INT_ITEM(strobe_count, 0),
	PD_INT_ITEM(strobe_interval, 0),
	PD_INT_ITEM(strobe_enabled, 0),
	PD_INT_ITEM(sel_mc4, 0),
	PD_INT_ITEM(mc4, 0),
	PD_INT_ITEM(serial_baud, 0),
	PD_INT_ITEM(user_timeout, 0),
	PD_INT_ITEM(user_timeout_set, 0),
	PD_INT_ITEM(cam_width, 0),
	PD_INT_ITEM(cam_height, 0),
	PD_INT_ITEM(hskip, 0),
	PD_INT_ITEM(hactv, 0),
	PD_INT_ITEM(vskip, 0),
	PD_INT_ITEM(vactv, 0),
	PD_INT_ITEM(cnt_continuous, 0),
	PD_INT_ITEM(sim_ctl, 0),
	PD_INT_ITEM(started_continuous, 0),
	PD_INT_ITEM(kbs_red_row_first, 0),
	PD_INT_ITEM(kbs_green_pixel_first, 0),
	PD_STRING_ITEM(camera_download_file, 0),
	PD_STRING_ITEM(camera_command_file, 0),
	PD_STRING_ITEM(camera_class, 0),
	PD_INT_ITEM(double_rate, 0),
	PD_INT_ITEM(force_single, 0),
	PD_STRING_ITEM(serial_trigger, 0),
	PD_INT_ITEM(variable_size, 0),
	PD_INT_ITEM(header_offset, 0),
	PD_INT_ITEM(header_dma, 0),
	PD_METHOD_ITEM(header_position, pdv_header_method_args),
	PD_INT_ITEM(pause_for_serial, 0),
	PD_INT_ITEM(roi_enabled, 0),
	PD_INT_ITEM(binx, 0),
	PD_INT_ITEM(biny, 0),
	PD_METHOD_ITEM(serial_mode, pdv_serial_mode_args),
	PD_INT_ITEM(inv_ptrig, 0),
	PD_INT_ITEM(inv_fvalid, 0),
	PD_INT_TAG_ITEM("CL_CFG_NORM",cl_cfg, CFG_FORMAT_HEX),
	PD_INT_TAG_ITEM("CL_DATA_PATH_NORM", cl_data_path, CFG_FORMAT_HEX),
	PD_INT_ITEM(htaps, 0),
	PD_INT_ITEM(vtaps, 0),
	PD_INT_ITEM(pulnix, 0),
	PD_INT_ITEM(dbl_trig, 0),
	PD_INT_ITEM(cameralink, CFG_FORMAT_INONLY),
	PD_INT_ITEM(start_delay, 0),
	PD_INT_ITEM(serial_waitc, CFG_FORMAT_HEX),
	PD_INT_ITEM(frame_period, 0),
	PD_METHOD_ITEM(frame_timing, pdv_frame_timing_args),
	PD_INT_ITEM(register_wrap, 0),
	PD_INT_ITEM(fval_done, 0),
	PD_INT_ITEM(cl_hmax, CFG_FORMAT_HEX),
	PD_INT_ITEM(serial_init_delay, 0),
	PD_INT_ITEM(mode16, 0),
	PD_INT_ITEM(n_intlv_taps, 0),
	PD_INT_ITEM(cl_channels, 0),
	PD_BYTE_ITEM(intlv_taps, 0),
	PD_METHOD_ITEM(startdma, pdv_act_method_args),
	PD_METHOD_ITEM(enddma, pdv_act_method_args),
	PD_METHOD_ITEM(flushdma, pdv_act_method_args),

	/* all of the members of the ClSimControl structure */

	PD_BIT_TAG_ITEM("cls_linescan",cls.flags.b.cfga,1,7),
	PD_BIT_TAG_ITEM("cls_lvcont",cls.flags.b.cfga,1,6),
	PD_BIT_TAG_ITEM("cls_rven",cls.flags.b.cfga,1,5),
	PD_BIT_TAG_ITEM("cls_uartloop",cls.flags.b.cfga,1,4),
	PD_BIT_TAG_ITEM("cls_smallok",cls.flags.b.cfga,1,3),
	PD_BIT_TAG_ITEM("cls_intlven",cls.flags.b.cfga,1,2),
	PD_BIT_TAG_ITEM("cls_firstfc",cls.flags.b.cfga,1,1),
	PD_BIT_TAG_ITEM("cls_datacnt",cls.flags.b.cfga,1,0),

	PD_BIT_TAG_ITEM("cls_firstfc",cls.flags.b.cfgb,4,4),
	PD_BIT_TAG_ITEM("cls_datacnt",cls.flags.b.cfgb,4,0),

	PD_BIT_TAG_ITEM("cls_led",cls.flags.b.cfgc,1,4),
	PD_BIT_TAG_ITEM("cls_trigsrc",cls.flags.b.cfgc,1,3),
	PD_BIT_TAG_ITEM("cls_trigpol",cls.flags.b.cfgc,1,2),
	PD_BIT_TAG_ITEM("cls_trigframe",cls.flags.b.cfgc,1,1),
	PD_BIT_TAG_ITEM("cls_trigline",cls.flags.b.cfgc,1,0),

	PD_BYTE_TAG_ITEM("cls_Exsyncdly", cls.Exsyncdly,CFG_FORMAT_HEX),
	PD_BYTE_TAG_ITEM("cls_FillA", cls.FillA,CFG_FORMAT_HEX),
	PD_BYTE_TAG_ITEM("cls_FillB", cls.FillB,CFG_FORMAT_HEX),
	PD_BYTE_TAG_ITEM("cls_Exsyncdly", cls.Exsyncdly,CFG_FORMAT_HEX),

	PD_USHORT_TAG_ITEM("cls_hblank",cls.hblank,0),
	PD_USHORT_TAG_ITEM("cls_hcntmax",cls.Hcntmax,0),

	PD_USHORT_TAG_ITEM("cls_vblank",cls.vblank,0),
	PD_USHORT_TAG_ITEM("cls_vcntmax",cls.Vcntmax,0),

	PD_USHORT_TAG_ITEM("cls_hfvstart",cls.Hfvstart,0),
	PD_USHORT_TAG_ITEM("cls_hfvend",cls.Hfvend,0),

	PD_USHORT_TAG_ITEM("cls_hlvstart",cls.Hlvstart,0),
	PD_USHORT_TAG_ITEM("cls_hlvend",cls.Hlvend,0),

	PD_USHORT_TAG_ITEM("cls_hrvstart",cls.Hrvstart,0),
	PD_USHORT_TAG_ITEM("cls_hrvend",cls.Hrvend,0),

	PD_FLOAT_TAG_ITEM("cls_pixel_clock", cls.pixel_clock, 0),

    {NULL}
};


CfgFileSet *pDepSet = NULL;

#define NFIELDS (sizeof(pdv_dep_fields)/sizeof(CfgFileItem))-1

static int pdv_pep_nfields = NFIELDS;

/**
 *
 * Compare two CfgFileItems to see if they refer to the same memory
 *
 * @param f1 first item pointer
 * @param f2 second item pointer
 *
 * @return 1 if they are the same, 0 if they are not
 *
 */

static int equivalent_fields(CfgFileItem *f1, CfgFileItem *f2)

{
    if (f1->base_offset == f2->base_offset &&
	f1->bit_offset == f2->bit_offset &&
	f1->size == f2->size)
	return 1;

    return 0;

}



void
pdv_dep_default(PdvDependent *dd_p)

{
    int i;

    /*
    * clear dependent struct and set defaults to nonsense values
    */
    memset(dd_p, 0, sizeof(PdvDependent));
    dd_p->rbtfile[0] = '\0';
    dd_p->cameratype[0] = '\0';
    dd_p->shutter_speed = NOT_SET;
    dd_p->default_shutter_speed = NOT_SET;
    dd_p->default_gain = NOT_SET;
    dd_p->default_offset = NOT_SET;
    dd_p->default_aperture = NOT_SET;
    dd_p->binx = 1;
    dd_p->biny = 1;
    dd_p->byteswap = NOT_SET;
    dd_p->serial_timeout = 1000;
    dd_p->serial_response[0] = '\r';
    dd_p->xilinx_rev = NOT_SET;
    dd_p->timeout = NOT_SET;
    dd_p->user_timeout = NOT_SET;
    dd_p->mode_cntl_norm = NOT_SET;
    dd_p->mc4 = NOT_SET;
    dd_p->pulnix = 0;
    dd_p->dbl_trig = 0;
    dd_p->shift = NOT_SET;
    dd_p->mask = 0xffff;
    dd_p->serial_baud = NOT_SET;
    dd_p->serial_waitc = NOT_SET ;
    dd_p->serial_format = SERIAL_ASCII;
    strcpy(dd_p->serial_term, "\r");	/* term for most ASCII exc. ES4.0 */

    dd_p->kbs_red_row_first = 1;
    dd_p->kbs_green_pixel_first = 0;


    dd_p->htaps = 1;
    dd_p->vtaps = 1;

    dd_p->cameralink = 0;
    dd_p->start_delay = 0;
    dd_p->frame_period = NOT_SET;
    dd_p->frame_timing = NOT_SET;

    dd_p->strobe_enabled = NOT_SET;
    dd_p->register_wrap = 0;
    dd_p->serial_init_delay = NOT_SET;

    dd_p->startdma = NOT_SET;
    dd_p->enddma = NOT_SET;
    dd_p->flushdma = NOT_SET;

    /*
    * xregwrite registers can be 0-ff. We need a way to flag the
    * end of the array, so just waste ff and call that "not set"
    */
    for (i=0; i<32; i++)
	dd_p->xilinx_flag[i] = 0xff;

}

/**
 *
 * Returns the predefined set of config items with their offsets and types
 * in the PdvDependent structure. Returned as a CfgFileSet for easy access.
 *
 * The first time in it initializes the pDepSet pointer and creates a default
 * dependent structure by calling pdv_dep_default on the default_dep - so there's
 * a complete set of fields described by pDepSet and a default PdvDependent structure.
 *
 * @return pointer to the pDepSet structure
 */

CfgFileSet *pdv_get_config_fields()

{

    int i;
    int j;
    int ngroups = 0;

    if (!pDepSet)
    {
	pDepSet = new_cfg_set();

	pDepSet->pItems = (CfgFileItem *) calloc(NFIELDS, sizeof (pdv_dep_fields));
	memcpy(pDepSet->pItems, pdv_dep_fields, sizeof(pdv_dep_fields));
	pDepSet->nItems = NFIELDS;

	pDepSet->ignore_case = TRUE;

	for (i=0;i<NFIELDS;i++)
	{
	    char fullname[80];
	    CfgFileItem *pItem = pDepSet->pItems + i;

	    if (pItem->format_flags & CFG_FORMAT_METHOD)
	    {
		sprintf(fullname, "method_%s", pItem->ItemName);
	    }
	    else
	    {
		strcpy(fullname, pItem->ItemName);
	    }

	    pItem->ItemName = strdup(fullname);

	    for (j=0;j<i;j++)
	    {
		CfgFileItem * pTestItem = pDepSet->pItems + j;

		if (equivalent_fields(pItem, pTestItem))
		{
		    pItem->format_flags |= CFG_FORMAT_INONLY;
		    if (pTestItem->group_number)
			pItem->group_number = pTestItem->group_number;
		    else
			pItem->group_number = pTestItem->group_number = ++ngroups;
		    
		    while (pTestItem->group_next)
		    {
			pTestItem = pDepSet->pItems + pTestItem->group_next;
		    }

		    pTestItem->group_next = i;

		    break;
		}
	    }
	}

	pDepSet->nItemsAllocated = NFIELDS;

	pdv_dep_default(&default_dep);

    }

    return pDepSet;

}

/**
 *
 * @return This returns the mask of bits described by a bit type item
 *
 */

u_char bit_field_mask(CfgFileItem *pItem)

{
    int i;
    int shift = pItem->bit_offset;
    u_char mask = 0;

    for (i=pItem->size;i> 0; i--)
    {
	mask |= 1 << shift;
	shift--;
    }

    return mask;
}

/**
 *
 * Compares the values in two PdvDependent structures based on the type and offset in 
 * pItem
 * Does binary compare using memcmp, so strings that match will fail if there's extra stuff
 * past the '\0'.
 *
 * @param pItem: the item descriptor
 * @param dd_p1: pointer to the first PdvDependent structure
 * @param dd_p1: pointer to the first PdvDependent structure
 *
 * @return 0 if the data is identical, non-zero otherwise (like memcmp)
 *
 */

int compare_fields(CfgFileItem *pItem, PdvDependent *dd_p1, PdvDependent *dd_p2)

{
    u_char *p1;
    u_char *p2;

    p1 = ((u_char *) dd_p1) + pItem->base_offset;
    p2 = ((u_char *) dd_p2) + pItem->base_offset;

    if (pItem->type == TYPE_BIT)
    {
	u_char mask = bit_field_mask(pItem);

	return ((*p1 & mask) != (*p2 & mask));

    }
    else
    {
	return memcmp(p1, p2, pItem->size);
    }

}

/**
 *
 * Given a CfgFileItem which has the name xregwrite_xx, add it to
 * xilinx_flag/xilinx_value array
 *
 * @param pItem: item with name/value from a config file
 * @param dd_p: PdvDependent struct to fill in
 *
 * @return 0 on success, -1 on failure due to inapprropriate name or value
 *
 */

int
pdv_parse_xilinx_item(CfgFileItem *pItem, Dependent * dd_p)
{
    int     index = 0;
    u_int   reg = 0;
    u_int   value = 0;
    char   *name=pItem->ItemName;
    char   *hexstr=pItem->ItemValue;

    if (strncmp(name, "xregwrite_", 10) == 0)
    {
	char *regs = name + 10;
	reg = strtol(regs, NULL, 10);

	if (reg > 0xfe)
	{
	    edt_msg(WARN,
		"Error in parsing %s.  Address %d is out of range for xilinx_%d.\n",
		name, reg, reg);
	    edt_msg(WARN, "Legal values are 0 through 254\n");
	    return -1;
	}

	value = strtol(hexstr, NULL, 16);

	if (value > 0xff)
	{
	    edt_msg(WARN,
		"Error in parsing %s.  Value %x is out of range for xilinx_%d.\n",
		name, value, reg);
	    edt_msg(WARN, "Legal values are 0 through ff\n");
	    return -1;
	}

	/*
	* xilinx_flag used to just be just a flag 0 or 1, now it's the actual
	* address of the register, 0-254
	*/
	while ((index < 32) && dd_p->xilinx_flag[index] != 0xff)
	    ++index;

	if (index >= MAXXIL)
	    edt_msg(WARN, "Error! Too_many xregwrite_ directives (max %d)\n",MAXXIL);
	else
	{
	    dd_p->xilinx_flag[index] = reg;
	    dd_p->xilinx_value[index] = value;
	}
	return 0;
    }
    else return -1;

}

/**
 *
 * Given a CfgFileItem read from a file (pItem) and an item (fmt_item) which describes the
 * data type and offset in the PdvDependent structure (dd_p), parse the pItem's ItemValue field
 * and fill in the correct location in the dd_p.
 *
 * @param pItem: item read from a file
 * @param fmt_item: descriptor item w/ offset and type
 * @param dd_p: pointer to PdvDependent structure to be filled in
 *
 * @return 0 on success, -1 on failure
 *
 */


int
pdv_parse_cfg_file_input(CfgFileItem *pItem, CfgFileItem *fmt_item, PdvDependent *dd_p)

{
    u_int ui;
    int i;
    float f;
    u_char b;
    u_short us;

    if (!pItem->ItemValue)
    {
	printf("No Value String for %s\n", pItem->ItemName);
	return -1;
    }

    /* special for xregwrite */

    if (fmt_item->format_flags & CFG_FORMAT_METHOD)
    {
	/* search key/value pairs */

	if (fmt_item->possible_values)
	{

	    ConfigKeyValue *pkv = fmt_item->possible_values;

	    int * p = (u_int*) (((u_char *) dd_p) + fmt_item->base_offset);
	    u_char found = false;

	    while (pkv->idstr && !found)
	    {
		if (!strcasecmp(pItem->ItemValue, pkv->idstr))
		{
		    *p = pkv->value;
		    found = TRUE;
		    break;
		}
		pkv++;
	    }

	    if (!found)
	    {
		edt_msg(WARN, "parse_cfg_file_input: %s value %s not found\n",
		    pItem->ItemName, pItem->ItemValue);
		return -1;
	    }

	}

    }
    else
    {
	switch(fmt_item->type)
	{
	case TYPE_BIT:
	    {

		u_char mask = bit_field_mask(fmt_item);
		u_char * p = ((u_char *) dd_p) + fmt_item->base_offset;
		b = (u_char) strtoul(pItem->ItemValue,NULL, 16);	

		*p &= ~mask;

		*p |= (b << fmt_item->bit_offset);
	    }

	    break;

	case TYPE_BYTE:
	    {   
		u_char * p = ((u_char *) dd_p) + fmt_item->base_offset;
		if (fmt_item->size == sizeof(u_char))
		{
		    if (fmt_item->format_flags & CFG_FORMAT_HEX)
			b = (u_char) strtoul(pItem->ItemValue,NULL, 16);
		    else
			b = (u_char) strtoul(pItem->ItemValue,NULL, 10);

		    *p = b;
		}

	    }
	    break;

	case TYPE_USHORT:
	    {   
		u_short * p = (u_short*) (((u_char *) dd_p) + fmt_item->base_offset);
		if (fmt_item->size == sizeof(u_short))
		{
		    if (fmt_item->format_flags & CFG_FORMAT_HEX)
			us = (u_short)strtoul(pItem->ItemValue,NULL, 16);
		    else
			us = (u_short)strtoul(pItem->ItemValue,NULL, 10);

		    *p = us;
		}

	    }
	    break;

	case TYPE_UINT:
	    {   
		u_int * p = (u_int*) (((u_char *) dd_p) + fmt_item->base_offset);

		if (fmt_item->size == sizeof(u_int))
		{
		    if (fmt_item->format_flags & CFG_FORMAT_HEX)
			ui = strtoul(pItem->ItemValue,NULL, 16);
		    else
			ui = strtoul(pItem->ItemValue,NULL, 10);

		    *p = ui;
		}

	    }
	    break;
	case TYPE_INT:
	    {   
		int * p = (u_int*) (((u_char *) dd_p) + fmt_item->base_offset);

		if (fmt_item->size == sizeof(int))
		{
		    if (fmt_item->format_flags & CFG_FORMAT_HEX)
			i = strtol(pItem->ItemValue,NULL, 16);
		    else
			i = strtol(pItem->ItemValue,NULL, 10);

		    *p = i;
		}

	    }
	    break;
	case TYPE_FLOAT:
	    {   
		float * p = (float*) (((u_char *) dd_p) + fmt_item->base_offset);

		if (fmt_item->size == sizeof(float))
		{
		    f = (float) atof(pItem->ItemValue);

		    *p = f;
		}

	    }
	    break;
	case TYPE_CHAR:
	    {
		char *p = (((u_char *) dd_p) + fmt_item->base_offset);

		strncpy(p, pItem->ItemValue, fmt_item->size-1);

	    }
	    break;

	default:

	    printf("Field %s not parsed\n", pItem->ItemName);
	    return -1;

	}
    }

    return 0;

}

/**
 *
 * Given a CfgFileItem read from a file (pItem) and an item (fmt_item) which describes the
 * data type and offset in the PdvDependent structure (dd_p), format the value from dd_p
 * and put the string into s
 *
 * @param pItem: item read from a file. This is used because some fields have multiple names 
 *		(hwpad, rgb30) so we can keep the original name
 * @param fmt_item: descriptor item w/ offset and type
 * @param dd_p: pointer to PdvDependent structure to get value from
 * @param s: pointer to string to be filled in. 
 *
 * @return 0 on success, -1 on failure
 *
 */

int
pdv_dep_format_out_value(CfgFileItem *pItem, CfgFileItem *fmt_item, PdvDependent *dd_p, char *s)

{
    u_char *bp = (((u_char *) dd_p) + fmt_item->base_offset);


    /* search key/value pairs */

    if (fmt_item->possible_values)
    {

	ConfigKeyValue *pkv = fmt_item->possible_values;

	u_char found = false;

	int i = * ((int *) bp);

	while (pkv->idstr && !found)
	{
	    if (pkv->value == i)
	    {

		sprintf(s, "%s", pkv->idstr);
		found = TRUE;
		break;
	    }
	    pkv++;
	}

	if (!found)
	{
	    edt_msg(WARN, "Method for %s value %d not found\n",
		pItem->ItemName, i);
	    return -1;
	}

    }
    else
    {
	switch(fmt_item->type)
	{
	case TYPE_BIT:
	    {

		u_char mask = bit_field_mask(fmt_item);
		u_char b = (u_char) *bp;	
		sprintf(s,"%x", b >> fmt_item->bit_offset);
	    }

	    break;

	case TYPE_BYTE:
	    {   
		u_char b = *bp;

		if (fmt_item->size == sizeof(u_char))
		{
		    if (fmt_item->format_flags & CFG_FORMAT_HEX)
			sprintf(s,"%02x",  b);
		    else
			sprintf(s,"%d", b);
		}
		else
		{
		    edt_msg(WARN, "byte array not implemented out\n");
		}

	    }
	    break;

	case TYPE_USHORT:
	    {   
		u_short us = *((u_short *) bp);

		if (fmt_item->size == sizeof(u_short))
		{
		    if (fmt_item->format_flags & CFG_FORMAT_HEX)
			sprintf(s,"%04x", us);
		    else
			sprintf(s,"%d",us);
		}
		else
		{
		    edt_msg(WARN, "u_short array not implemented out\n");
		}

	    }
	    break;

	case TYPE_UINT:
	    {   
		u_int ui = *((u_int *) bp);

		if (fmt_item->size == sizeof(u_int))
		{
		    if (fmt_item->format_flags & CFG_FORMAT_HEX)
			sprintf(s,"%x", ui);
		    else
			sprintf(s,"%d",ui);
		}
		else
		{
		    edt_msg(WARN, "u_int array not implemented out\n");
		}

	    }
	    break;
	case TYPE_INT:
	    {   
		int i = *((int *) bp);

		if (fmt_item->size == sizeof(int))
		{
		    if (fmt_item->format_flags & CFG_FORMAT_HEX)
			sprintf(s,"%x", i);
		    else
			sprintf(s,"%d",i);
		}
		else
		{
		    edt_msg(WARN, "int array not implemented out\n");
		}


	    }
	    break;
	case TYPE_FLOAT:
	    {   
		float f = *((float *) bp);

		if (fmt_item->size == sizeof(float))
		{
		    sprintf(s,"%f", f);
		}
		else
		{
		    edt_msg(WARN, "float array not implemented out\n");
		}
	    }
	    break;
	case TYPE_CHAR:
	    {
		char *p = (char *) bp;

		sprintf(s, "%s", p);

	    }
	    break;

	default:

	    printf("Field %s not formatted\n", pItem->ItemName);
	    return -1;

	}
    }    

    return 0;

}


/**
 *
 * Inserts a field name into string s and calls pdv_dep_format_value
 *
 * @param pItem: item read from a file. This is used because some fields have multiple names 
 *		(hwpad, rgb30) so we can keep the original name
 * @param fmt_item: descriptor item w/ offset and type
 * @param dd_p: pointer to PdvDependent structure to get value from
 * @param s: pointer to string to be filled in. 
 *
 * @return 0 on success, -1 on failure
 *
 */


int
pdv_dep_format_out(CfgFileItem *pItem, CfgFileItem *fmt_item, PdvDependent *dd_p, char *s)

{

    sprintf(s, "%s: ", pItem->ItemName);
    return pdv_dep_format_out_value(pItem, fmt_item, dd_p, s + strlen(s));
}

/**
 * 
 * Goes through all of the fields defined for dependent structure and
 * compares two PdvDependent structures 
 *
 * @param dd_p1: pointer to first PdvDependent structure
 * @param dd_p2: pointer to second PdvDependent structure
 *
 */

void
pdv_cmp_dependent(PdvDependent *dd_p1, PdvDependent *dd_p2)

{

    int i;

    char s[512];

    CfgFileSet *pdep = pdv_get_config_fields();

    for (i=0;i<pdep->nItems;i++)
    {
	if (pdep->pItems[i].ItemName)
	    if (compare_fields(pdep->pItems+i, dd_p1, dd_p2))
	    {
		printf("Different at %s\n", pdep->pItems[i].ItemName);
		pdv_dep_format_out(pdep->pItems+i,pdep->pItems+i, dd_p1, s);
		printf("1: %s\n", s);
		pdv_dep_format_out(pdep->pItems+i,pdep->pItems+i, dd_p2, s);
		printf("2: %s\n", s);

	    }

    }
}

/**
 * 
 * Makes sure cameratype field exists by
 * combining class, model, and info fields
 * 
 * @param dd_p: pointer to PdvDependent structure
 *
 * @return 1 on success, on on failure
 */

int
pdv_cfg_resolve_cameratype(Dependent *dd_p)
{
    char tmpstr[256];

    if (!(*dd_p->cameratype))
    {
	sprintf(dd_p->cameratype, "%s%s%s%s%s", dd_p->camera_class,
	    *dd_p->camera_model? " ":"", dd_p->camera_model,
	    *dd_p->camera_info? " ":"", dd_p->camera_info);
    }

    /*
    * make sure class (manufacturer)) isn't duplicated
    */
    if ((*dd_p->cameratype && *dd_p->camera_class))
    {
	if (strncmp(dd_p->camera_class, dd_p->cameratype,
	    strlen(dd_p->camera_class)) != 0)	
	{
	    sprintf(tmpstr, "%s %s", dd_p->camera_class, dd_p->cameratype);
	    strcpy(dd_p->cameratype, tmpstr);
	}
    }

    if (*dd_p->cameratype)
	return 1;
    return 0;
}


static int pdv_cfg_parse_hex_str(char *str, char *buf)
{
    int i = 0;
    u_int ibyte;
    char *sp = str;

    while ((*(sp+2) == ' ') || (*(sp+2) == '\0'))
    {
	sscanf(sp, "%02x ", &ibyte);
	buf[i++] = (char)ibyte;
	if (*(sp+2) == '\0')
	    break;
	sp += 3;
    }
    buf[i] = '\0';
    return 1;
}

/**
* parse a hex string -- 2 digit bytes separated by spaces, put into
* char buffer
*
*
*/

CfgFileSet *
pdv_cfg_file_read(char *cfgfile, PdvDependent * dd_p,int nofs_cfg, CfgFileSet *cfg)

{
    CfgFileItem *pItems;
    CfgFileItem *fmt_item;
    CfgFileSet *depset;
    int i;

    if (!cfg)
	cfg = new_cfg_set();

    cfg->strip_quotes = TRUE;
    cfg->ignore_case = TRUE;

    cfg_set_load(cfg, cfgfile);

    pItems = cfg->pItems;

    depset = pdv_get_config_fields();

    pdv_dep_default(dd_p);

    for (i=0;i<cfg->nItems;i++)
    {
	if (pItems[i].ItemName)
	{
	    if (strncmp(pItems[i].ItemName, "xregwrite_",10) == 0)
	    {
		pdv_parse_xilinx_item(pItems + i, dd_p);

	    } 
	    else if (fmt_item = cfg_set_lookup_item(depset, pItems[i].ItemName))
	    {
		if (pdv_parse_cfg_file_input(pItems + i, fmt_item, dd_p))
		{


		}
		else
		{
		    /* parse error */

		}

	    }
	    else
	    {

		printf("Unrecognized item %s: %s\n", pItems[i].ItemName, pItems[i].ItemValue);
		printf("Original: %d <%s>\n", pItems[i].line_number, pItems[i].original_string);
	    }
	}
    }

    /* deal with various post-processing issues */

    strncpy(dd_p->cfgname, cfgfile, sizeof(dd_p->cfgname) - 1);

    if (cfg_set_lookup(cfg,"CL_DATA_PATH_NORM") ||
	cfg_set_lookup(cfg,"CL_CFG_NORM"))
	dd_p->cameralink = 1;


    pdv_cfg_resolve_cameratype(dd_p);

    {

	int check = 1;
	if (cfg_set_lookup(cfg,"serial_init_hex"))
	{
	    dd_p->serial_binit = dd_p->serial_init; 
	    dd_p->serial_format = SERIAL_BINARY;
	    check = 0;
	}
	if (cfg_set_lookup(cfg,"serial_binit"))
	{
	    dd_p->serial_binit = dd_p->serial_init; 
	    dd_p->serial_format = SERIAL_BINARY;
	    check = 0;
	}
	if (cfg_set_lookup(cfg,"serial_init_baslerf"))
	{
	    dd_p->serial_format = SERIAL_BASLER_FRAMING;
	    check = 0;
	}
	if (cfg_set_lookup(cfg,"serial_init_duncanf"))
	{
	    dd_p->serial_format = SERIAL_DUNCAN_FRAMING;
	    check = 0;
	}

	if (dd_p->serial_init[0] && check)
	{
	    if (strlen(dd_p->serial_init) < sizeof(dd_p->serial_init)-1 &&
		dd_p->serial_init[strlen(dd_p->serial_init)-1] != ':')
		strcat(dd_p->serial_init,":");
	}

    }

    if (dd_p->serial_term[0] == '<')
    {
	char temp[32];
	strcpy(temp, dd_p->serial_term+1);
	memset(dd_p->serial_term,0, sizeof(dd_p->serial_term));
	if (temp[strlen(temp)-1] == '>')
	    temp[strlen(temp)-1] = '\0';
	pdv_cfg_parse_hex_str(temp, dd_p->serial_term);
    }



    return cfg;

}

/**
 *
 * Go through all differences between default and PdvDependent structure dd_p descibed by
 * config file cfg_s and add any fields to cfg_s which weren't there. Also
 * mark any fields in cfg_s which are at default values with status field = 4
 *
 */

int
pdv_cfg_mark_nondefault(PdvDependent *dd_p, CfgFileSet *cfg_s)

{
    CfgFileItem * pItem;
    CfgFileItem *fmt_item;
    int n_non_default = 0;

    int i;

    char s[512];

    CfgFileSet *pdep = pdv_get_config_fields();

    for (i=0;i<cfg_s->nItems;i++)
    {
	pItem = cfg_s->pItems + i;
	if (pItem->ItemName)
	{
	    pItem->status = CFG_STATUS_UNKNOWN;
	}
    }

    for (i=0;i<pdep->nItems;i++)
    {
	fmt_item = pdep->pItems + i;

	if (!(fmt_item->format_flags & CFG_FORMAT_INONLY))
	{
	    /* see if two dep structures are different */

	    if (compare_fields(fmt_item, dd_p, &default_dep))
	    {
		/* format value */

		pdv_dep_format_out_value(fmt_item,fmt_item, dd_p, s);

		pItem = NULL;

		if (fmt_item->group_number)
		{
		    CfgFileItem *pTest = fmt_item;

		    do 
		    {
			pItem = cfg_set_lookup_item(cfg_s, pTest->ItemName);
			if (pTest->group_next)
			{
			    pTest = pdep->pItems + pTest->group_next;
			}
		    } while (pTest != NULL && pItem == NULL);

		}
		else
		    pItem = cfg_set_lookup_item(cfg_s, fmt_item->ItemName);

		if (pItem)
		{
		    cfg_item_set_value(pItem, s);

		    pItem->status = CFG_STATUS_MODIFIED;

		}
		else
		{
		    pItem = cfg_set_add_item(cfg_s, fmt_item->ItemName,s, NULL);
		    if (pItem)
			pItem->status = CFG_STATUS_NEW | CFG_STATUS_MODIFIED;
		}

		n_non_default++;

	    }
	    else
	    {
		/* check to see if field is in cfg_s, mark as default value */

		if (fmt_item->ItemName)
		{
		    pItem = cfg_set_lookup_item(cfg_s, fmt_item->ItemName);

		    if (pItem)
		    {
			pItem->status = CFG_STATUS_DEFAULT;
		    }
		}
	    }
	}
    }

    for (i=0;i<cfg_s->nItems;i++)
    {
	pItem = cfg_s->pItems + i;
	if (pItem->status & CFG_STATUS_UNKNOWN)
	{
	    edt_msg(WARN, "Unknown field %s: %s\n", pItem->ItemName, pItem->ItemValue);
	}
    }

    return n_non_default;

}

// for debug

static int print_tag = 1;

void
pdv_cfg_write_file(CfgFileSet *cfg, FILE *f, unsigned int exclude_mask)

{

    int i;

    for (i=0;i<cfg->nItems;i++)
    {
	CfgFileItem *pItem = cfg->pItems + i;

	if (!(pItem->status & exclude_mask))
	{
	    if (print_tag)
	    {
		if (pItem->status & CFG_STATUS_NEW)
		{
		    fprintf(f,"# new\n");
		}
		else if (pItem->status & CFG_STATUS_UNKNOWN)
		{
		    fprintf(f,"# unknown\n");
		}
		else if (pItem->status & CFG_STATUS_MODIFIED)
		{
		    //fprintf(f," ");
		}
		else if (pItem->status & CFG_STATUS_DEFAULT)
		{
		    fprintf(f,"# default\n ");
		}
	    }
		
	    cfg_set_write_line(cfg, f, i);
	    
	}
    }
}

int
pdv_cfg_write(CfgFileSet *cfg, const char *fname, unsigned int exclude_mask)

{
    FILE *f;

    f = fopen(fname, "wt+");

    if (f)
    {
	pdv_cfg_write_file(cfg, f, exclude_mask);
	
	fclose(f);

	return 0;

    }
    else
	return -1;

}

