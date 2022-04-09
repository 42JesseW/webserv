#pragma once

#include <csignal>
#include <cstring>
#include <ConfigUtil.hpp>

void    signal_handler(int sig);
void    register_signals(void);
