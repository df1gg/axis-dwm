#include "movestack.c"

/* appearance */
static const unsigned int borderpx = 2; // граница окна
static const unsigned int snap = 10;    // прилипаемость окон
static const unsigned int gappih = 10;  /* horiz inner gap between windows */
static const unsigned int gappiv = 10;  /* vert inner gap between windows */
static const unsigned int gappoh =
    10; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov =
    10; /* vert outer gap between windows and screen edge */
static const int smartgaps =
    0; /* 1 means no outer gap when there is only one window */
static const int showbar = 1;  // показывать статусбар
static const int topbar = 1;   // бар сверху
static const int vertpad = 10; /* vertical padding of bar */
static const int sidepad = 10; /* horizontal padding of bar */
static const char *fonts[] = {"JetBrainsMono Nerd Font:size=11"};
static const char dmenufont[] = "JetBrainsMono Nerd Font:size=11";

enum {
  SchemeNorm,
  SchemeSel, // стандартные схемы
  SchemeTag1,
  SchemeTag2,
  SchemeTag3,
  SchemeTag4,
  SchemeTag5,
  SchemeTag6,
  SchemeTag7,
  SchemeTag8,
  SchemeTag9,
  SchemeTagEmpty,
  SchemeLast
};

/* Colors */
static const char black[] = "#1E1D2D";
static const char gray2[] = "#282737"; // unfocused window border
static const char gray3[] = "#585767";
static const char gray4[] = "#282737";
static const char blue[] = "#96CDFB"; // focused window border
static const char green[] = "#ABE9B3";
static const char red[] = "#F28FAD";
static const char orange[] = "#F8BD96";
static const char yellow[] = "#FAE3B0";
static const char pink[] = "#d5aeea";
static const char col_borderbar[] = "#1E1D2D"; // inner border
static const char white[] = "#f8f8f2";

static const char *colors[][3] = {
    /*             fg       bg      border */
    [SchemeNorm] = {gray3, black, gray2},
    [SchemeSel] = {gray4, blue, blue},
    [SchemeTag1] = {"#8aadf4", black, black},
    [SchemeTag2] = {"#f4b8e4", black, black},
    [SchemeTag3] = {"#f9e2af", black, black},
    [SchemeTag4] = {"#a6e3a1", black, black},
    [SchemeTag5] = {"#89b4fa", black, black},
    [SchemeTag6] = {"#f5c2e7", black, black},
    [SchemeTag7] = {"#94e2d5", black, black},
    [SchemeTag8] = {"#fab387", black, black},
    [SchemeTag9] = {"#cdd6f4", black, black},
    [SchemeTagEmpty] = {gray3, black, gray2},
};

/* tagging */
static const char *tags[] = {"", "", "", "", "",
                             "", "", "", ""};

static const unsigned int ulinepad =
    4; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke =
    2; /* thickness / height of the underline */
static const unsigned int ulinevoffset =
    0; /* how far above the bottom of the bar the line should appear */
static const int ulineall =
    0; /* 1 to show underline on all tags, 0 for just the active ones */

/* rules */
static const Rule rules[] = {
    {"Gimp", NULL, NULL, 0, 1, -1},
    {"Firefox", NULL, NULL, 1 << 1, 0, -1},
    {"discord", NULL, NULL, 1 << 7, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55;
static const int nmaster = 1;
static const int resizehints = 1;
static const int lockfullscreen = 1;

static const Layout layouts[] = {
    {"[]=", tile},
    {"><>", NULL},
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask // WIN/Super
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] = "0";
static const char *dmenucmd[] = {"dmenu_run", "-m",  dmenumon, "-fn", dmenufont,
                                 "-nb",       black, "-nf",    gray3, "-sb",
                                 blue,        "-sf", black,    NULL};
static const char *termcmd[] = {"alacritty", NULL};

/* keybindings */
static const Key keys[] = {
    /* --- запуск приложений --- */
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},

    /* --- внешний вид --- */
    {MODKEY, XK_b, togglebar, {0}},

    /* --- навигация по окнам --- */
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},

    /* --- управление gaps (vanitygaps) --- */
    {MODKEY, XK_g, incrgaps, {.i = +5}},                // увеличить все отступы
    {MODKEY | ShiftMask, XK_g, incrgaps, {.i = -5}},    // уменьшить все отступы
    {MODKEY | ControlMask, XK_g, incrogaps, {.i = +5}}, // внешние отступы +
    {MODKEY | ControlMask | ShiftMask,
     XK_g,
     incrogaps,
     {.i = -5}},                                     // внешние отступы -
    {MODKEY | Mod1Mask, XK_g, incrigaps, {.i = +5}}, // внутренние отступы +
    {MODKEY | Mod1Mask | ShiftMask,
     XK_g,
     incrigaps,
     {.i = -5}},                          // внутренние отступы -
    {MODKEY, XK_minus, togglegaps, {0}},  // включить/выключить gaps
    {MODKEY, XK_equal, defaultgaps, {0}}, // сбросить gaps к дефолту
    {MODKEY, XK_y, incrihgaps, {.i = +3}},
    {MODKEY | ShiftMask, XK_y, incrihgaps, {.i = -3}},
    {MODKEY, XK_u, incrivgaps, {.i = +3}},
    {MODKEY | ShiftMask, XK_u, incrivgaps, {.i = -3}},
    {MODKEY | ControlMask, XK_y, incrohgaps, {.i = +3}},
    {MODKEY | ControlMask | ShiftMask, XK_y, incrohgaps, {.i = -3}},
    {MODKEY | ControlMask, XK_u, incrovgaps, {.i = +3}},
    {MODKEY | ControlMask | ShiftMask, XK_u, incrovgaps, {.i = -3}},

    /* --- управление окнами --- */
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY | ShiftMask, XK_j, movestack, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, movestack, {.i = -1}},

    /* --- смена layout --- */
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}}, // tile
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}}, // floating
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}}, // monocle
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},

    /* --- теги --- */
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},

    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},

    /* --- привязка к тегам --- */
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8)

    /* --- выход --- */
    {MODKEY | ShiftMask, XK_q, quit, {0}},
};

/* mouse */
static const Button buttons[] = {
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
