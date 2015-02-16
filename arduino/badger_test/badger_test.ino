// First test
// Loops between two images

#include <SPI.h>
#include <SD.h>
#include "EPD.h"
#include "S5813A.h"
#include "EReader.h"

// I/O setup
void setup() {
  ereader.setup(EPD_2_7);
}

// main loop
unsigned long int loop_count = 0;

void loop() {
  if(loop_count % 2 == 0){
    ereader.display_wif("/BADGES/OCLS_01.WIF", 0, 0);
  }
  else if(loop_count % 2 == 1){
    ereader.display_wif("/IMAGES/LENA.WIF", 0, 0);
  }

  loop_count++;
  ereader.show();
  // while(1)
  ereader.sleep(4000); // allows EPD to power off gracefully
  ereader.wake();
}
