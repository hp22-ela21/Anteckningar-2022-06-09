/* Inkluderingsdirektiv: */
#include "gpio.h"

/***************************************************************************
* leds_blink: Blinkar lysdioder framåt och bakåt i en slinga. Lysdiodernas
*             PIN-nummer lagras i ett fält av storleken size. Först sker
*             blinkning framåt från första till sista lysdiod. Därefter
*             sker blinkning bakåt från näst sista till första lysdioden.
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
* leds_off: Släcker ett antal lysdioder, vars PIN-nummer lagras i ett
*           fält av storleken size.
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
*       PIN 27. Eventdetektering sker på tryckknappens insignal, där
*       lysdioderna togglas mellan att blinka var 100:e millisekund eller
*       vara släckta vid stigande flank (när tryckknappen trycks ned).
*       För att enkelt implementera blinkning samt avstängning av ett
*       valfritt antal lysdioder, så lagras PIN-numren i en statisk array,
*       där fler PIN-numren kan läggas till vid behov.
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

