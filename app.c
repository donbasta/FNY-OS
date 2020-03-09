/* Program FNY */
#include "kernel.h"

void main () {

  char pad = 0xe;
  char namaSatu[20*512];
  char namaDua[20*512];
  int i;
  int j;
  int cocok1;
  int cocok2;
  int kecocokan;
  int digit[10];
  int satuan;
  int puluhan;
  int persen;

  digit[0] = pad * 0x100 + '0';
  digit[1] = pad * 0x100 + '1';
  digit[2] = pad * 0x100 + '2';
  digit[3] = pad * 0x100 + '3';
  digit[4] = pad * 0x100 + '4';
  digit[5] = pad * 0x100 + '5';
  digit[6] = pad * 0x100 + '6';
  digit[7] = pad * 0x100 + '7';
  digit[8] = pad * 0x100 + '8';
  digit[9] = pad * 0x100 + '9';
  persen = pad * 0x100 + '%';
  
  printString("Aplikasi jodoh lol!!\n\n\r");

  printString("Masukkan nama anda: ");
  readString(namaSatu);
  printString("\n\r");
  printString("Masukkan nama teman spesial anda yang ingin dicocokkan: ");
  readString(namaDua);

  printString("\n\n");

  i = 0;
  cocok1 = 0;
  while(namaSatu[i] != 0x0){
    cocok1 = cocok1 + namaSatu[i]; 
    i++;
  }

  j = 0;
  cocok2 = 0;
  while(namaDua[j] != 0x0){
    cocok2 = cocok2 + namaDua[j]; 
    j++;
  }

  kecocokan = mod(cocok1 + cocok2, 100);
  satuan = mod(kecocokan, 10);
  puluhan = div(kecocokan, 10);


  printString("\r\nTingkat kecocokan anda dengan si dia adalah:");
  interrupt(0x10, digit[puluhan], 0x0, 0x0, 0x0); // interrupt digit puluhan
  interrupt(0x10, digit[satuan], 0x0, 0x0, 0x0); // interrupt digit satuan
  interrupt(0x10, persen, 0x0, 0x0, 0x0); // interrupt karakter persen
  printString("\n\n\r");

  printString("Verdict:\n\r");
  if(kecocokan < 20){
    printString("Bukan Jodoh :(\n\n\r");
  } else if (kecocokan < 50){
    printString("Rawan Cerai :/\n\n\r");
  } else if (kecocokan < 75){
    printString("Jaim2an, chat mandek\n\n\r");
  } else if (kecocokan < 90){
    printString("Gaskeun KUA\n\n\r");
  } else {
    printString("Selamat, langsung kawin aja\n\n\r");
  }
  
  while(1);

}
