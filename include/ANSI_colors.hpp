#pragma once

#define ANSI_DEFAULT_COLOR  "\033[0m"
#define ANSI_BLACK          "\033[30m"
#define ANSI_RED            "\033[31m"
#define ANSI_GREEN          "\033[32m"
#define ANSI_YELLOW         "\033[33m"
#define ANSI_BLUE           "\033[34m"
#define ANSI_MAGENTA        "\033[35m"
#define ANSI_CYAN           "\033[36m"
#define ANSI_WHITE          "\033[37m"
#define ANSI_LIGHT_BLACK    "\033[90m"
#define ANSI_LIGHT_RED      "\033[91m"
#define ANSI_LIGHT_GREEN    "\033[92m"
#define ANSI_LIGHT_YELLOW   "\033[93m"
#define ANSI_LIGHT_BLUE     "\033[94m"
#define ANSI_LIGHT_MAGENTA  "\033[95m"
#define ANSI_LIGHT_CYAN     "\033[96m"
#define ANSI_LIGHT_WHITE    "\033[97m"

#define print_black(s)      ANSI_BLACK          << s << ANSI_DEFAULT_COLOR
#define print_red(s)        ANSI_RED            << s << ANSI_DEFAULT_COLOR
#define print_green(s)      ANSI_GREEN          << s << ANSI_DEFAULT_COLOR
#define print_yellow(s)     ANSI_YELLOW         << s << ANSI_DEFAULT_COLOR
#define print_blue(s)       ANSI_BLUE           << s << ANSI_DEFAULT_COLOR
#define print_magenta(s)    ANSI_MAGENTA        << s << ANSI_DEFAULT_COLOR
#define print_cyan(s)       ANSI_CYAN           << s << ANSI_DEFAULT_COLOR
#define print_white(s)      ANSI_WHITE          << s << ANSI_DEFAULT_COLOR
#define print_lblack(s)     ANSI_LIGHT_BLACK    << s << ANSI_DEFAULT_COLOR
#define print_lred(s)       ANSI_LIGHT_RED      << s << ANSI_DEFAULT_COLOR
#define print_lgreen(s)     ANSI_LIGHT_GREEN    << s << ANSI_DEFAULT_COLOR
#define print_lyellow(s)    ANSI_LIGHT_YELLOW   << s << ANSI_DEFAULT_COLOR
#define print_lblue(s)      ANSI_LIGHT_BLUE     << s << ANSI_DEFAULT_COLOR
#define print_lmagenta(s)   ANSI_LIGHT_MAGENTA  << s << ANSI_DEFAULT_COLOR
#define print_lcyan(s)      ANSI_LIGHT_CYAN     << s << ANSI_DEFAULT_COLOR
#define print_lwhite(s)     ANSI_LIGHT_WHITE    << s << ANSI_DEFAULT_COLOR