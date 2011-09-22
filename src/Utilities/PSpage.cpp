/*
 * PSpage.cpp
 *
 *  Created on: 28 Feb 2011
 *      Author: b0wter
 */

#include "PSpage.h"
#include "cstring"
#include "stdio.h"
#include "stdlib.h"
#include "algorithm"

namespace Utilities
{

FILE* PSpage::PLT = NULL;
char* PSpage::file = "";
//! Constructor. Argument is the name of the PostScript file to be created.
PSpage::PSpage(char *filename)
{
	file = new char[128];
	strcpy(file, filename);
	PLT = fopen(file,"wb");

	if(!PLT) throw("failure opening output file for plot");
	fprintf(PLT,"%%!\n/mt{moveto}def /lt{lineto}def /np{newpath}def\n");
	fprintf(PLT,"/st{stroke}def /cp{closepath}def /fi{fill}def\n");
	fprintf(PLT,"/zp {gsave /ZapfDingbats findfont exch ");
	fprintf(PLT,"scalefont setfont moveto show grestore} def\n");
	setfont("Times-Roman",12.);
	setlinewidth(0.5);
}

//! Alternative constructor, used internally to bind a PSplot to a PSpage.
PSpage::PSpage() {}

PSpage::~PSpage()
{
	if (PLT) close();
}

//! Change font from default (12 pt Times-Roman).
void PSpage::setfont(char *fontnam, Doub size)
{
	strcpy(fontname,fontnam);
	fontsize = size;
	fprintf(PSpage::PLT,"/%s findfont %g scalefont setfont\n",fontnam,size);
}

//! Change color from default (0,0,0=black). Range is 0 to 255.
void PSpage::setcolor(Int r, Int g, Int b)
{
	fprintf(PSpage::PLT,"%g %g %g setrgbcolor\n",r/255.,g/255.,b/255.);
}

//! Change line pattern from default (solid). Call with "" to reset to solid.
void PSpage::setdash(char *patt, Int phase)
{ fprintf(PSpage::PLT,"[%s] %d setdash\n",patt,phase);}

//!Change line width from default (0.5 pt).
void PSpage::setlinewidth(Doub w) {fprintf(PSpage::PLT,"%g setlinewidth\n",w);}

//! Change gray level from default (0.0=black). Range is 0 to 1.
void PSpage::setgray(Doub w) {fprintf(PSpage::PLT,"%g setgray\n",w);}

//! Do a PostScript gsave
void PSpage::gsave() {fprintf(PSpage::PLT,"gsave\n");}

//! Do a PostScript grestore. Restores settings prior to last gsave.
void PSpage::grestore()
{
	fprintf(PSpage::PLT,"grestore\n");
}

//! Emit a string to the plot file as raw PostScript.
void PSpage::rawps(char *text) {fprintf(PSpage::PLT,"%s\n",text);}

//! Plot text at current location.
void PSpage::addtext(char *text) { fprintf(PSpage::PLT,"(%s) show ",text); }

//! Plot text at page location x; y (in pts) at angle rot.
void PSpage::puttext(char *text, Doub x, Doub y, Doub rot)
{
	fprintf(PSpage::PLT,"gsave %g %g translate %g rotate 0 0 mt ",x,y,rot);
	addtext(text);
	fprintf(PSpage::PLT,"grestore \n");
}

//! Plot centered text at page location x; y (in pts) at angle rot.
void PSpage::putctext(char *text, Doub x, Doub y, Doub rot)
{
	fprintf(PSpage::PLT,"gsave %g %g translate %g rotate 0 0 mt (%s) ",x,y,rot,text);
	fprintf(PSpage::PLT,"dup stringwidth pop 2 div neg 0 rmoveto show grestore\n");
}

//! Plot right-justified text at page location x; y (in pts) at angle rot.
void PSpage::putrtext(char *text, Doub x, Doub y, Doub rot)
{
	fprintf(PSpage::PLT,"gsave %g %g translate %g rotate 0 0 mt (%s) ",x,y,rot,text);
	fprintf(PSpage::PLT,"dup stringwidth pop neg 0 rmoveto show grestore\n");
}

//! Close the plot file. Called automatically by destructor.
void PSpage::close() {fprintf(PSpage::PLT,"showpage\n"); fclose(PSpage::PLT); PSpage::PLT = NULL;}

//! Start external process to display the plot file.
void PSpage::display()
{
	char cmd[128];
	if (PSpage::PLT) close();
	strcpy(cmd,"\"/usr/bin/evince\" ");
	// Change the above line to be your PostScript viewer.
	strcat(cmd,PSpage::file);
	system(cmd);
}

//! Draw a line segment in page coordinates (pts).
void PSpage::lineseg(Doub xs, Doub ys, Doub xf, Doub yf)
{
	fprintf(PSpage::PLT,"np %g %g mt %g %g lt st\n",xs,ys,xf,yf);
}

/*! Draw connected line segments in page coordinates (pts), with options to close
 * /and/or fill the curve, or to set the curve as a clip area.
 */
void PSpage::polyline(VecDoub &x, VecDoub &y, Int close, Int fill, Int clip)
{
	Int i,n = min(x.size(),y.size());
	fprintf(PSpage::PLT,"np %g %g mt\n",x[0],y[0]);
	for (i=1;i<n;i++) fprintf(PSpage::PLT,"%g %g lt\n",x[i],y[i]);
	if (close || fill || clip) fprintf(PSpage::PLT,"cp ");
	if (fill) fprintf(PSpage::PLT,"fi\n");
	else if (clip) fprintf(PSpage::PLT,"clip\n");
	else fprintf(PSpage::PLT,"st\n");
}

} // end of namespace
