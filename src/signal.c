#include "signal.h"
void eof_handler()
{
	return exit(EXIT_SUCCESS);
}
struct sigaction siga;
void signal_handler(int sig) {
    switch (sig) {
        case 17:
            break;
        case 2:
            return 1;
            break;
    }
}

// sets f as handler to all the possible signals.
void signal_controller(void(*f)(int sig)) {
    siga.sa_handler = f;
    for (int sig = 1; sig <= SIGRTMAX; ++sig) {
        sigaction(sig, &siga, NULL);
    }
}
