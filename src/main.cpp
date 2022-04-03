#include "mbed.h"
#include "FlashIAPBlockDevice.h"
#include <TDBStore.h>



static DigitalOut led(LED1);


#define FIRST_DB_BLOCK (ADDR_FLASH_SECTOR_6)

static FlashIAPBlockDevice full_flash(FIRST_DB_BLOCK, 256 * 1024);
static TDBStore nvStore(&full_flash);

static uint32_t val;

int main() {
  full_flash.init();
  int res = nvStore.init();
  printf("init result = %d\n", res);

  res = nvStore.get("_0000", &val, sizeof(val));
  printf("get result = %d, value = %ld\n", res, val);

  val += 1;
  res = nvStore.set("_0000", &val, sizeof(val), 0);
  printf("set result = %d\n", res);

  for(;;) {
    led = !led;
    wait_us(1000000);
  }
}