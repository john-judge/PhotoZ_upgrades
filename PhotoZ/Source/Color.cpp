//=============================================================================
// Color.cpp
//=============================================================================
#include <stdlib.h>

#include <FL/Fl.h>
#include <FL/Fl_Draw.h>

#include "Color.h"

//=============================================================================
void RGB::set(unsigned char rr,unsigned char gg,unsigned char bb)
{
	r=rr;
	g=gg;
	b=bb;
}

//=============================================================================
Color::Color()
{
	colorMode='1';
	controlMode='B';

	setTraceColor();
	setColorScale1();
	setColorScale2();
	setBlueRedScale();

	upperBound=1;
	lowerBound=0;
	colorAmp=1;
	colorCenter=0;
}

//=============================================================================
Color::~Color()
{
}

//=============================================================================
void Color::setTraceColor()
{
	int i;
	unsigned char r,g,b;

	// Red
	r=255;g=0;b=0;
	for(i=0;i<6;i++)
	{
		traceColor[i].set(r,g,b);
		g+=51;
		used[i]=0;
	}
	r=255;g=0;b=51;
	for(i=6;i<11;i++)
	{
		traceColor[i].set(r,g,b);
		b+=51;
		used[i]=0;
	}

	// Green
	r=0;g=255;b=0;
	for(i=11;i<17;i++)
	{
		traceColor[i].set(r,g,b);
		r+=51;
		used[i]=0;
	}
	r=0;g=255;b=51;
	for(i=17;i<22;i++)
	{
		traceColor[i].set(r,g,b);
		b+=51;
		used[i]=0;
	}

	// Blue
	r=102;g=0;b=255;
	for(i=22;i<26;i++)
	{
		traceColor[i].set(r,g,b);
		r+=51;
		used[i]=0;
	}
	r=0;g=204;b=255;
	for(i=26;i<28;i++)
	{
		traceColor[i].set(r,g,b);
		g+=51;
		used[i]=0;
	}

	// Exchange color index
	indexExchange(1,20);
	indexExchange(2,25);
	indexExchange(4,18);
	indexExchange(5,23);
	indexExchange(7,12);
	indexExchange(8,27);
	indexExchange(10,16);
	indexExchange(14,24);
}

//=============================================================================
void Color::indexExchange(int i,int j)
{
	unsigned char tmp;
	tmp=traceColor[i].r;
	traceColor[i].r=traceColor[j].r;
	traceColor[j].r=tmp;

	tmp=traceColor[i].g;
	traceColor[i].g=traceColor[j].g;
	traceColor[j].g=tmp;

	tmp=traceColor[i].b;
	traceColor[i].b=traceColor[j].b;
	traceColor[j].b=tmp;
}

//=============================================================================
void Color::setColorScale1()
{
	int i,j,k;

	i=0;
	for(j=0;j<256;i++,j++)				// Black to Blue
	{
		colorScale1[i].set(0,0,j);
	}

	for(j=1;j<256;i++,j++)				// Blue to cyan
	{
		colorScale1[i].set(0,j,255);
	}

	k=0;
	for(j=254;j>=0;i++,j--)				// Cyan to green
	{
		colorScale1[i].set(0,254-k/8,j);
		k++;
	}

	for(j=1;j<256;i++,j++)				// Green to yellow
	{
		colorScale1[i].set(j,255-k/8,0);
		k--;
	}

	for(j=254;j>=0;i++,j--)				// Yellow to red
	{
		colorScale1[i].set(255,j,0);
	}
}

//=============================================================================
void Color::setColorScale2()
{
	int i;

	for(i=0;i<1022;i++)
	{
		colorScale2[i].set(colorScale1[i+254].r,colorScale1[i+254].g,colorScale1[i+254].b);
	}
}

//=============================================================================
void Color::setBlueRedScale()
{
	int i;

	for(i=0;i<500;i++)
	{
		blueRedScale[i].set(0,0,unsigned char((499-i)/499.0*255));
	}

	for(i=500;i<1000;i++)
	{
		blueRedScale[i].set(unsigned char((i-500)/499.0*255),0,0);
	}
}

//=============================================================================
int Color::getColorIndex()
{
	int i;

	for(i=0;i<28;i++)
	{
		if(used[i]==0)
		{
			used[i]=1;
			return i;
		}
	}

	return (rand()%28);
}

//=============================================================================
void Color::releaseColorIndex(int input)
{
	used[input]=0;
}

//=============================================================================
void Color::setTraceColor(int i)
{
	i%=28;

	fl_color(traceColor[i].r,traceColor[i].g,traceColor[i].b);
}

//=============================================================================
void Color::resetUsed()
{
	for(int i=0;i<28;i++)
	{
		used[i]=0;
	}
}

//=============================================================================
void Color::setGrayScale(double input)
{
	if(input>1)
	{
		r=g=b=255;
	}
	else if(input<0)
	{
		r=g=b=0;
	}
	else
	{
		r=g=b=unsigned char(input*255);
	}

	fl_color(r,g,b);
}

//=============================================================================
void Color::setColor1(int i)
{
	if(i>1275)
	{
		r=g=b=255;
	}
	else if(i<0)
	{
		r=g=b=0;
	}
	else
	{
		r=colorScale1[i].r;
		g=colorScale1[i].g;
		b=colorScale1[i].b;
	}

	fl_color(r,g,b);
}

//=============================================================================
void Color::setColor1(double p)
{
	setColor1(int(p*1275));
}

//=============================================================================
void Color::setColor2(int i)
{
	if(i>1021)
	{
		r=g=b=255;
	}
	else if(i<0)
	{
		r=g=b=0;
	}
	else
	{
		r=colorScale2[i].r;
		g=colorScale2[i].g;
		b=colorScale2[i].b;
	}

	fl_color(r,g,b);
}

//=============================================================================
void Color::setColor2(double p)
{
	setColor2(int(p*1021));
}

//=============================================================================
void Color::setBlueRedColor(int i)
{
	if(i>999)
	{
		r=g=b=255;
	}
	else if(i<0)
	{
		r=g=b=0;
	}
	else
	{
		r=blueRedScale[i].r;
		g=blueRedScale[i].g;
		b=blueRedScale[i].b;
	}

	fl_color(r,g,b);
}

//=============================================================================
void Color::setBlueRedColor(double p)
{
	setBlueRedColor(int(p*999));
}

//=============================================================================
void Color::setColorMode(char input)
{
	colorMode=input;
}

//=============================================================================
void Color::setColorControlMode(char input)
{
	controlMode=input;
}

//=============================================================================
void Color::setColor(double input)
{
	if(controlMode=='B')		// Bounds
	{
		double height=upperBound-lowerBound;
		if(height==0)
		{
			height=0.01;
		}
		input=(input-lowerBound)/height;
	}
	else if(controlMode=='C')	// Center - Amplitude
	{
		input=(input-colorCenter)/colorAmp/2+0.5;
	}

	setScaleColor(input);
}

//=============================================================================
void Color::setScaleColor(double input)
{
	if(colorMode=='1')
	{
		setColor1(input);
	}
	else if(colorMode=='2')
	{
		setColor2(input);
	}
	else if(colorMode=='B')
	{
		setBlueRedColor(input);
	}
	else if(colorMode=='G')
	{
		setGrayScale(input);
	}
}

//=============================================================================
void Color::setColorUpperBound(double input)
{
	upperBound=input;
}

//=============================================================================
void Color::setColorLowerBound(double input)
{
	lowerBound=input;
}

//=============================================================================
void Color::setColorAmp(double input)
{
	colorAmp=input;
}

//=============================================================================
void Color::setColorCenter(double input)
{
	colorCenter=input;
}

//=============================================================================
void Color::getRGB(unsigned char* rr,unsigned char* gg,unsigned char* bb)
{
	*rr=r;
	*gg=g;
	*bb=b;
}

//=============================================================================
