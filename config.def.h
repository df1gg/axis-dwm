/*
 * dwm configuration file
 * Prepared for production release on Reddit
 */

#include "movestack.c"
#include <X11/XF86keysym.h>

/* ========== Appearance Settings ========== */
static const unsigned int borderpx   = 2;   // Window border width
static const unsigned int snap        = 10;  // Snap pixel threshold
static const unsigned int gappih      = 10;  // Horizontal inner gap between windows
static const unsigned int gappiv      = 10;  // Vertical inner gap between windows
static const unsigned int gappoh      = 10;  // Horizontal outer gap between windows and screen edge
static const unsigned int gappov      = 10;  // Vertical outer gap between windows and screen edge
static const int          smartgaps   = 0;   // 1: no outer gap when only one window, 0: always enable outer gaps
static const int          showbar     = 1;   // 1: display status bar, 0: hide it
static const int          topbar      = 1;   // 1: top bar, 0: bottom bar
static const int          horizpadbar = 15;  // Horizontal padding inside the status bar
static const int          vertpadbar  = 10;  // Vertical padding inside the status bar
static const int          vertpad     = 10;  // Vertical padding for UI elements
static const int          sidepad     = 10;  // Horizontal padding for UI elements
static const char *       fonts[]     = {"JetBrainsMono Nerd Font:size=11"};
static const char         dmenufont[] = "JetBrainsMono Nerd Font:size=11";

/* ========== Color Schemes ========== */
enum {
    SchemeNorm,
    SchemeSel,     // Selected window
    SchemeLayout,  // Layout indicator
    SchemeTag1,
    SchemeTag2,
    SchemeTag3,
    SchemeTag4,
    SchemeTag5,
    SchemeTag6,
    SchemeTag7,
    SchemeTag8,
    SchemeTag9,
    SchemeTagEmpty, // Empty tag
    SchemeLast
};

/* Color definitions (hex) */
static const char black[]         = "#1e2122";
static const char white[]         = "#c7b89d";
static const char gray2[]         = "#282b2c"; // Inactive window border
static const char gray3[]         = "#5d6061";
static const char gray4[]         = "#282b2c";
static const char blue[]          = "#6f8faf"; // Active window border
static const char green[]         = "#89b482";
static const char red[]           = "#ec6b64";
static const char orange[]        = "#d6b676";
static const char yellow[]        = "#d1b171";
static const char pink[]          = "#cc7f94";
static const char col_borderbar[] = "#1e2122"; // Inner border color for bars

/* Color table: fg, bg, border */
static const char *colors[][3] = {
    [SchemeNorm]     = { gray3, black, gray2 },
    [SchemeSel]      = { gray4, blue,  blue  },
    [SchemeLayout]   = { "#85ae61", black, black },
    [SchemeTag1]     = { "#6f8faf", black, black },
    [SchemeTag2]     = { "#ec6b64", black, black },
    [SchemeTag3]     = { "#d6b676", black, black },
    [SchemeTag4]     = { "#85ae61", black, black },
    [SchemeTag5]     = { "#6f8faf", black, black },
    [SchemeTag6]     = { "#ec6b64", black, black },
    [SchemeTag7]     = { "#d6b676", black, black },
    [SchemeTag8]     = { "#85ae61", black, black },
    [SchemeTag9]     = { "#6f8faf", black, black },
    [SchemeTagEmpty] = { gray3, black, gray2 },
};

/* ========== Tags ========== */
static const char *tags[] = {
    "", // Tag1: Editor/Code
    "", // Tag2: Terminal/Build
    "", // Tag3: Browser/Docs
    "", // Tag4: System/Infra
    "", // Tag5: AI/ML
    "", // Tag6: Notes
    "", // Tag7: Chat
    "", // Tag8: Media
    ""  // Tag9: Sandbox
};

/* Underline settings for tags */
static const unsigned int ulinepad     = 7;  // Horizontal padding between underline and tag text
static const unsigned int ulinestroke  = 2;  // Underline thickness
static const unsigned int ulinevoffset = 0;  // Vertical offset from bottom of bar
static const int          ulineall     = 0;  // 1: underline all tags, 0: only active tag

/* ========== Window Rules ========== */
static const Rule rules[] = {
    /* class              instance  title  tags mask  isfloating  monitor */
    { "TelegramDesktop", NULL,     NULL, 1 << 6,     0,          -1 },
    { "firefox",         NULL,     NULL, 1 << 2,     0,          -1 },
    { "obsidian",        NULL,     NULL, 1 << 5,     0,          -1 },
};

/* ========== Layouts ========== */
static const float mfact     = 0.55; // Master area size factor
static const int   nmaster   = 1;    // Number of clients in master area
static const int   resizehints = 1;  // Respect size hints in tiled resizals
static const int   lockfullscreen = 1; // Lock focus on fullscreen windows

static const Layout layouts[] = {
    /* symbol  arrange function */
    { "[]=", tile },    // Tiled layout
    { "><>", NULL },    // Floating layout
    { "[M]", monocle },  // Monocle layout (fullscreen)
};

/* ========== Key Definitions ========== */
#define MODKEY Mod4Mask  // Super/Windows key

/* Helper for tag keybindings */
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view, { .ui = 1 << TAG } }, \
    { MODKEY|ControlMask,           KEY,      toggleview, { .ui = 1 << TAG } }, \
    { MODKEY|ShiftMask,             KEY,      tag, { .ui = 1 << TAG } }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag, { .ui = 1 << TAG } },

/* Helper for spawning shell commands */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* ========== Command Definitions ========== */
static char dmenumon[2] = "0"; /* dmenu monitor */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", black, "-nf", gray3, "-sb", blue, "-sf", black, NULL };
static const char *termcmd[]  = { "st", NULL };

/* Volume controls via pamixer */
static const char *volupcmd[]   = { "pamixer", "--allow-boost", "--increase", "5", NULL };
static const char *voldowncmd[] = { "pamixer", "--allow-boost", "--decrease", "5", NULL };
static const char *volmutecmd[] = { "pamixer", "--toggle-mute", NULL };

/* Brightness controls via brightnessctl */
static const char *brightupcmd[]   = { "brightnessctl", "set", "5%+", NULL };
static const char *brightdowncmd[] = { "brightnessctl", "set", "5%-", NULL };

/* Screenshots via flameshot */
static const char *flameshotfullcmd[] = { "flameshot", "gui", NULL };
static const char *flameshotsavecmd[] = { "sh", "-c", "flameshot full -p ~/Pictures/Screenshots", NULL };

/* Screen lock */
static const char *lockcmd[] = { "slock", NULL };

/* Application shortcuts */
static const char *fmtermcmd[]  = { "st", "-e", "lf", NULL };
static const char *browsercmd[] = { "firefox", NULL };
static const char *obsidiancmd[] = { "obsidian", NULL };

/* ========== Key Bindings ========== */
static const Key keys[] = {
    /* Launchers */
    { MODKEY,           XK_p,      spawn,          { .v = dmenucmd } },
    { MODKEY|ShiftMask, XK_Return, spawn,          { .v = termcmd } },
    { MODKEY,           XK_b,      spawn,          { .v = browsercmd } },
    { MODKEY,           XK_e,      spawn,          { .v = fmtermcmd } },
    { MODKEY,           XK_n,      spawn,          { .v = obsidiancmd } },

    /* Toggle status bar */
    { MODKEY|ShiftMask, XK_b,      togglebar,      { 0 } },

    /* Window navigation */
    { MODKEY,           XK_j,      focusstack,     { .i = +1 } },
    { MODKEY,           XK_k,      focusstack,     { .i = -1 } },
    { MODKEY,           XK_i,      incnmaster,     { .i = +1 } },
    { MODKEY,           XK_d,      incnmaster,     { .i = -1 } },
    { MODKEY,           XK_h,      setmfact,       { .f = -0.05 } },
    { MODKEY,           XK_l,      setmfact,       { .f = +0.05 } },

    /* Gaps controls (vanitygaps patch) */
    { MODKEY,           XK_g,      incrgaps,       { .i = +5 } },
    { MODKEY|ShiftMask, XK_g,      incrgaps,       { .i = -5 } },
    { MODKEY|ControlMask, XK_g,    incrogaps,      { .i = +5 } },
    { MODKEY|ControlMask|ShiftMask, XK_g, incrogaps,  { .i = -5 } },
    { MODKEY|Mod1Mask,  XK_g,      incrigaps,      { .i = +5 } },
    { MODKEY|Mod1Mask|ShiftMask, XK_g, incrigaps,   { .i = -5 } },
    { MODKEY,           XK_y,      incrihgaps,     { .i = +3 } },
    { MODKEY|ShiftMask, XK_y,      incrihgaps,     { .i = -3 } },
    { MODKEY,           XK_u,      incrivgaps,     { .i = +3 } },
    { MODKEY|ShiftMask, XK_u,      incrivgaps,     { .i = -3 } },
    { MODKEY|ControlMask, XK_y,    incrohgaps,     { .i = +3 } },
    { MODKEY|ControlMask|ShiftMask, XK_y, incrohgaps,  { .i = -3 } },
    { MODKEY|ControlMask, XK_u,    incrovgaps,     { .i = +3 } },
    { MODKEY|ControlMask|ShiftMask, XK_u, incrovgaps,  { .i = -3 } },
    { MODKEY,           XK_minus,  togglegaps,     { 0 } },
    { MODKEY,           XK_equal,  defaultgaps,    { 0 } },

    /* Window actions */
    { MODKEY,           XK_Return, zoom,           { 0 } },
    { MODKEY,           XK_Tab,    view,           { 0 } },
    { MODKEY|ShiftMask, XK_c,      killclient,     { 0 } },
    { MODKEY|ShiftMask, XK_j,      movestack,      { .i = +1 } },
    { MODKEY|ShiftMask, XK_k,      movestack,      { .i = -1 } },

    /* Layouts */
    { MODKEY,           XK_t,      setlayout,      { .v = &layouts[0] } },
    { MODKEY,           XK_f,      setlayout,      { .v = &layouts[1] } },
    { MODKEY,           XK_m,      setlayout,      { .v = &layouts[2] } },
    { MODKEY,           XK_space,  setlayout,      { 0 } },
    { MODKEY|ShiftMask, XK_space,  togglefloating, { 0 } },

    /* Tags navigation */
    { MODKEY,           XK_0,      view,           { .ui = ~0 } },
    { MODKEY|ShiftMask, XK_0,      tag,            { .ui = ~0 } },
    { MODKEY,           XK_comma,  focusmon,       { .i = -1 } },
    { MODKEY,           XK_period, focusmon,       { .i = +1 } },
    { MODKEY|ShiftMask, XK_comma,  tagmon,         { .i = -1 } },
    { MODKEY|ShiftMask, XK_period, tagmon,         { .i = +1 } },
    TAGKEYS(XK_1,0) TAGKEYS(XK_2,1) TAGKEYS(XK_3,2) TAGKEYS(XK_4,3)
    TAGKEYS(XK_5,4) TAGKEYS(XK_6,5) TAGKEYS(XK_7,6) TAGKEYS(XK_8,7)
    TAGKEYS(XK_9,8)

    /* Audio controls (XF86 keys) */
    { 0, XF86XK_AudioRaiseVolume, spawn, { .v = volupcmd } },
    { 0, XF86XK_AudioLowerVolume, spawn, { .v = voldowncmd } },
    { 0, XF86XK_AudioMute,        spawn, { .v = volmutecmd } },

    /* Screen brightness (XF86 keys) */
    { 0, XF86XK_MonBrightnessUp,   spawn, { .v = brightupcmd } },
    { 0, XF86XK_MonBrightnessDown, spawn, { .v = brightdowncmd } },

    /* Screenshots */
    { MODKEY,           XK_Print,  spawn, { .v = flameshotfullcmd } },
    { MODKEY|ShiftMask, XK_Print,  spawn, { .v = flameshotsavecmd } },

    /* Lock screen */
    { MODKEY|ControlMask, XK_l,    spawn, { .v = lockcmd } },

    /* Quit dwm */
    { MODKEY|ShiftMask, XK_q,      quit,   { 0 } },
};

/* ========== Mouse Button Bindings ========== */
static const Button buttons[] = {
    /* Click                Mask       Button        Function        Argument */
    { ClkLtSymbol,  0, Button1, setlayout,      { 0 } },
    { ClkLtSymbol,  0, Button3, setlayout,      { .v = &layouts[2] } },
    { ClkStatusText,0, Button2, spawn,           { .v = termcmd } },
    { ClkClientWin, MODKEY, Button1, movemouse,    { 0 } },
    { ClkClientWin, MODKEY, Button2, togglefloating,{ 0 } },
    { ClkClientWin, MODKEY, Button3, resizemouse,  { 0 } },
    { ClkTagBar,    0, Button1, view,            { 0 } },
    { ClkTagBar,    0, Button3, toggleview,      { 0 } },
    { ClkTagBar,    MODKEY, Button1, tag,         { 0 } },
    { ClkTagBar,    MODKEY, Button3, toggletag,   { 0 } },
};

