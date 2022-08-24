// generated by Fast Light User Interface Designer (fluid) version 1.0106

#ifndef CameraConfigUI_h
#define CameraConfigUI_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <string.h>
#include "CameraConfigEditor.h"
#include <FL/Fl_File_Input.H>
extern void config_file_in_cb(Fl_File_Input*, void*);
#include <FL/Fl_Button.H>
extern void cfg_edit_load_cb(Fl_Button*, void*);
extern void cfg_edit_save_cb(Fl_Button*, void*);
#include <FL/Fl_Input.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Check_Button.H>
extern void cfg_edit_cl_flag_cb(Fl_Check_Button*, void*);
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Value_Slider.H>
extern void cfg_edit_extdepth_slider_cb(Fl_Value_Slider*, void*);
extern void cfg_edit_depth_slider_cb(Fl_Value_Slider*, void*);
#include <FL/Fl_Counter.H>
extern void cfg_edit_taps_counter_cb(Fl_Counter*, void*);
#include <FL/Fl_Value_Input.H>
extern void cfg_edit_width_cb(Fl_Value_Input*, void*);
#include <FL/Fl_Slider.H>
extern void cfg_edit_width_slider_cb(Fl_Slider*, void*);
extern void cfg_edit_height_cb(Fl_Value_Input*, void*);
extern void cfg_edit_height_slider_cb(Fl_Slider*, void*);
extern void cfg_edit_hskip_cb(Fl_Value_Input*, void*);
extern void cfg_edit_hskip_slider_cb(Fl_Slider*, void*);
extern void cfg_edit_vskip_cb(Fl_Value_Input*, void*);
extern void cfg_edit_vskip_slider_cb(Fl_Slider*, void*);
extern void cfg_edit_hactv_cb(Fl_Value_Input*, void*);
extern void cfg_edit_hactv_slider_cb(Fl_Slider*, void*);
extern void cfg_edit_vactv_cb(Fl_Value_Input*, void*);
extern void cfg_edit_vactv_slider_cb(Fl_Slider*, void*);
extern void cfg_edit_hblank_cb(Fl_Value_Input*, void*);
extern void cfg_edit_hblank_slider_cb(Fl_Slider*, void*);
extern void cfg_edit_vblank_cb(Fl_Value_Input*, void*);
extern void cfg_edit_vblank_slider_cb(Fl_Slider*, void*);
extern void cfg_edit_roi_enable_cb(Fl_Check_Button*, void*);
extern void cfg_edit_tap_orientation_cb(Fl_Button*, void*);
#include <FL/Fl_Round_Button.H>
extern void cfg_edit_baud_cb(Fl_Round_Button*, void*);
extern void cfg_edit_intlv_method_cb(Fl_Button*, void*);
extern void cfg_edit_intlv_bayer_cb(Fl_Button*, void*);
extern void cfg_edit_intlv_taps_counter_cb(Fl_Counter*, void*);
extern void cfg_edit_cls_flags_cb(Fl_Check_Button*, void*);
#include <FL/Fl_Box.H>
extern void cfg_edit_full_sample_timing(Fl_Button*, void*);
extern void cfg_edit_sample_timing(Fl_Button*, void*);
#include <FL/Fl_Choice.H>
extern void cfg_edit_pdv_unit_cb(Fl_Choice*, void*);
extern void cfg_edit_pdv_channel_cb(Fl_Choice*, void*);
extern void cfg_edit_sim_unit_cb(Fl_Choice*, void*);
extern void cfg_edit_apply_fg_cb(Fl_Button*, void*);
extern void cfg_edit_apply_cls_cb(Fl_Button*, void*);
extern void cfg_edit_show_cfg_cb(Fl_Button*, void*);

class CameraConfigUI {
public:
  CameraConfigUI(CameraConfigEditor *owner, CameraConfig *p, const char *path);
  Fl_Double_Window *CameraConfigWindow;
  Fl_File_Input *cfgname;
  Fl_Button *load_btn;
  Fl_Button *save_btn;
  Fl_Input *camera_class;
  Fl_Input *camera_model;
  Fl_Input *camera_info;
  Fl_Tabs *config_tabs;
  Fl_Group *main_folder;
  Fl_Output *cl_data_path_norm;
  Fl_Output *cl_cfg_norm;
  Fl_Group *CameraLinkFlags;
  Fl_Check_Button *cl_flag_RGB;
  Fl_Check_Button *cl_flag_IgnoreDV;
  Fl_Check_Button *cl_flag_LineScan;
  Fl_Check_Button *cl_flag_DisableROI;
  Fl_Check_Button *cl_flag_InvertDV;
  Fl_Check_Button *camera_link;
  Fl_Input *bitfile;
  Fl_Group *txt_win;
  Fl_Text_Editor *InfoWindow;
  Fl_Value_Slider *extdepth_slider;
  Fl_Value_Slider *depth_slider;
  Fl_Counter *taps_counter;
  Fl_Value_Input *width;
  Fl_Slider *widthslider;
  Fl_Value_Input *height;
  Fl_Slider *heightslider;
  Fl_Value_Input *hskip;
  Fl_Slider *hskipslider;
  Fl_Value_Input *vskip;
  Fl_Slider *vskipslider;
  Fl_Value_Input *hactv;
  Fl_Slider *hactvslider;
  Fl_Value_Input *vactv;
  Fl_Slider *vactvslider;
  Fl_Value_Input *hblank;
  Fl_Slider *hblankslider;
  Fl_Value_Input *vblank;
  Fl_Slider *vblankslider;
  Fl_Check_Button *Roi_Enable;
  Fl_Counter *taps_counter2;
  Fl_Group *TapOrientation;
  Fl_Input *shutter_speed_min;
  Fl_Input *shutter_speed_max;
  Fl_Input *shutter_speed_def;
  Fl_Input *offset_min;
  Fl_Input *offset_max;
  Fl_Input *offset_def;
  Fl_Input *gain_min;
  Fl_Input *gain_max;
  Fl_Input *gain_def;
  Fl_Input *SerialInitInput;
  Fl_Group *SerialBaud;
  Fl_Round_Button *baud_9600;
  Fl_Round_Button *baud_19200;
  Fl_Round_Button *baud_38400;
  Fl_Round_Button *baud_57600;
  Fl_Round_Button *baud_115200;
  Fl_Group *interlace_grp;
  Fl_Tabs *InterleaveTabs;
  Fl_Group *NoIntlvOptions;
  Fl_Group *LineIntlvOptions;
  Fl_Group *BayerFilterOptions;
  Fl_Group *GenericIntlvOptions;
  Fl_Group *InterleaveMethodButtons;
  Fl_Counter *intlv_taps;
  Fl_Counter *taps_counter3;
  Fl_Group *TapOrientation2;
  Fl_Value_Input *pixel_clock_input;
  Fl_Group *CLS_Flags;
  Fl_Check_Button *cls_flag_linescan;
  Fl_Check_Button *cls_flag_LVCONT;
  Fl_Check_Button *cls_flag_RVEN;
  Fl_Check_Button *cls_flag_UARTLOOP;
  Fl_Check_Button *cls_flag_SMALLOK;
  Fl_Check_Button *cls_flag_INTLVEN;
  Fl_Check_Button *cls_flag_FIRSTFC;
  Fl_Check_Button *cls_flag_DATACNT;
  Fl_Check_Button *cls_flag_LED;
  Fl_Check_Button *cls_flag_TRIGSRC;
  Fl_Check_Button *cls_flag_TRIGPOL;
  Fl_Check_Button *cls_flag_TRIGFRAME;
  Fl_Check_Button *cls_flag_TRIGLINE;
  Fl_Value_Input *FillAInput;
  Fl_Value_Input *FillBInput;
  Fl_Group *GeometryFrame;
  Fl_Box *FrameBox;
  Fl_Box *HBlankBox;
  Fl_Box *CameraBox;
  Fl_Group *RoiImageGroup;
  Fl_Box *RoiBOX;
  Fl_Group *RoiXYLabelHolder;
  Fl_Box *ROIXYLabel;
  Fl_Group *RoiWidthLabelHolder;
  Fl_Box *ROIWidthLabel;
  Fl_Group *RoiHeightLabelHolder;
  Fl_Box *ROIHeightLabel;
  Fl_Group *HeightLabelHolder;
  Fl_Box *HeightLabel;
  Fl_Group *WidthLabelHolder;
  Fl_Box *WidthLabel;
  Fl_Group *VBlankLabelHolder;
  Fl_Box *VBlankLabel;
  Fl_Group *HBlankLabelHolder;
  Fl_Box *HBlankLabel;
  Fl_Button *SampleFullButton;
  Fl_Output *StatusOutput;
  Fl_Button *SampleXYButton;
  Fl_Choice *PdvUnitChoice;
  static Fl_Menu_Item menu_PdvUnitChoice[];
  Fl_Choice *PdvChannelChoice;
  static Fl_Menu_Item menu_PdvChannelChoice[];
  Fl_Choice *SimUnitChoice;
  static Fl_Menu_Item menu_SimUnitChoice[];
  Fl_Button *ApplyFGButton;
  Fl_Button *ApplySimButton;
  Fl_Box *ModifiedTag;
  void show();
  ~CameraConfigUI();
  CameraConfigEditor *pE;
};
#endif