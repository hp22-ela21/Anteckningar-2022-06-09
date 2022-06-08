/* Inkluderingsdirektiv: */
#include "gpio.h"

/***************************************************************************
* leds_blink: Blinkar lysdioder fram�t och bak�t i en slinga. Lysdiodernas
*             PIN-nummer lagras i ett f�lt av storleken size. F�rst sker
*             blinkning fram�t fr�n f�rsta till sista lysdiod. D�refter
*             sker blinkning bak�t fr�n n�st sista till f�rsta lysdioden.
***************************************************************************/
static void leds_blink(const uint8_t* leds, const size_t size, const size_t delay_time)
{
   for (register const uint8_t* i = leds; i < leds + size; ++i)
   {
      gpio_write(*i, 1);
      gpio_delay(delay_time);
      gpio_write(*i, 0);
   }

   for (register const uint8_t* i = leds + size - 2; ; --i)
   {
      gpio_write(*i, 1);
      gpio_delay(delay_time);
      gpio_write(*i, 0);
      if (i == leds) break;
   }
   return;
}

/***************************************************************************
* leds_off: Sl�cker ett antal lysdioder, vars PIN-nummer lagras i ett
*           f�lt av storleken size.
***************************************************************************/
static void leds_off(const uint8_t* leds, const size_t size)
{
   for (register const uint8_t* i = leds; i < leds + size; ++i)
   {
      gpio_write(*i, 0);
   }
   return;
}

/***************************************************************************
* main: Ansluter lysdioder till PIN 17, 22 och 23 samt en tryckknapp till
*       PIN 27. Eventdetektering sker p� tryckknappens insignal, d�r
*       lysdioderna togglas mellan att blinka var 100:e millisekund eller
*       vara sl�ckta vid stigande flank (n�r tryckknappen trycks ned).
*       F�r att enkelt implementera blinkning samt avst�ngning av ett
*       valfritt antal lysdioder, s� lagras PIN-numren i en statisk array,
*       d�r fler PIN-numren kan l�ggas till vid behov.
***************************************************************************/
int main(void)
{
   gpio_new(17, GPIO_DIRECTION_OUT);
   gpio_new(22, GPIO_DIRECTION_OUT);
   gpio_new(23, GPIO_DIRECTION_OUT);
   gpio_new(27, GPIO_DIRECTION_IN);

   uint8_t leds[] = { 17, 22, 23 };
   uint8_t last_value = 0;
   bool leds_enabled = false;

   while (1)
   {
      if (gpio_event_detected(27, GPIO_EVENT_RISING_EDGE, &last_value))
      {
         leds_enabled = !leds_enabled;
      }

      if (leds_enabled)
      {
         leds_blink(leds, 3, 100);
      }
      else
      {
         leds_off(leds, 3);
      }
   }

   return 0;
}

