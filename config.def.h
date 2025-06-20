/* appearance */
static const unsigned int borderpx = 2; // граница окна
static const unsigned int snap = 10;    // прилипаемость окон
static const int showbar = 1;           // показывать статусбар
static const int topbar = 1;            // бар сверху
static const int vertpad = 10;          /* vertical padding of bar */
static const int sidepad = 10;          /* horizontal padding of bar */
static const char *fonts[] = {"JetBrainsMono Nerd Font:size=11"};
static const char dmenufont[] = "JetBrainsMono Nerd Font:size=11";

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
};

/* tagging */
static const char *tags[] = {" ", " ", " ", " ", " ",
                             " ", " ", " ", " "};

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
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY | ShiftMask, XK_q, quit, {0}},
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
