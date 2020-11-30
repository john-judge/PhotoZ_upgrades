//=============================================================================
// ImageEncoder.cpp: implementation of the ImageEncoder class.
//		Replaces Paintlib dependency for writing image files from bmp.
//=============================================================================

#include <stdlib.h>
#include <string>
#include <iostream>

#include <FL/Fl.H>
#include <FL/Fl_File_Chooser.H>

/*
#include <paintlib/plstdpch.h>		// Must have one for paintlib
#include <paintlib/planydec.h>
#include <paintlib/plwinbmp.h>
#include <paintlib/plbmpenc.h>
#include <paintlib/pljpegenc.h>
#include <paintlib/plpngenc.h>
#include <paintlib/pltiffenc.h>
*/
#include "ImageEncoder.h"


using namespace std;

//=============================================================================

ImageEncoder::ImageEncoder() {

}

ImageEncoder::~ImageEncoder() {

}

void saveJpegFromBmp(const char* fileName, HBITMAP *hbm) {

}

void savePngFromBmp(const char* fileName, HBITMAP *hbm) {

}

void saveTiffFromBmp(const char* fileName, HBITMAP *hbm) {

}
void saveBmp(const char* fileName, HBITMAP *hbm) {

}