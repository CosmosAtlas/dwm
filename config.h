/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 4;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { 
   "Wuncon Siji:style=Regular",
   "cozette:style=Medium:pixelsize=13",
   "WenQuanYi WenQuanYi Bitmap Song:style=Regular:pixelsize=13", 
};
static const char dmenufont[]       = "cozette:style=Medium:pixelsize=13";
static const char col_gray1[]       = "#2E3440";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#4C566A";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
    /* class      instance    title       tags mask     iscentered   isfloating   monitor  scratch key*/
	{ "Gimp",     NULL,       NULL,       0,            0,           1,           -1,      0   },
	{ "mpv",      NULL,       NULL,       0,            0,           1,           -1,      0   },
	{ "Firefox",  NULL,       NULL,       0,            0,           0,           -1,      0   },
	{ NULL,       NULL,   "scratchpad",   0,            1,           1,           -1,      's' },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
#include "tcl.c"
#define FORCE_VSPLIT 1
#include "nrowgrid.c"
#include "layouts.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[D]",    deck },  // t, f, t, u, m, g
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "",      tile },
	{ "",      bstackhoriz },
    { "|||",      tcl },
	{ "HHH",      grid },
	/* { "",      nrowgrid }, */
	/* { "",      monocle }, */
	/* { "",      spiral }, */ 
	/* { "",     dwindle }, */
	/* { "",      bstack }, */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", "-e", "tmux", NULL };

/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = {"s", "st", "-t", "scratchpad", NULL}; 

static Key keys[] = {
	/* modifier             key    function        argument */
	/* { MODKEY,               65,    spawn,          {.v = dmenucmd } },      // Space */
	{ MODKEY,               49,    togglescratch,  {.v = scratchpadcmd } },    // grave
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      { 0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      { .v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           { 0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   { .i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   { .i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   { .i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   { .i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   { .i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   { .i = 6} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      { 0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, { 0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    { 0} },
	{ ClkTagBar,            0,              Button1,        view,           { 0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     { 0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            { 0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      { 0} },
};


void
setlayoutex(const Arg *arg)
{
	setlayout(&((Arg) { .v = &layouts[arg->i] }));
}

void
viewex(const Arg *arg)
{
	view(&((Arg) { .ui = 1 << arg->ui }));
}

void
viewall(const Arg *arg)
{
	view(&((Arg){.ui = ~0}));
}

void
toggleviewex(const Arg *arg)
{
	toggleview(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagex(const Arg *arg)
{
	tag(&((Arg) { .ui = 1 << arg->ui }));
}

void
toggletagex(const Arg *arg)
{
	toggletag(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagall(const Arg *arg)
{
	tag(&((Arg){.ui = ~0}));
}

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
	/* signum           function */
	{ "focusstack",     focusstack },
	{ "setmfact",       setmfact },
	{ "togglebar",      togglebar },
	{ "incnmaster",     incnmaster },
	{ "togglefloating", togglefloating },
	{ "focusmon",       focusmon },
	{ "tagmon",         tagmon },
	{ "zoom",           zoom },
	{ "view",           view },
	{ "viewall",        viewall },
	{ "viewex",         viewex },
	{ "toggleview",     view },
	{ "toggleviewex",   toggleviewex },
	{ "tag",            tag },
	{ "tagall",         tagall },
	{ "tagex",          tagex },
	{ "toggletag",      tag },
	{ "toggletagex",    toggletagex },
	{ "killclient",     killclient },
	{ "quit",           quit },
	{ "setlayout",      setlayout },
	{ "setlayoutex",    setlayoutex },
};
