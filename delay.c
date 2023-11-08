#include "my_lib.c"

void Delay(int Value){
  for(int i=0; i<Value;i++){
    asm("nop");
  }
}