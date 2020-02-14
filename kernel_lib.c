/* Function Declaration */
void handleInterrupt21(int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void readFile(char *buffer, char *filename, int *success);
void clear(char *buffer, int length);
void writeFile(char *buffer, char *filename, int *sectors);
void executeProgram(char *filename, int segment, int *success);

void handleInterrupt21 (int AX, int BX, int CX, int DX) {
	switch (AX) {
    case 0x0:
      printString(BX);
      break;
    case 0x1:
      readString(BX);
      break;
    case 0x2:
      readSector(BX, CX);
      break;
    case 0x3:
      writeSector(BX, CX);
      break;
    case 0x4:
      readFile(BX, CX, DX);
      break;
    case 0x5:
      writeFile(BX, CX, DX);
      break;
    case 0x6:
      executeProgram(BX, CX, DX);
      break;
    default:
      printString("Invalid interrupt");
	}
}

void printString(char *string){

  char ah = 0xe;
  int al;
  int ax;

  while (*string != '\0'){
    al = *string;
    ax = ah * 256 + al;
    interrupt(0x10, ax, 0x0, 0x0, 0x0);
    ++string;
  }

  return;

}

void readString(char *string){

  int ptr = 0;
  char ch = 0x0;

  while(1){
    ch = interrupt(0x16, 0x0, 0x0, 0x0, 0x0);
    if(ch == 0xd) break; //jika user menekan tombol enter
    if(ch == 0x8) {
      if(ptr>0) {
        interrupt(0x10, 0xe * 0x100 + ch, 0x0, 0x0, 0x0);
        --ptr;
      }
      else continue;
    }
    else {
      string[ptr++] = ch;
      interrupt(0x10, 0xe * 0x100 + ch, 0x0, 0x0, 0x0);
    }
  }

}

int div(int a, int b){
  return a/b;
}

int mod(int a, int b){
  return a - (a/b)*b;
}

void readSector(char *buffer, int sector){
  interrupt(0x13, 0x201, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void writeSector(char *buffer, int sector){
  interrupt(0x13, 0x301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

//The comment follows from the flowchart in the task specification (milestone 1)
void writeFile(char *buffer, char *filename, int *sectors){

  char map[512];
  char dir[512];
  char freeSector[512];

  int entryFile;
  int i,j;
  int numSectors;

  //Baca sektor map dan dir
  readSector(map,1);
  readSector(dir,2);

  //cek dir yang kosong
  for(i = 0; i < 16; i++){ //cek tiap baris di dir
    if(dir[i*32] == 0x0){ //jika ada baris yang kosong
      entryFile = i;
      break;
    }
  }

  //Cek Jumlah sektor di map apakah cukup untuk buffer file
  j = 0;
  for(i=0; i<512; i++){
    if(map[i] == 0x0){
      freeSector[j++] = i;
    }
  }
  //sekarang j menyimpan banyak sektor yang kosong.

  //Isi sektor pada dir dengan nama file
  for(j = 0; filename[j] != 0x0 && j < 12; j++){
    dir[entryFile*32+j] = filename[j];
  }
  for(; j<12; j++){ //pad the remaining place with 0
    dir[entryFile*32+j] = 0x0;
  }

  //Cari sektor di map yang kosong
  //Isi sektor tersebut dengan byte pada file buffer dan tandai di map 
  //Bersihkan sektor yang akan digunakan untuk menyimpan nama file
  j = 0; //j bakal menyimpan banyak sektor yang diperlukan  
  i = 0;
  while(buffer[i] != 0x00){
    map[freeSector[j]] = 0xff;
    dir[entryFile*32 + j + 12] = freeSector[j];
    writeSector(buffer + i, freeSector[j]);
    j++;
    i = i + 512;
  }

  *sectors = j;

  for(; j<20; j++){
    dir[entryFile*32 + j + 12] = 0x00; //pad remaining with zeroes
  }

  writeSector(map,1);
  writeSector(dir,2);

}

void readFile(char *buffer, char *filename, int *success){

  char map[512];
  char dir[512];
  char freeSector[512];

  int i,j,cek;
  int sect;
  int cnt;
  int entryFile;
  char cur[12];
  int sama;

  // Baca sektor dir
  readSector(dir,2);

  // iterate pada array dir, cek apakah nama file sesuai
  cek = 0; // ada nama filenya ga
  for(i=0; i<16; i++){
    sama = 1;
    if(dir[i*32] == 0x0) continue;

    for(j=0; j<12; j++){
      cur[j] = dir[i*32+j];
    }
    for(j=0; j<12 && filename[j]!=0x0; j++){
      if(cur[j]!=filename[j]){
        sama = 0;
        break;
      }
    }
    if(filename[j]==0x0 && cur[j]!=0x0){
      sama = 0;
    }
    // printString(cur);
    if(sama){
      cek = 1;
      entryFile = i;
      break;
    }
  }

  // jika tidak ada, success <- false, keluar
  if(!cek){
    *success = 0;
    printString("Tidak ada file yang memenuhi\n\r");
    return;
  }

  // jika ada, inisialisasi no_sektor <- dir[entry*32+12] (isi file)
  cnt = 0;

  // while no_sektor != 0, baca ke buffer dan lanjutkan next hingga no_sektor == 0
  for(j=12; j<32; j++){
    sect = dir[entryFile*32+j];

    if(sect == 0){
      break;
    }

    readSector(buffer + cnt, sect);
    cnt = cnt + 512;
  }

  *success = 1;
  printString("File berhasil dimuat!!\n\r");
  return;
}


void clear(char *buffer, int length){

  int i;

  for(i=0;i<length;i++){
    buffer[i] = 0x00;
  }

}

void executeProgram(char *filename, int segment, int *success){
  
  int i;
  char buffer[13312];

  readFile(buffer, filename, success);

  if(*success){
    for(i=0;i<13312;i++){
      putInMemory(segment, i ,buffer[i]);
    }

    launchProgram(segment);
    printString("Sukses\n\r");
  } else{
    printString("Gagal\n\r");
  }
}