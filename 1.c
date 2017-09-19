#include	<stdio.h>
#include	<math.h>
#include	<X11/Xlib.h>

typedef	struct{
	int	wid;
	int	hei;
	Display	*dsp;
	int	scn;
	Window	win;
	GC	gc;
	unsigned long	cdp;
	XColor	clr;
	Colormap	cmp;
}Table;

typedef	struct{
	double	x;
	char	D;
	double	u;
	double	P[2];
	int	R;
	int	G;
	int	B;
}Ant;

static	Ant	lgtsin(Ant	ant)
{
	Ant	ant1=ant;
	double	a=ant.u*0.03;
	ant1.x=0.5+0.5*sin(ant.u*2*M_PI*ant.x-ant.u*M_PI/4);
	if(ant.x<0.25)			{ant1.D='A';ant1.P[1]=ant.P[1]-a;}
	if(ant.x>=0.25&&ant.x<0.5)	{ant1.D='B';ant1.P[0]=ant.P[0]-a;}
	if(ant.x>=0.5&&ant.x<0.75)	{ant1.D='C';ant1.P[0]=ant.P[0]+a;}
	if(ant.x>=0.75)			{ant1.D='D';ant1.P[1]=ant.P[1]+a;}
	return	ant1;
}

static	Ant	lgt_sin(Ant	ant)
{
	Ant	ant1=ant;
	double	a=ant.u*0.03;
	ant1.x=0.5-0.5*sin(ant.u*2*M_PI*ant.x+ant.u*M_PI/8);
	if(ant.x<0.25)			{ant1.D='A';ant1.P[1]=ant.P[1]-a;}
	if(ant.x>=0.25&&ant.x<0.5)	{ant1.D='B';ant1.P[0]=ant.P[0]-a;}
	if(ant.x>=0.5&&ant.x<0.75)	{ant1.D='C';ant1.P[0]=ant.P[0]+a;}
	if(ant.x>=0.75)			{ant1.D='D';ant1.P[1]=ant.P[1]+a;}
	return	ant1;
}

static	Ant	lgtcos(Ant	ant)
{
	Ant	ant1=ant;
	double	a=ant.u*0.03;
	ant1.x=0.5+0.5*cos(ant.u*2*M_PI*ant.x);
	if(ant.x<0.25)			{ant1.D='A';ant1.P[1]=ant.P[1]-a;}
	if(ant.x>=0.25&&ant.x<0.5)	{ant1.D='B';ant1.P[0]=ant.P[0]-a;}
	if(ant.x>=0.5&&ant.x<0.75)	{ant1.D='C';ant1.P[0]=ant.P[0]+a;}
	if(ant.x>=0.75)			{ant1.D='D';ant1.P[1]=ant.P[1]+a;}
	return	ant1;
}

static	void	setclr(Table table,int R,int G,int B)
{
	table.clr.red=R;
	table.clr.green=G;
	table.clr.blue=B;
	table.clr.flags=DoRed|DoGreen|DoBlue;
	table.cmp=DefaultColormap(table.dsp,DefaultScreen(table.dsp));
	XAllocColor(table.dsp,table.cmp,&table.clr);
	XSetForeground(table.dsp,table.gc,table.clr.pixel);
}

static	void	drwant(Ant	ant,Table	table)
{
	if((ant.P[0]<=0))	ant.P[0]=table.wid+ant.P[0];
	if((ant.P[1]<=0))	ant.P[1]=table.hei+ant.P[1];
	if((ant.P[0]>=table.wid))	ant.P[0]=ant.P[0]-table.wid;
	if((ant.P[1]>=table.hei))	ant.P[1]=ant.P[1]-table.hei;
	setclr(table,ant.R,ant.G,ant.B);
	XDrawPoint(table.dsp,table.win,table.gc,ant.P[0],ant.P[1]);
}

int	main()
{
	Table	table={0};
	table.dsp=XOpenDisplay(NULL);
	table.scn=DefaultScreen(table.dsp);
	table.wid=500;
	table.hei=500;
	table.win=XCreateSimpleWindow(
			table.dsp,
			DefaultRootWindow(table.dsp),
			0,0,
			table.wid,table.hei,
			1,
			BlackPixel(table.dsp,table.scn),
			WhitePixel(table.dsp,table.scn)
			);
	XMapWindow(table.dsp,table.win);
	table.gc=XCreateGC(table.dsp,table.win,0,NULL);
	table.cdp=DefaultDepth(table.dsp,DefaultScreen(table.dsp));
	Pixmap	StartUp=XCreatePixmap(table.dsp,table.win,30,40,table.cdp);
	long	EvtMask;
	XEvent	evt;
	Ant	ant1={0};
	ant1.x=0.5;
	ant1.u=1;
	ant1.R=0x00fff;
	ant1.G=0xeffff;
	ant1.B=0x00fff;
	Ant	ant2={0};
	ant2.x=0.25;
	ant2.u=1;
	ant2.R=0x00fff;
	ant2.G=0x00fff;
	ant2.B=0xeffff;
	Ant	ant3={0};
	ant3.x=0.75;
	ant3.u=1;
	ant3.R=0xeffff;
	ant3.G=0x00fff;
	ant3.B=0x00fff;
	while(1)
	{
		EvtMask=ExposureMask
//			|KeyPressMask
//			|KeyReleaseMask
//			|ButtonPressMask
//			|ButtonReleaseMask
			|PointerMotionMask
			|StructureNotifyMask
			;
		XSelectInput(table.dsp,table.win,EvtMask);
		XNextEvent(table.dsp,&evt);
		switch(evt.type)
			{
				case	ConfigureNotify:
					{
						table.wid=evt.xconfigure.width;
						table.hei=evt.xconfigure.height;
						break;
					}
				case	Expose:
					{
						setclr(table,0x00ff,0x00ff,0x00ff);
						XFillRectangle(table.dsp,table.win,table.gc,0,0,table.wid,table.hei);
						ant1.P[0]=table.wid*0.5;
						ant1.P[1]=table.hei*0.5;
						drwant(ant1,table);
						ant2.P[0]=table.wid*0.5;
						ant2.P[1]=table.hei*0.5;
						drwant(ant2,table);
						ant3.P[0]=table.wid*0.5;
						ant3.P[1]=table.hei*0.5;
						drwant(ant3,table);
						break;
					}
				case	MotionNotify:
					{
						ant1.u=(sqrt((evt.xmotion.x-ant1.P[0])*(evt.xmotion.x-ant1.P[0])
								+(evt.xmotion.y-ant1.P[1])*(evt.xmotion.y-ant1.P[1])));
						ant2.u=(sqrt((ant1.P[0]-ant2.P[0])*(ant1.P[0]-ant2.P[0])
								+(ant1.P[1]-ant2.P[1])*(ant1.P[1]-ant2.P[1])));
						ant3.u=(sqrt((ant2.P[0]-ant3.P[0])*(ant2.P[0]-ant3.P[0])
								+(ant2.P[1]-ant3.P[1])*(ant2.P[1]-ant3.P[1])));
						break;
					}
				default:
					{
						ant1=lgtsin(ant1);
						drwant(ant1,table);
						printf("1:%f %f %c %f %f ",ant1.x,ant1.u,ant1.D,ant1.P[0],ant1.P[1]);
						ant2=lgtcos(ant2);
						drwant(ant2,table);
						printf("2:%f %f %c %f %f ",ant2.x,ant2.u,ant2.D,ant2.P[0],ant2.P[1]);
						ant3=lgt_sin(ant3);
						drwant(ant3,table);
						printf("3:%f %f %c %f %f\n",ant3.x,ant3.u,ant3.D,ant3.P[0],ant3.P[1]);
						break;
					}
			}
		ant1=lgtsin(ant1);
		drwant(ant1,table);
		printf("1:%f %f %c %f %f ",ant1.x,ant1.u,ant1.D,ant1.P[0],ant1.P[1]);
		ant2=lgtcos(ant2);
		drwant(ant2,table);
		printf("2:%f %f %c %f %f ",ant2.x,ant2.u,ant2.D,ant2.P[0],ant2.P[1]);
		ant3=lgt_sin(ant3);
		drwant(ant3,table);
		printf("3:%f %f %c %f %f\n",ant3.x,ant3.u,ant3.D,ant3.P[0],ant3.P[1]);
	}
	return	0;
}

