#ifndef ECAP_H
#define ECAP_H
#include <iostream>
#include "engineFrame.h"
#include "signals.h"
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <signal.h>

int *base = get_base();

void convert_text_to_vectors();
void terminate_tcpdump();

#endif