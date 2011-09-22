/*
 * PSpage.h
 *
 *  Created on: 28 Feb 2011
 *      Author: b0wter
 */

#ifndef PSPAGE_H_
#define PSPAGE_H_


#include<stdio.h>
#include "Utils.h"

typedef double Doub;
typedef int Int;
typedef Vector2d VecDoub;
/*
 *
 */
namespace Utilities
{

class PSpage
{
public:
	static FILE *PLT;
	static char *file;
	char fontname[128];
	Doub fontsize;

	PSpage(char *filename);
	PSpage();
	virtual ~PSpage();
	void setfont(char *fontnam, Doub size);
	void setcolor(Int r, Int g, Int b);
	void setdash(char *patt, Int phase=0);
	void setlinewidth(Doub w);
	void setgray(Doub w);
	void gsave();
	void grestore();
	void rawps(char *text);
	void addtext(char *text);
	void puttext(char *text, Doub x, Doub y, Doub rot=0.0);
	void putctext(char *text, Doub x, Doub y, Doub rot=0.0);
	void putrtext(char *text, Doub x, Doub y, Doub rot=0.0);
	void close();
	void display();
	void lineseg(Doub xs, Doub ys, Doub xf, Doub yf);
	void polyline(VecDoub &x, VecDoub &y, Int close=0, Int fill=0, Int clip=0);

private:

};

}

#endif /* PSPAGE_H_ */
