#include <or1k-support.h>
#include <spr-defs.h>
#include<stdio.h>

char* i2c_base = (char*)  0xa0000000;
char* i2c_start = (char*) 0xa0000001; // 0x1; //4'b0001
char* i2c_wite = (char*)  0xa0000003; // 0x4;//4'b0100

unsigned char dat = 0x8A;
int test = 0;

/* Tick rate storage */
extern unsigned long or1k_timer_period;

extern volatile unsigned long or1k_timer_ticks;

void or1k_timer_user_handler(void)
{
  // Increment the GPIO counter
  *(i2c_wite) = ++dat;

}


void or1k_timer_interrupt_handler_new(void)
{
  /* Re-initialise timer */
  or1k_mtspr(SPR_TTMR, SPR_TTMR_IE | SPR_TTMR_RT | 
	     (or1k_timer_period & SPR_TTMR_PERIOD));

  /* Increment global ticks value */
  or1k_timer_ticks++;

  /* Call a user handler function */
  or1k_timer_user_handler();

}

int main(void)
{



  *(i2c_base) = 0xA0	; //write
  /* Initialise the counter at 1Hz, it won't start yet */
  or1k_timer_init(1000000);

  /* Install a custom timer handler */
  or1k_exception_handler_add(0x5,or1k_timer_interrupt_handler_new);
  
  /* OK, start the timer now */
  or1k_timer_enable();


  /* Now loop and do nothing, the timer will interrupt us when it fires */
  while (1);

  return 0;
}
