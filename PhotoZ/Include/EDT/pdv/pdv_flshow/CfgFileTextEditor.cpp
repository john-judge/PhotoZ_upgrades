

#include "CfgFileTextEditor.h"

#include "CameraConfigEditor.h"

#include <ctype.h>
// This is pulled from the FLTK text editor example

Fl_Text_Display::Style_Table_Entry styletable[] = {     // Style table
  { FL_BLACK,      FL_HELVETICA,        FL_NORMAL_SIZE }, // A - Plain
  { FL_DARK_GREEN, FL_HELVETICA,	FL_NORMAL_SIZE }, // B - Line comments
  { FL_BLUE,       FL_HELVETICA,        FL_NORMAL_SIZE }, // C - Strings
  { FL_DARK_RED,   FL_HELVETICA,        FL_NORMAL_SIZE }, // D - Directives
  { FL_BLUE,       FL_HELVETICA_ITALIC,   FL_NORMAL_SIZE }  // E - Keywords
};

void
style_unfinished_cb(int, void*) {
}
/////////////////////////////////////////////////

class CfgTextEditWin : public Fl_Text_Editor
{
public:
    CfgTextEditWin(const int X, const int Y, const int W, const int H,
	const char *l = NULL) :	Fl_Text_Editor(X,Y,W,H,l)
    {
	tooltip_text = NULL;
	pCfg = NULL;
	pOwner = NULL;
	lastline = -1;
    }

    virtual ~CfgTextEditWin()
    
    {
    }

    virtual int handle(int event);

    int xy_to_pos(int x, int y, int PosType = CHARACTER_POS)

    {
	return xy_to_position(x,y, PosType);
    }


    char *tooltip_text;

    CameraConfig *pCfg;

    CfgFileTextEditor *pOwner;
    int lastline;
};


int CfgTextEditWin::handle(int event)

{
   if (event == FL_SHORTCUT)
    {

	int key = Fl::event_key();

	if (key == FL_F + 1)
	{
	    int pos = xy_to_pos(Fl::event_x()-x(),Fl::event_y()-y());

	    int curline = buffer()->count_lines(0,pos);

	    if (pCfg->cfg->pItems[curline].ItemName)
		pOwner->set_help(pCfg->cfg->pItems[curline].ItemName);   	    

	}
    }

#if 0
   if (event == FL_MOVE)
   {
       int pos = xy_to_pos(Fl::event_x(),Fl::event_y()-y());

	int curline = buffer()->count_lines(0,pos);

	if (pCfg && curline >= 0 && curline < pCfg->cfg->nItems && curline != lastline)
	{	    
	    if (pCfg->cfg->pItems[curline].ItemName)
		pOwner->set_help(pCfg->cfg->pItems[curline].ItemName);
	    

	    lastline = curline;
	}

   }
#endif

   return Fl_Text_Editor::handle(event);

}


void update_cb(Fl_Button *button, void *p)

{

}



CfgFileTextEditor::CfgFileTextEditor(int W, int H, const char *t) : Fl_Double_Window(W,H,t)

{
    Fl_Button * update = new Fl_Button(0,0,50,25,"Update");
    update->callback((Fl_Callback *) update_cb);
    update->user_data(this);

    pHelp = new CfgFileSet();

    cfg_set_load(pHelp, "cfgfilehelp.help");

    ed = new CfgTextEditWin(0,25,w(),h());

    ed->pOwner = this;



   
    resizable(ed);

    buf = new Fl_Text_Buffer;

    ed->buffer(buf);

    stylebuf = new Fl_Text_Buffer;

    ed->highlight_data(stylebuf, styletable,
                  sizeof(styletable) / sizeof(styletable[0]),
		'A', style_unfinished_cb, 0);

    end();

    helpframe = new Fl_Double_Window(W,300,"Config Item Help");
    //helpframe->clear_border();

    helpwin = new Fl_Text_Display(0,0,W,300);
    helpwin->buffer(new Fl_Text_Buffer);

   

}



CfgFileTextEditor::~CfgFileTextEditor()

{

}

void CfgFileTextEditor::set_help(const char *name)

{
    CfgFileItem *pItem = cfg_set_lookup_item(pHelp, name);

    if (pItem)
    {

	helpframe->show();

	helpframe->copy_label(name);
	char *s = pItem->ItemValue;

	int cf = fl_font();
	int cs = fl_size();

	fl_font(helpwin->textfont(), helpwin->textsize());
	
	int cw = fl_width('X');

	fl_font(cf,cs);

	if (cw)
	    cw = helpwin->w()/cw;
	else
	    cw = helpwin->w()/10;

	// make a formatted version of ItemValue

	char *work = (char *) malloc(2 * strlen(s)+strlen(name) + 3);
	int wp = 0;


	int ix; 
	int line = 0;
	int col = 0;
	int srccol = 0;
	int lastspace = -1;

	int startlen = strlen(s);

	sprintf(work,"%s\n\n", name);

	wp = strlen(work);

	for (ix=0;ix<startlen;ix++)
	{
	    if (col >= cw)
	    {
		if (lastspace > 0)
		    work[lastspace] = '\n';
		col = wp - lastspace;
		lastspace = -1;
	    }    

	    if (s[ix] == '\n')
		srccol = 0;

	    if (isspace(s[ix]))
		lastspace = wp;

	    if (s[ix] != '\n' || srccol < cw)
		work[wp++] = s[ix];

	    col++;
	    srccol++;


	}

	work[wp] = '\0';

    
	helpwin->buffer()->text(work);

	free(work);
	

    }



}

void CfgFileTextEditor::SetEditor(CameraConfigEditor * ED) // this will set CFG

{
    pCfgEd = ED;

    SetCfg(pCfgEd->pCfg);

}



void CfgFileTextEditor::SetCfg(CameraConfig *CFG)

{

    pCfg = CFG;
    ed->pCfg = CFG;

    modified = false;

    buf->text("");
    stylebuf->text("");

    CfgFileItem *pItem = cfg_set_FirstItem(pCfg->cfg);

    while (pItem)
    {
	char s[256]; 

	//if (pItem->ItemName || pItem->ItemValue)
	
	if (pItem->ItemName)
	{
	    sprintf(s,"%s:",pItem->ItemName);
	    buf->append(s);
	    memset(s,'C',strlen(s));

	    stylebuf->append(s);
	}

	if (pItem->ItemValue)
	{
	    sprintf(s,"\t%s",pItem->ItemValue);
	    buf->append(s);
	    memset(s,'D',strlen(s));

	    stylebuf->append(s);
	}

	if (pItem->ItemComment)
	{
	    sprintf(s,"# %s",pItem->ItemComment);
	    buf->append(s);
	    memset(s,'B',strlen(s));

	    stylebuf->append(s);
	}

	buf->append("\n");
	stylebuf->append("A");

	pItem = cfg_set_NextItem(pCfg->cfg);
    }     
}

void
CfgFileTextEditor::style_init(void) 

{
    char *style = new char[buf->length() + 1];

    memset(style, 'A', buf->length());
    style[buf->length()] = '\0';
    if (!stylebuf) 
	stylebuf = new Fl_Text_Buffer(buf->length());

    stylebuf->text(style);

    parse_colors(0, buf->length());

    delete[] style;

}

void CfgFileTextEditor::parse_colors(int start, int end)

{
    // go through lines in buf

    // if first char is #, make line comment color
    
    int start_line = buf->count_lines(0,start);
    int lines = buf->count_lines(start, end);
    int pos = buf->line_start(start);
    char ch;

    char *line_text = new char[100];
    int  allocated_size = 100;
    int line;

    for (line = start_line; line <= start_line + lines; line++)
    {
	if (buf->character(pos) == '#')
	{
	    ch = 'B';   
	}
	else
	    ch = 'A';
	    
	int nextpos = buf->line_end(pos);
	
	int linesize = nextpos - pos + 1;

	// substitute 

	if (linesize + 1 > allocated_size)
	{
	    delete line_text;
	    line_text = new char[linesize+1];
	    allocated_size = linesize + 1;
	}

	memset(line_text, ch, linesize);
	line_text[linesize] = '\0';

	stylebuf->replace(pos,nextpos,line_text);
	
	pos = nextpos + 1;
	int test = buf->count_lines(0,pos);

    }
}


