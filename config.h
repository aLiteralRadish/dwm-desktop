/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx 	= 20;
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh			= 4;		/* 2 is the default spacing around the bar's font */
static const char *fonts[]          = { "FiraCode Mono:pixelsize=20:antialias=true:autohint=true" };
static const char dmenufont[]       = "FiraCode Mono:antialias=true:autohint=true";
static const char d_fg[]      	= "#868d80";
static const char d_bg[]		= "#3c474d";
static const char d_border[]	= "#3c474d";
static const char s_fg[]		= "#3c474d";
static const char s_bg[]    	= "#83b6af";
static const char s_border[]	= "#83b6af";
static const char *colors[][3]	= {
	/*               fg         bg			border   */
	[SchemeNorm] = { d_fg,		d_bg,		d_border },
	[SchemeSel]  = { s_fg,		s_bg,		s_border },
};

/* tagging */
static const char *tags[] = { "󰈹 ", "", "󰙯 ", " ", "󰎱", "󰎳", "󰎶", "󰎹", "󰎼" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "firefox",  NULL,       NULL,       1 << 0,       0,           -1 },
	{ "kitty",	  NULL,		  NULL,		  1 << 1,		0,			 -1 },
	{ "discord",  NULL,		  NULL,		  1 << 2,		0,			 -1 },
	{ "steam",	  NULL,		  NULL,		  1 << 3,		0,			 -1 },
	{ "mpv",	  NULL,		  NULL,		  1 << 4,		0,			 -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */

#define FORCE_VSPLIT 1
#include "nrowgrid.c"
#include "fibonacci.c"
#include "movestack.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "󰋁",      nrowgrid},    /* no layout function means floating behavior */
	{ "󰊓",      monocle },
	{ "",		centeredmaster },
	{ "󱪴",		dwindle },
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#include <X11/XF86keysym.h>
/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = 	{ "dmenu_run", "-g", "3", "-l", "5", "-c", "-m", dmenumon, "-fn", dmenufont, "-nb", d_bg, "-nf", d_fg, "-sb", s_bg, "-sf", s_fg, NULL };
static const char *termcmd[]  = 	{ "kitty", "/home/radish", NULL };
static const char *screenshot[] =	{ "/home/radish/.scripts/ss.sh", NULL };
static const char *screenshot_selection[] =	{ "/home/radish/.scripts/ss-sel.sh", NULL };
static const char *killcmd[] = 		{ "/home/radish/.scripts/kill.sh", d_bg, d_fg, s_bg, s_fg, NULL };
static const char *volupcmd[] =		{ "/home/radish/.scripts/vol.sh", "+5%", NULL };
static const char *voldowncmd[] =	{ "/home/radish/.scripts/vol.sh", "-5%", NULL };
static const char *volmutecmd[] =	{ "/home/radish/.scripts/volmute.sh", NULL };
static const char *vpntoggle[] =    { "/home/radish/.scripts/toggle-vpn.sh" };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_k,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      incnmaster,     {.i = -1 } },
	{ MODKEY|ControlMask,			XK_k,	   movestack,      {.i = -1 } },
	{ MODKEY|ControlMask,			XK_j,	   movestack,      {.i = +1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,						XK_t,	   setlayout,	   {.v = &layouts[0]} },
	{ MODKEY,						XK_g,	   setlayout,	   {.v = &layouts[1]} },
	{ MODKEY,					    XK_m,	   setlayout,	   {.v = &layouts[2]} },
	{ MODKEY,						XK_c,	   setlayout,	   {.v = &layouts[3]} },
	{ MODKEY,						XK_d,	   setlayout,	   {.v = &layouts[4]} },
	{ 0, XF86XK_AudioMute, 					   spawn, 		   {.v = volmutecmd} },
	{ 0, XF86XK_AudioLowerVolume, 			   spawn, 		   {.v = voldowncmd} },
	{ 0, XF86XK_AudioRaiseVolume, 			   spawn, 		   {.v = volupcmd} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,         				XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,						XK_s,	   spawn,		   {.v = screenshot_selection} },
	{ MODKEY|ShiftMask,				XK_s,	   spawn,		   {.v = screenshot} },
	{ MODKEY|ShiftMask,				XK_q,	   spawn,		   {.v = killcmd} },
	{ 0, XF86XK_Display,					   spawn,		   {.v = vpntoggle} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

