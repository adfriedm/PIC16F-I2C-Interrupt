#ifndef __interrupts
#define __interrupts

// Note: As the interrupt system is currently setup, at the end
//       of each high-priority interrupt, the system will check
//       to see if the processor may be put to sleep.  This is
//       done with the call SleepIfOkay() which is defined in
//       messages.h -- init_queues() MUST be called prior to
//       enabling interrupts if SleepIfOkay() is called!

// enable the interrupts (high and low priority)
void enable_interrupts(void);

#endif
