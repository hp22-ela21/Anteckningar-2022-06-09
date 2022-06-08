/* Inkluderingsdirektiv: */
#include "gpio.h"

/* Statiska funktioner: */
static void file_write(const char* filepath, const char* s);
static void file_read(const char* filepath, char* s, const size_t size);
static void gpio_export(const uint8_t pin);
static void gpio_unexport(const uint8_t pin);
static void gpio_set_direction(const uint8_t pin, const enum gpio_direction direction);

/***************************************************************************
* gpio_new: Reserverar aktuell PIN och väljer datariktning.
***************************************************************************/
void gpio_new(const uint8_t pin, const enum gpio_direction direction)
{
   gpio_export(pin);
   gpio_set_direction(pin, direction);
   return;
}

/***************************************************************************
* gpio_delete: Tar bort reservation från aktuell PIN.
***************************************************************************/
void gpio_delete(const uint8_t pin)
{
   gpio_unexport(pin);
   return;
}

/***************************************************************************
* gpio_write: Sätter utsignal på aktuell PIN. Filsökvägen sätts samman
*             med PIN-numret via anrop av funktionen sprintf.
***************************************************************************/
void gpio_write(const uint8_t pin, const uint8_t value)
{
   char filepath[50];
   filepath[0] = '\0';
   sprintf(filepath, "/sys/class/gpio/gpio%u/value", pin);
   if (value)
   {
      file_write(filepath, "1");
   }
   else
   {
      file_write(filepath, "0");
   }
   return;
}

/***************************************************************************
* gpio_read: Returnerar insignal från en given PIN.
***************************************************************************/
uint8_t gpio_read(const uint8_t pin)
{
   char s[10];
   char filepath[50];
   s[0] = '\0';
   filepath[0] = '\0';

   sprintf(filepath, "/sys/class/gpio/gpio%u/value", pin);
   file_read(filepath, s, sizeof(s));

   if (s[0] == '1')
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

/***************************************************************************
* gpio_delay: Genererar fördröjning mätt i millisekunder.
***************************************************************************/
void gpio_delay(const size_t delay_time)
{
   usleep(delay_time * 1000);
   return;
}

/***************************************************************************
* gpio_blink: Blinkar utsignalen på aktuell PIN.
***************************************************************************/
void gpio_blink(const uint8_t pin, const size_t delay_time)
{
   gpio_write(pin, 1);
   gpio_delay(delay_time);
   gpio_write(pin, 0);
   gpio_delay(delay_time);
   return;
}

/***************************************************************************
* gpio_event_detected: Indikerar ifall ett specifikt event har ägt rum
*                      på aktuell PIN och uppdaterar senast avlästa
*                      insignal via ingående argument last_value. En
*                      fördröjning på 50 ms implementeras för att minimera
*                      påverkan av kontaktstudsar.
***************************************************************************/
bool gpio_event_detected(const uint8_t pin, const enum gpio_event event, uint8_t* last_value)
{
   const uint8_t previous_value = *last_value;
   const uint8_t current_value = gpio_read(pin);
   *last_value = current_value;

   if (current_value == previous_value) return false;
   gpio_delay(50);

   if (event == GPIO_EVENT_RISING_EDGE)
   {
      if (current_value && !previous_value) return true;
      else return false;
   }
   else if (event == GPIO_EVENT_FALLING_EDGE)
   {
      if (!current_value && previous_value) return true;
      else return false;
   }
   else if (event == GPIO_EVENT_BOTH_EDGES)
   {
      if (current_value != previous_value) return true;
      else return false;
   }
 
   return false;
}

/***************************************************************************
* file_write: Skriver ett textstycke till en fil.
***************************************************************************/
static void file_write(const char* filepath, const char* s)
{
   FILE* fstream = fopen(filepath, "w");
   
   if (!fstream)
   {
      fprintf(stderr, "Could not open file at path %s!\n\n", filepath);
   }
   else
   {
      fprintf(fstream, "%s", s);
      fclose(fstream);
   }

   return;
}

/***************************************************************************
* file_read: Läser en rad från en fil och lagrar i en sträng.
***************************************************************************/
static void file_read(const char* filepath, char* s, const size_t size)
{
   FILE* fstream = fopen(filepath, "r");

   if (!fstream)
   {
      fprintf(stderr, "Could not open file at path %s!\n\n", filepath);
   }
   else
   {
      fgets(s, (int)size, fstream);
      fclose(fstream);
   }

   return;
}

/***************************************************************************
* gpio_export: Reserverar aktuell PIN för användning.
***************************************************************************/
static void gpio_export(const uint8_t pin)
{
   const char* filepath = "/sys/class/gpio/export";
   char s[10];
   s[0] = '\0';
   sprintf(s, "%u", pin);
   file_write(filepath, s);
   return;
}

/***************************************************************************
* gpio_unexport: Tar bort reservation av PIN efter användning.
***************************************************************************/
static void gpio_unexport(const uint8_t pin)
{
   const char* filepath = "/sys/class/gpio/unexport";
   char s[10];
   s[0] = '\0';
   sprintf(s, "%u", pin);
   file_write(filepath, s);
   return;
}

/***************************************************************************
* gpio_set_direction: Sätter datariktning på aktuell PIN.
***************************************************************************/
static void gpio_set_direction(const uint8_t pin, const enum gpio_direction direction)
{
   char filepath[50];
   filepath[0] = '\0';
   sprintf(filepath, "/sys/class/gpio/gpio%u/direction", pin);
   
   if (direction == GPIO_DIRECTION_OUT)
   {
      file_write(filepath, "out");
   }
   else
   {
      file_write(filepath, "in");
   }

   return;
}
