//=============================================================================
// Definitions.h
//=============================================================================
#ifndef _Definitions_h
#define _Definitions_h
//=============================================================================

// Global Objects
extern class UserInterface *ui;
extern class ArrayWindow *aw;
extern class TraceWindow *tw;
extern class RecControl *recControl;

extern class DapController *dc;
extern class DapController *dapControl;

extern class Camera *camera;

extern class LiveFeed *lf;
extern class DataArray *dataArray;
extern class SignalProcessor *sp;
extern class ColorWindow *cw;
extern class DiodeArray *diodeArray;
extern class DapWindow *dw;
extern class FileController *fileController;
extern class WindowExporter *we;
extern class Color* colorControl;

char* i2txt(int);
char* d2txt(double);
char* d2txt(double value,int digit);
char* f2txt(float);
//=============================================================================
#endif
//=============================================================================
