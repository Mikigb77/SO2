#include <stdio.h>

int main(){
  char b = 0x8f;
  if (b & 0x80)
    printf("%s\n", "YES!!!");
  else
    printf("%s\n", "NO!!!");
}
