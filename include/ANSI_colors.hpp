#pragma once

#define ANSI_DEFAULT_COLOR   "\033[0m"
#define ANSI_YELLOW          "\033[33m"
#define ANSI_LIGHT_RED       "\033[91m"
#define ANSI_LIGHT_GREEN     "\033[92m"
#define ANSI_LIGHT_YELLOW    "\033[93m"
#define ANSI_LIGHT_BLUE      "\033[94m"
#define ANSI_LIGHT_CYAN      "\033[96m"

#define print_yellow(s)     ANSI_YELLOW        << s << ANSI_DEFAULT_COLOR
#define print_lred(s)       ANSI_LIGHT_RED     << s << ANSI_DEFAULT_COLOR
#define print_lgreen(s)     ANSI_LIGHT_GREEN   << s << ANSI_DEFAULT_COLOR
#define print_lyellow(s)    ANSI_LIGHT_YELLOW  << s << ANSI_DEFAULT_COLOR
#define print_lblue(s)      ANSI_LIGHT_BLUE    << s << ANSI_DEFAULT_COLOR
#define print_lcyan(s)      ANSI_LIGHT_CYAN    << s << ANSI_DEFAULT_COLOR
