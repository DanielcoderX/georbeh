#ifndef GEORBEH_SIGNAL_H
#define GEORBEH_SIGNAL_H
#include    <stdlib.h>
#include    <sys/types.h>
#include    <signal.h>
#include    <unistd.h>
void signal_controller(void(*f)(int sig));
void signal_handler(int sig);
void eof_handler();
#endif //GEORBEH_SIGNAL_H
