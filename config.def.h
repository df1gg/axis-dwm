#include "movestack.c"
#include <X11/XF86keysym.h>

/* ========== Параметры внешнего вида ========== */
static const unsigned int borderpx = 2; // Ширина границы окна
static const unsigned int snap = 10;    // Пиксели для прилипания окон
static const unsigned int gappih =
    10; /* Горизонтальный внутренний отступ между окнами */
static const unsigned int gappiv =
    10; /* Вертикальный внутренний отступ между окнами */
static const unsigned int gappoh =
    10; /* Горизонтальный внешний отступ между окнами и краем экрана */
static const unsigned int gappov =
    10; /* Вертикальный внешний отступ между окнами и краем экрана */
static const int smartgaps =
    0; /* 1: нет внешних отступов, если одно окно; 0: всегда есть */
static const int showbar = 1; // 1: отображать статусбар, 0: скрыть
static const int topbar = 1;  // 1: бар сверху, 0: бар снизу
static const int horizpadbar =
    15;                           /* Горизонтальные отступы внутри статусбара */
static const int vertpadbar = 10; /* Вертикальные отступы внутри статусбара */
static const int vertpad = 10;    /* Вертикальный отступ вокруг элементов */
static const int sidepad = 10;    /* Горизонтальный отступ вокруг элементов */
static const char *fonts[] = {"JetBrainsMono Nerd Font:size=11"};
static const char dmenufont[] = "JetBrainsMono Nerd Font:size=11";

/* ========== Схемы цветов ========== */
enum {
  SchemeNorm,
  SchemeSel,    // Выбранное окно
  SchemeLayout, // Схема для отображения layout
  SchemeTag1,
  SchemeTag2,
  SchemeTag3,
  SchemeTag4,
  SchemeTag5,
  SchemeTag6,
  SchemeTag7,
  SchemeTag8,
  SchemeTag9,
  SchemeTagEmpty, // Схема для пустого тега
  SchemeLast
};

/* Определение цветов (hex) */
static const char black[] = "#1e2122";
static const char white[] = "#c7b89d";
static const char gray2[] = "#282b2c"; // Рамка неактивного окна
static const char gray3[] = "#5d6061";
static const char gray4[] = "#282b2c";
static const char blue[] = "#6f8faf"; // Рамка активного окна
static const char green[] = "#89b482";
static const char red[] = "#ec6b64";
static const char orange[] = "#d6b676";
static const char yellow[] = "#d1b171";
static const char pink[] = "#cc7f94";
static const char col_borderbar[] = "#1e2122"; // Цвет внутренней границы баров

/* Таблица схем: fg, bg, border */
static const char *colors[][3] = {
    /*               fg       bg       border */
    [SchemeNorm] = {gray3, black, gray2},
    [SchemeSel] = {gray4, blue, blue},
    [SchemeLayout] = {"#85ae61", black, black},
    [SchemeTag1] = {"#6f8faf", black, black},
    [SchemeTag2] = {"#ec6b64", black, black},
    [SchemeTag3] = {"#d6b676", black, black},
    [SchemeTag4] = {"#85ae61", black, black},
    [SchemeTag5] = {"#6f8faf", black, black},
    [SchemeTag6] = {"#ec6b64", black, black},
    [SchemeTag7] = {"#d6b676", black, black},
    [SchemeTag8] = {"#85ae61", black, black},
    [SchemeTag9] = {"#6f8faf", black, black},
    [SchemeTagEmpty] = {gray3, black, gray2},
};

/* ========== Теги ========== */
static const char *tags[] = {
    "", /* Tag1: Editor/Code */
    "", /* Tag2: Terminal/Build */
    "", /* Tag3: Browser/Docs */
    "", /* Tag4: System/Infra */
    "", /* Tag5: AI/ML */
    "", /* Tag6: Notes */
    "", /* Tag7: Chat */
    "", /* Tag8: Media */
    ""  /* Tag9: Sandbox */
};

/* Параметры подчёркивания тегов */
static const unsigned int ulinepad =
    7; /* Горизонтальный отступ между подчеркиванием и текстом тега */
static const unsigned int ulinestroke = 2; /* Толщина подчеркивания */
static const unsigned int ulinevoffset =
    0;                         /* Смещение линии вверх от нижнего края бар */
static const int ulineall = 0; /* 1: подчеркнуть все теги, 0: только активный */

/* ========== Правила для окон ========== */
static const Rule rules[] = {
    /* class        instance    title       tags mask   isfloating  monitor */
    {"TelegramDesktop", NULL, NULL, 1 << 6, 0, -1},
    {"firefox", NULL, NULL, 1 << 2, 0, -1},
    {"Google-chrome", NULL, NULL, 1 << 2, 0, -1},
    {"obsidian", NULL, NULL, 1 << 5, 0, -1},
};

/* ========== Макеты (layouts) ========== */
static const float mfact = 0.55; // Доля главного окна
static const int nmaster = 1;    // Количество окон в мастере
static const int resizehints =
    1; // 1: учитывать подсказки приложений при изменении размера
static const int lockfullscreen = 1; // 1: блокировать fullscreen-окна

static const Layout layouts[] = {
    /* символ  функция */
    {"[]=", tile},    /* Основной макет: tiled */
    {"><>", NULL},    /* Плавающие окна */
    {"[M]", monocle}, /* Все окна на весь экран */
};

/* ========== Клавиши и сочетания ========== */
/* Модификатор: WIN/Super */
#define MODKEY Mod4Mask

/* Помощник для тегов */
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* Запуск shell-команд */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* ========== Команды для запуска ========== */
static char dmenumon[2] = "0"; /* Монитор для dmenu, обновляется динамически */
static const char *dmenucmd[] = {"dmenu_run", "-m",  dmenumon, "-fn", dmenufont,
                                 "-nb",       black, "-nf",    gray3, "-sb",
                                 blue,        "-sf", black,    NULL};
static const char *termcmd[] = {"st", NULL};

/* Звук через pamixer */
static const char *volupcmd[] = {"pamixer", "--allow-boost", "--increase", "5",
                                 NULL};
static const char *voldowncmd[] = {"pamixer", "--allow-boost", "--decrease",
                                   "5", NULL};
static const char *volmutecmd[] = {"pamixer", "--toggle-mute", NULL};

/* Яркость через brightnessctl */
static const char *brightupcmd[] = {"brightnessctl", "set", "5%+", NULL};
static const char *brightdowncmd[] = {"brightnessctl", "set", "5%-", NULL};

/* Скриншот через flameshot */
static const char *flameshotfullcmd[] = {"flameshot", "gui", NULL};
static const char *flameshotsavecmd[] = {
    "sh", "-c", "flameshot full -p ~/Pictures/Screenshots", NULL};

/* Блокировка экрана */
static const char *lockcmd[] = {"slock", NULL};

/* Файловый менеджер и браузер */
static const char *fmtermcmd[] = {"st", "-e", "lf", NULL};
static const char *browsercmd[] = {"firefox", NULL};

/* ========== Привязки клавиш ========== */
static const Key keys[] = {
    /* --- Запуск приложений --- */
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_b, spawn, {.v = browsercmd}},
    {MODKEY, XK_e, spawn, {.v = fmtermcmd}},

    /* --- Показ/скрытие статусбара --- */
    {MODKEY | ShiftMask, XK_b, togglebar, {0}},

    /* --- Навигация по окнам --- */
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},

    /* --- Управление отступами (gaps) при наличии патча vanitygaps --- */
    {MODKEY, XK_g, incrgaps, {.i = +5}},
    {MODKEY | ShiftMask, XK_g, incrgaps, {.i = -5}},
    {MODKEY | ControlMask, XK_g, incrogaps, {.i = +5}},
    {MODKEY | ControlMask | ShiftMask, XK_g, incrogaps, {.i = -5}},
    {MODKEY | Mod1Mask, XK_g, incrigaps, {.i = +5}},
    {MODKEY | Mod1Mask | ShiftMask, XK_g, incrigaps, {.i = -5}},
    {MODKEY, XK_y, incrihgaps, {.i = +3}}, // Горизонтальные внутренние
    {MODKEY | ShiftMask, XK_y, incrihgaps, {.i = -3}},
    {MODKEY, XK_u, incrivgaps, {.i = +3}}, // Вертикальные внутренние
    {MODKEY | ShiftMask, XK_u, incrivgaps, {.i = -3}},
    {MODKEY | ControlMask,
     XK_y,
     incrohgaps,
     {.i = +3}}, // Горизонтальные внешние
    {MODKEY | ControlMask | ShiftMask, XK_y, incrohgaps, {.i = -3}},
    {MODKEY | ControlMask, XK_u, incrovgaps, {.i = +3}}, // Вертикальные внешние
    {MODKEY | ControlMask | ShiftMask, XK_u, incrovgaps, {.i = -3}},
    {MODKEY, XK_minus, togglegaps, {0}},
    {MODKEY, XK_equal, defaultgaps, {0}},

    /* --- Управление окнами --- */
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY | ShiftMask, XK_j, movestack, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, movestack, {.i = -1}},

    /* --- Смены макета --- */
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}}, /* tile */
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}}, /* floating */
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}}, /* monocle */
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},

    /* --- Теги --- */
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8)

    /* --- Громкость (XF86 keys) --- */
    {0, XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd}},
    {0, XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd}},
    {0, XF86XK_AudioMute, spawn, {.v = volmutecmd}},

    /* --- Яркость экрана (XF86 keys) --- */
    {0, XF86XK_MonBrightnessUp, spawn, {.v = brightupcmd}},
    {0, XF86XK_MonBrightnessDown, spawn, {.v = brightdowncmd}},

    /* --- Скриншот через flameshot --- */
    {MODKEY, XK_Print, spawn, {.v = flameshotfullcmd}},
    {MODKEY | ShiftMask, XK_Print, spawn, {.v = flameshotsavecmd}},

    /* --- Блокировка экрана --- */
    {MODKEY | ControlMask, XK_l, spawn, {.v = lockcmd}},

    /* --- Выход из dwm --- */
    {MODKEY | ShiftMask, XK_q, quit, {0}},
};

/* ========== Привязки мыши ========== */
static const Button buttons[] = {
    /* Click                Mask       Button   Function        Argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    // {ClkWinTitle,         0,         Button2, zoom,           {0} },
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
