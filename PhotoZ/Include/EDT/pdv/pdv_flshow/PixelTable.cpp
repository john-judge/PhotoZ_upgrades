#include <FL/Fl.H>


#include <FL/Fl_Tile.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>


#include <stdio.h>

#include "PixelTable.h"

PixelTable::PixelTable(int X, int Y, int W, int H, const char*L) : Fl_Group(X,Y,W,H,L) 

{  

    m_image = NULL;
    int maxval = 0x10000;
   
    char buf[16];

    sprintf(buf,"%d",maxval);

    cell_width = 0;
    cell_height = 0;

    fl_measure(buf, cell_width, cell_height);

    cell_width += 2;
    cell_height += 2;

    columns = W / cell_width;
    rows = H / cell_height;
    
    cells = new Fl_Box **[rows];

    int rowy = Y;
    // Create widgets
    for ( int row=0; row<rows; row++ ) 
    {
	cells[row] = new Fl_Box *[columns];

	int colx = X;

        for ( int column =0; column<columns; column++ ) 
	{
            Fl_Box *box = new Fl_Box(colx,rowy,cell_width,cell_height,"     ");

            if (row == 0 || column == 0)
	    {
		box->box(FL_BORDER_BOX);
		box->labelcolor(FL_BLUE);
	    }
	    else
	    {
		box->box(FL_ENGRAVED_BOX);
		box->color(FL_WHITE);
	    }
	    
	    box->align(FL_ALIGN_CENTER);

            cells[row][column] = box;

	    colx += cell_width;
        
        }

        rowy += cell_height;
    }
    end();

    pixel_x = 0;
    pixel_y = 0;

    last_pixel_x = -1;
    last_pixel_y = -1;

    set_hex(false);

    channel = 0;
    show_all_colors = false;
    firsttime = true;


}

void PixelTable::update()

{
    int endcol   = columns - 1;
    int endrow   = rows - 1;

    int start_x = pixel_x - columns/2 + 1;
    int end_x   = start_x + columns - 2;
    int start_y = pixel_y - rows/2 + 1;
    int end_y   = start_y + rows - 2;

    int col;
    int row;

    int px;
    int py;

    if (!m_image)
	return;
    
    char buf[16];

    if (last_pixel_x != pixel_x)
    {
	/* fill in column header */
	for (col = 1, px = start_x; 	col < columns; col++, px++)
	{
	    if (px >= 0 && px <= xmax)
		sprintf(buf,"%d",px);
	    else
		buf[0] = 0;

	    cells[0][col]->copy_label(buf);
	}

	last_pixel_x = pixel_x;
    }

     if (last_pixel_y != pixel_y)
    {
	/* fill in row labels*/
	for (row = 1, py = start_y; 	row < rows; row++, py++)
	{
	    if (py >= 0 && py <= ymax)
		sprintf(buf,"%d",py);
	    else
		buf[0] = 0;

	    cells[row][0]->copy_label(buf);
	}

	last_pixel_y = pixel_y;
    }

    py = start_y;

    for (row = 1;row < rows;row ++, py++)
    {

	px = start_x;

	for (col = 1; col < columns; col++, px++)
	{
	    if (px >= 0 && px <= xmax &&
		py >= 0 && py <= ymax)

	    {
		int Value[4];

		int n = m_image->GetPixelV(px, py, Value);

		if (n == 1 || channel < 0 || channel >= n)
		{
		    sprintf(buf,pixelformat, Value[0]);
		}
		else 
		{
		    sprintf(buf,pixelformat, Value[channel]);
		}
	    }
	    else
		buf[0] = 0;

	    cells[row][col]->copy_label(buf);

	}
	

    }

}

void PixelTable::update(const int X, const int Y)

{
    pixel_x = X;
    pixel_y = Y;

    if (X < 0)
	pixel_x = 0;
    else if (X > xmax)
	pixel_x = xmax;

    if (Y < 0)
	pixel_y = 0;
    else if (Y > ymax)
	pixel_y = ymax;

    update();

}


void PixelTable::set_hex(bool state)

{

    use_hex = state;
    if (use_hex)
	strcpy(pixelformat, "%X");
    else
	strcpy(pixelformat, "%d");
}

void PixelTable::set_box_labelcolor(Fl_Color c)

{
    int col;
    int row;

    for (row = 1;row < rows;row ++)
    {
	for (col = 1; col < columns; col++)
	{

	    cells[row][col]->labelcolor(c);

	}
	

    }


}


void PixelTable::set_channel(const int chan)

{
    
    if (chan != channel || firsttime)
    {
	channel = chan;
	    
	Fl_Color c = FL_BLACK;

	if (nchannels > 1)
	{
	    switch(channel)
	    {
	        case 0:
		    c = FL_DARK_RED;
		break;
	        case 1:
		    c = FL_DARK_GREEN;
		break;
	        case 2:
		    c = FL_DARK_BLUE;
		break;

	    }
	}

	set_box_labelcolor(c);

	firsttime =  false;
    }

}

