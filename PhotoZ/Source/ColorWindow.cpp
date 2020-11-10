//=============================================================================
// ColorWindow.cpp
//=============================================================================
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <FL/fl.h>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.h>
#include <FL/fl_draw.h>

#include "Definitions.h"
#include "ColorWindow.h"
#include "DataArray.h"
#include "UserInterface.h"
#include "WindowExporter.h"
#include "DapController.h"
#include "Color.h"
#include "Data.h"

char* i2txt(int);
extern char txtBuf[];

//=============================================================================
ColorWindow::ColorWindow(int x,int y,int w,int h,const char* label=0)
:Fl_Gl_Window(x,y,w,h,label)
{
	mapFlag=1;
	mapMode=1;
	movieMode=AmpNor2ArrayMax;
	blackBGFlag=1;

	contourFlag=1;
	colorControlMode='B';
	upperBound=1;
	lowerBound=0;
	colorAmp=1;
	colorCenter=0;

	//============================
	// Texture
	textureIndex=0;
	makeStripeImage();
	makeContourImage();

	plane[0]=0.0;
	plane[1]=0.0;
	plane[2]=2.0;
	plane[3]=-0.5;

	//============================
	// Movie
	makeMovieFlag=0;

	currentPt=160;
	startPt=160;
	endPt=200;
	stopFlag=1;
	speed=1/10.0;
	repeatFlag=0;
}

//=============================================================================
void ColorWindow::setPointXYZ()
{
	int i;
	int xx,yy;

	for(i=0;i<Num_Diodes-8;i++)
	{
		xx=aw->diodeX[i]+13;
		yy=(h()-aw->diodeY[i])-16;

		point[i].setXYZ(float(xx),float(yy),0);
	}
}

//=============================================================================
void ColorWindow::setStripe(unsigned char* stripe,int i,
							unsigned char r,unsigned char g,unsigned char b)
{
	stripe[4*i] = (GLubyte) r;
	stripe[4*i+1] = (GLubyte) g;
	stripe[4*i+2] = (GLubyte) b;
	stripe[4*i+3] = (GLubyte) 255;
}

//=============================================================================
void ColorWindow::makeStripeImage()
{
	unsigned char r,g,b;
	int i;
	unsigned char* stripe;

	stripe=stripeImage[0];
	for(i=0;i<stripeLength;i++)
	{
		colorControl->setColor1(double(i-1)/(stripeLength-3));
		colorControl->getRGB(&r,&g,&b);
		setStripe(stripe,i,r,g,b);
	}

	stripe=stripeImage[1];
	for(i=0;i<stripeLength;i++)
	{
		colorControl->setColor2(double(i-1)/(stripeLength-3));
		colorControl->getRGB(&r,&g,&b);
		setStripe(stripe,i,r,g,b);
	}

	stripe=stripeImage[2];
	for(i=0;i<stripeLength;i++)
	{
		colorControl->setBlueRedColor(double(i-2)/(stripeLength-5));
		colorControl->getRGB(&r,&g,&b);
		setStripe(stripe,i,r,g,b);
	}

	stripe=stripeImage[3];
	for(i=0;i<stripeLength;i++)
	{
		colorControl->setGrayScale(double(i-1)/(stripeLength-3));
		colorControl->getRGB(&r,&g,&b);
		setStripe(stripe,i,r,g,b);
	}
}

//=============================================================================
void ColorWindow::makeContourImage()
{
	int i;
	unsigned char r,g,b;
	unsigned char* stripe;

	// 1024=16*64

	// Contour 1
	stripe=contourImage[0];

	colorControl->setColor1(-0.1);		// 0
	colorControl->getRGB(&r,&g,&b);
	setStripe(stripe,0,r,g,b);

	colorControl->setColor1(1.1);		// 1
	colorControl->getRGB(&r,&g,&b);
	setStripe(stripe,stripeLength-1,r,g,b);

	for(i=1;i<stripeLength-1;i++)
	{
		colorControl->setColor1((int(i/64)+0.5)*64.0/(stripeLength-2));
		colorControl->getRGB(&r,&g,&b);
		setStripe(stripe,i,r,g,b);
	}

	// Contour 2
	stripe=contourImage[1];

	colorControl->setColor2(-0.1);		// 0
	colorControl->getRGB(&r,&g,&b);
	setStripe(stripe,0,r,g,b);

	colorControl->setColor2(1.1);		// 1
	colorControl->getRGB(&r,&g,&b);
	setStripe(stripe,stripeLength-1,r,g,b);

	for(i=1;i<stripeLength-1;i++)
	{
		colorControl->setColor2((int(i/64)+0.5)*64.0/(stripeLength-2));
		colorControl->getRGB(&r,&g,&b);
		setStripe(stripe,i,r,g,b);
	}

	// Contour 3
	stripe=contourImage[2];

	colorControl->setBlueRedColor(-0.1);	// 0
	colorControl->getRGB(&r,&g,&b);
	setStripe(stripe,0,r,g,b);

	colorControl->setBlueRedColor(1.1);		// 1
	colorControl->getRGB(&r,&g,&b);
	setStripe(stripe,stripeLength-1,r,g,b);

	for(i=1;i<stripeLength-1;i++)
	{
		colorControl->setBlueRedColor((int(i/64)+0.5)*64.0/(stripeLength-2));
		colorControl->getRGB(&r,&g,&b);
		setStripe(stripe,i,r,g,b);
	}

	// Contour 4
	stripe=contourImage[3];

	colorControl->setGrayScale(-0.1);		// 0
	colorControl->getRGB(&r,&g,&b);
	setStripe(stripe,0,r,g,b);

	colorControl->setGrayScale(1.1);		// 1
	colorControl->getRGB(&r,&g,&b);
	setStripe(stripe,stripeLength-1,r,g,b);

	for(i=1;i<stripeLength-1;i++)
	{
		colorControl->setGrayScale((int(i/64)+0.5)*64.0/(stripeLength-2));
		colorControl->getRGB(&r,&g,&b);
		setStripe(stripe,i,r,g,b);
	}

}

//=============================================================================
void ColorWindow::setTextureIndex(int index)
{
	textureIndex=index;
}

//=============================================================================
void ColorWindow::setContourFlag(char input)
{
	contourFlag=input;
}

//=============================================================================
void ColorWindow::setMovieMode(int input)
{
	movieMode=input;
}

//=============================================================================
void ColorWindow::draw()
{
	if (!valid()) 
	{
		init();
		reshape();
	}

	display();
	drawScale();
}

//=============================================================================
void ColorWindow::init()
{
	// Background Color
	if(blackBGFlag)
	{
		glClearColor (0,0,0,0);
	}
	else
	{
		glClearColor (255,255,255,0);
	}

	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glGenTextures(1,&textureHandle);
	glBindTexture(GL_TEXTURE_1D,textureHandle);

	glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
	glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );

	// Texture Management
	if(contourFlag)
	{
		glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,stripeLength,0,GL_RGBA,
			GL_UNSIGNED_BYTE,contourImage[textureIndex]);
	}
	else
	{
		glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,stripeLength,0,GL_RGBA,
			GL_UNSIGNED_BYTE,stripeImage[textureIndex]);
	}

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);

	// Color Scale Control
	if(colorControlMode=='B')
	{
		double height=upperBound-lowerBound;
		if(height==0)
		{
			height=0.01;
		}
		plane[2]=1/height;
		plane[3]=-lowerBound/height;
	}
	else if(colorControlMode=='C')
	{
		plane[2]=1/(2*colorAmp);
		plane[3]=-colorCenter/(2*colorAmp)+0.5;
	}
	glTexGenfv(GL_S,GL_OBJECT_PLANE,plane);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_1D);
}

//=============================================================================
void ColorWindow::reshape()
{
	glViewport(0,0,w(),h());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0,w(),0,h(),-10.0,10.0);
	glMatrixMode(GL_MODELVIEW);
}

//=============================================================================
void ColorWindow::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	setZ();
	build3DModel();
}

//=============================================================================
void ColorWindow::drawScale()
{
	float z1=-0.01f;
	float z2=1.01f;

	// Color Bar
	glBegin(GL_QUADS);
	{
		double gain=plane[2];
		double offset=plane[3];
		
		glVertex3f(9,9,(z1-offset)/gain);
		glVertex3f(31,9,(z1-offset)/gain);
		glVertex3f(31,112,(z2-offset)/gain);
		glVertex3f(9,112,(z2-offset)/gain);
	}
	glEnd();


	// Outline
	glDisable(GL_TEXTURE_1D);
	glColor3f (0.5,0.5,0.5);

	glBegin (GL_LINES);
		glVertex3f (9,8,2);			//|
		glVertex3f (9,112,2);

		glVertex3f (31,8,2);		//|
		glVertex3f (31,112,2);

		glVertex3f (9,112,2);		//-
		glVertex3f (32,112,2);

		glVertex3f (9,110,2);			//--
		glVertex3f (35,110,2);

		glVertex3f (9,60,2);		//--
		glVertex3f (35,60,2);

		glVertex3f (9,10,2);			//--
		glVertex3f (35,10,2);

		glVertex3f (9,8,2);			//-
		glVertex3f (31,8,2);
	glEnd();

	glEnable(GL_TEXTURE_1D);
}

//=============================================================================
void ColorWindow::build3DModel()
{
	int i,j,k,i0,w;
	//
	glBegin(GL_TRIANGLE_STRIP);
		k=12;
		i0=0;
		w=12;
		for(i=0;i<w;i++)
		{
			glVertex3f(point[i0+i+k].x,point[i0+i+k].y,point[i0+i+k].z);
			glVertex3f(point[i0+i].x,point[i0+i].y,point[i0+i].z);
		}
		glVertex3f(point[i0+i+k].x,point[i0+i+k].y,point[i0+i+k].z);
	glEnd();
	//
	k=13;
	w++;
	i0--;
	for(j=0;j<10;j++)
	{
		glBegin(GL_TRIANGLE_STRIP);
			i0=i0+w;
			k++;
			w++;
			for(i=0;i<w;i++)
			{
				glVertex3f(point[i0+i+k].x,point[i0+i+k].y,point[i0+i+k].z);
				glVertex3f(point[i0+i].x,point[i0+i].y,point[i0+i].z);
			}
			glVertex3f(point[i0+i+k].x,point[i0+i+k].y,point[i0+i+k].z);
		glEnd();
	}
	//
	glBegin(GL_TRIANGLE_STRIP);
		i0=197;
		k=24;
		w=23;
		for(i=0;i<w;i++)
		{
			glVertex3f(point[i0+i].x,point[i0+i].y,point[i0+i].z);
			glVertex3f(point[i0+i+k].x,point[i0+i+k].y,point[i0+i+k].z);
		}
		glVertex3f(point[i0+i].x,point[i0+i].y,point[i0+i].z);
	glEnd();
	//
	glBegin(GL_TRIANGLE_STRIP);
		i0=221;
		k=23;
		w=23;
		for(i=0;i<w;i++)
		{
			glVertex3f(point[i0+i+k].x,point[i0+i+k].y,point[i0+i+k].z);
			glVertex3f(point[i0+i].x,point[i0+i].y,point[i0+i].z);
		}
		glVertex3f(point[i0+i+k].x,point[i0+i+k].y,point[i0+i+k].z);
	glEnd();
	//
	k=25;
	w=24;
	i0-=2;
	for(j=0;j<10;j++)
	{
		glBegin(GL_TRIANGLE_STRIP);
			i0=i0+w+1;
			k--;
			w--;
			for(i=0;i<w;i++)
			{
				glVertex3f(point[i0+i].x,point[i0+i].y,point[i0+i].z);
				glVertex3f(point[i0+i+k].x,point[i0+i+k].y,point[i0+i+k].z);
			}
			glVertex3f(point[i0+i].x,point[i0+i].y,point[i0+i].z);
		glEnd();
	}
	//
	glBegin(GL_TRIANGLE_STRIP);
		i0=440;
		k=13;
		w=11;
		for(i=0;i<w;i++)
		{
			glVertex3f(point[i0+i].x,point[i0+i].y,point[i0+i].z);
			glVertex3f(point[i0+i+k].x,point[i0+i+k].y,point[i0+i+k].z);
		}
		glVertex3f(point[i0+i].x,point[i0+i].y,point[i0+i].z);
	glEnd();
	//
	glBegin(GL_TRIANGLES);
		int d;
		d=11;
		glVertex3f(point[d].x,point[d].y,point[d].z);
		d=24;
		glVertex3f(point[d].x,point[d].y,point[d].z);
		d=25;
		glVertex3f(point[d].x,point[d].y,point[d].z);

		d=220;
		glVertex3f(point[d].x,point[d].y,point[d].z);
		d=243;
		glVertex3f(point[d].x,point[d].y,point[d].z);
		d=267;
		glVertex3f(point[d].x,point[d].y,point[d].z);

		d=452;
		glVertex3f(point[d].x,point[d].y,point[d].z);
		d=451;
		glVertex3f(point[d].x,point[d].y,point[d].z);
		d=463;
		glVertex3f(point[d].x,point[d].y,point[d].z);

		d=453;
		glVertex3f(point[d].x,point[d].y,point[d].z);
		d=440;
		glVertex3f(point[d].x,point[d].y,point[d].z);
		d=439;
		glVertex3f(point[d].x,point[d].y,point[d].z);

		d=244;
		glVertex3f(point[d].x,point[d].y,point[d].z);
		d=221;
		glVertex3f(point[d].x,point[d].y,point[d].z);
		d=197;
		glVertex3f(point[d].x,point[d].y,point[d].z);

	glEnd();
}

//=============================================================================
void ColorWindow::setZ()
{
	if(mapFlag)
	{
		setMapZ();
	}
	else
	{
		setMovieZ();
	}
}

//=============================================================================
void ColorWindow::setMapZ()
{
	if(mapMode==CW_RLI)
	{
		dataArray->setRli2DataFeature();
	}
	else if(mapMode==CW_Max_Amp)
	{
		dataArray->setMaxAmp2DataFeature();
	}
	else if(mapMode==CW_Spike_Amp)
	{
		dataArray->setSpikeAmp2DataFeature();
	}
	else if(mapMode==CW_Per_Amp_Latency)
	{
		dataArray->setHalfAmpLatencyRatio2DataFeature();
	}
	else if(mapMode==CW_Max_Amp_Latency)
	{
		dataArray->setMaxAmpLatency2DataFeature();
	}

	//==================================
	// Set Z Value
	int i;

	for(i=0;i<464;i++)
	{
		point[i].setZ(dataArray->getDataFeature(i)*0.999);
	}
}

//=============================================================================
void ColorWindow::setMovieZ()
{
	if(movieMode==AmpNor2ArrayMax)
	{
		setZ_AmpNor2ArrayMax();
	}
	else if(movieMode==AmpNor2DiodeMax)
	{
		setZ_AmpNor2DiodeMax();
	}
}

//=============================================================================
void ColorWindow::setZ_AmpNor2ArrayMax()
{
	int i;

	// Check Max Amp in the Array
	double maxAmp=0;

	for(i=0;i<Num_Diodes-8;i++)
	{
		if(!dataArray->getIgnoredFlag(i))
		{
			if(dataArray->getDataMaxAmp(i) > maxAmp)
			{
				maxAmp=dataArray->getDataMaxAmp(i);
			}
		}
	}

	// Set Z Value
	maxAmp*=1.01;

	for(i=0;i<464;i++)
	{
		point[i].setZ(float(dataArray->getAmp(i,currentPt)/maxAmp));
	}
}

//=============================================================================
void ColorWindow::setZ_AmpNor2DiodeMax()
{
	int i;

	for(i=0;i<464;i++)
	{
		point[i].setZ(float(dataArray->getAmp(i,currentPt)
				/dataArray->getDataMaxAmp(i))*0.999);
	}
}

//=============================================================================
void ColorWindow::setColorControlMode(char input)
{
	colorControlMode=input;
}

//=============================================================================
void ColorWindow::setColorUpperBound(double input)
{
	upperBound=input;
	sprintf_s(txtBuf, 32," % .2f",input);
	ui->cwColorUpperBound->value(txtBuf);
}

//=============================================================================
void ColorWindow::setColorLowerBound(double input)
{
	lowerBound=input;
	sprintf_s(txtBuf, 32," % .2f",input);
	ui->cwColorLowerBound->value(txtBuf);
}

//=============================================================================
void ColorWindow::setColorAmp(double input)
{
	colorAmp=input;
	sprintf_s(txtBuf, 32," % .2f",input);
	ui->cwColorAmp->value(txtBuf);
}

//=============================================================================
void ColorWindow::setColorCenter(double input)
{
	colorCenter=input;
	sprintf_s(txtBuf, 32," % .2f",input);
	ui->cwColorCenter->value(txtBuf);
}

//=============================================================================
void ColorWindow::setMapMode(int input)
{
	mapMode=input;
}

//=============================================================================
void ColorWindow::setMapFlag(char input)
{
	mapFlag=input;
}

//=============================================================================
void ColorWindow::setBlackBGFlag(char input)
{
	blackBGFlag=input;
}

//=============================================================================
