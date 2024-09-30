#pragma once

#include <chrono>

const double NSEC_TO_SEC = 1e-9;
#define NO_OPT(x) __asm__ volatile("" : "+g" (x)) // simple callq with one jmpq