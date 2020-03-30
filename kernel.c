/* Function Declaration */
void handleInterrupt21(int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void clear(char *buffer, int length);
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void executeProgram(char *filename, int segment, int *success);

char map[512];
char files[512 * 2];
char sectors[512];
char freeSector[512];

void main()
{
  int *success;
  char cmd[20];
  int curDir = 0xff;
  int sect;
  char buff[200];

  makeInterrupt21();

  clear(buff,200);
  // buff[0]='a';buff[1]='b';buff[2]='c';
  // writeFile(buff, "lol1", &sect, 4);
  // buff[0]='d';buff[1]='e';buff[2]='f';buff[3]='g';
  // writeFile(buff, "lol2", &sect, 4);

  printString("\n\n\n\n");
  printString("/        | / \\   / | / \\   /    |    /      \\  /     \\\n\r");
  printString("$$$$$$$$/  $$ \\ $$ | $$ \\ /$$  /    /$$$$$$ | /$$$$$$ |\n\r");
  printString("$$ |__     $$$ \\$$ |  $$ \\/$$ /     $$ | $$ | $$ \\__$$/\n\r");
  printString("$$ |       $$$$ $$ |   $$ $$ /      $$ | $$ | $$ \\\n\r");
  printString("$$$$$/     $$$$ $$ |    $$$$/       $$ | $$ |  $$$$$$ |\n\r");
  printString("$$ |       $$ $$$$ |     $$ |       $$ \\_$$|/   \\__$$ |\n\r");
  printString("$$ |       $$ |$$$ |     $$ |       $$   $$/  $$   $$/\n\r");
  printString("$$/        $$/  $$/      $$/        $$$$$$/   $$$$$$/\n\r");
  printString("\n\n\n\n");


  printString("Ketikkan cmd untuk masuk ke dalam shell: ");
  readString(cmd);
  if(cmd[0]=='c'&&cmd[1]=='m'&&cmd[2]=='d'){
    executeProgram("shell", 0x2000, success, curDir);
  }

  while (1);

}

void handleInterrupt21 (int AX, int BX, int CX, int DX) {  
    char AL, AH;
    AL = (char) (AX);
    AH = (char) (AX >> 8);   
    switch (AL) {      
        case 0x00:
            printString(BX);
            break;      
        case 0x01:         
            readString(BX);         
            break;      
        case 0x02:
            readSector(BX, CX);         
            break;      
        case 0x03:         
            writeSector(BX, CX);         
            break;      
        case 0x04:         
            readFile(BX, CX, DX, AH);         
            break;      
        case 0x05:         
            writeFile(BX, CX, DX, AH);         
            break;      
        case 0x06:         
            executeProgram(BX, CX, DX, AH);         
            break;      
        default:         
            printString("Invalid interrupt");   
    }
}

void printString(char *string)
{

  char ah = 0xe;
  int al;
  int ax;

  while (*string != '\0')
  {
    al = *string;
    ax = ah * 256 + al;
    interrupt(0x10, ax, 0x0, 0x0, 0x0);
    // if (al == 0xd) interrupt(0x10, 0xe*0x100+10, 0x0, 0x0);
    ++string;
  }

  return;
}

void readString(char *string)
{

  int ptr = 0;
  char ch = 0x0;

  while (1)
  {
    ch = interrupt(0x16, 0x0, 0x0, 0x0, 0x0);
    if (ch == 0xd)
      break; //jika user menekan tombol enter
    if (ch == 0x8)
    {
      if (ptr > 0)
      {
        interrupt(0x10, 0xe * 0x100 + ch, 0x0, 0x0, 0x0);
        interrupt(0x10, 0xe * 0x100 + 32, 0x0, 0x0, 0x0);
        interrupt(0x10, 0xe * 0x100 + ch, 0x0, 0x0, 0x0);
        --ptr;
      }
      else
        continue;
    }
    else
    {
      string[ptr++] = ch;
      interrupt(0x10, 0xe * 0x100 + ch, 0x0, 0x0, 0x0);
    }
  }
}

int div(int a, int b)
{
  return a / b;
}

int mod(int a, int b)
{
  return a - (a / b) * b;
}

void readSector(char *buffer, int sector)
{
  interrupt(0x13, 0x201, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void writeSector(char *buffer, int sector)
{
  interrupt(0x13, 0x301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void writeFile(char *buffer, char *path, int *sectors, char parentIndex){
  char map[512];
  char files[512 * 2];
  char sectors[512];

  int i,j,k;
  int entryIndex;
  int entryFile;
  int sama;
  int beda;

  char parent[14];
  char filename[14];
  char idxParent;

  readSector(map, 256);
  readSector(files, 257);
  readSector(files + 512, 258);
  readSector(sectors, 259);

  for(i = 0;i<14;i++){
    parent[i] = 0x0;
    filename[i] = 0x0;
  }
  
  // mengambil current file name dan current parent name
  j = 0;
  for(i = 0 ;i < sizeof(path); i++){
    if(path[i] != '/'){
      filename[j++] = path[i];
    }else{
      for(; j < 14; j++){
        filename[j] = 0x0;
      }
      j = 0;
      for(k = 0;k<14;k++){
        parent[k] = filename[k];
      }
    }
  }
  // pad with 0
  for(;j<14;j++){
    filename[j] = 0x0;
  }

  // mencari index parent
  idxParent = parentIndex;
  if(parent[0] != 0x0){
    for(i = 0;i<64;i++){
      if(files[i*16 + 1] == 0xFF){
        int beda = 0;
        for(j = 0;j<14;j++){
          if(files[i*16 + 2 + j] != parent[j]){
            beda = 1;
            break;
          }
        }
        if(beda){
          continue;
        }
        idxParent = files[i*16];
      }
    }
  }

  // mencari apakah ada file yang sama
  sama = 0;
  for(i = 0;i<64;i++){
    if(files[i*16 + 1] != 0xFF && files[i*16] == idxParent){
      beda = 0;
      for(j = 0;j<14;j++){
        if(files[i*16 + 2 + j] != filename[j]){
          beda = 1;
          break;
        }
      }
      if(beda) continue;

      sama = 1;
      break;
    }
  }

  if(sama){
    *sectors = -1;
    printString("File sudah ada!\n");
    return;
  }

  // mencari sector index yang kosong
  for(i = 0;i<32;i++){
    if(sectors[i*16]==0x0){
      entryIndex = i;
      break;
    }
  }

  // cek apakah terdapat entri yang masih kosong
  entryFile = -1;
  for(i = 0;i<64;i++){
    if(files[i*16 + 2] == 0x0){
      entryFile = i;
      break;
    }
  }

  if(entryFile == -1){
    *sectors = -2;
    printString("Tidak cukup entri di files");
    return;
  }
  // cek sector yang masih kosong
  j = 0;
  for(i = 0;i<512;i++){
    if(map[i] == 0x0){
      freeSector[j++] = i;
    }
  }

  // sector yang dibutuhkan oleh buffer

  k = 0;
  i = 0;
  while(buffer[k] != 0x0){
    i++;
    k+=512;
  }

  if(i > j){
    *sectors = -3;
    printString("Alokasi gagal!!\n");
    return;
  }

  files[entryFile*16] = idxParent;
  files[entryFile*16 + 1] = entryIndex;

  for(i = 0; i<14;i++){
    files[entryFile*16 + 2 + i] = filename[j];
  }

  // mengisi sector yang kosong pada sectors dengan indeks entryIndex;
  j = 0;
  i = 0;
  while(buffer[i] != 0x0){
    map[freeSector[j]] = 0xFF;
    sectors[entryIndex*16 + j] = freeSector[j];
    writeSector(buffer + i, freeSector[j]);
    j++;
    i += 512;
  }

  *sectors = j;

  // pad with 0
  for(; j<16;j++){
    sectors[entryIndex*16 + j] = 0x0;
  }

  writeSector(map, 256);
  writeSector(files, 257);
  writeSector(files + 512, 258);
  writeSector(sectors, 259);
}

void readFile(char *buffer, char *path, int *result, char parentIndex){
  char map[512];
  char files[512 * 2];
  char sectors[512];

  int sama;
  int beda;
  int i,j,k;
  int cnt;
  int sect;
  int entryIndex;
  int idxParent;
  char parent[14];
  char filename[14];
  readSector(files, 257);
  readSector(files + 512, 258);
  readSector(sectors, 259);


  for(i = 0;i<14;i++){
    parent[i] = 0x0;
    filename[i] = 0x0;
  }

  // mengambil current file name dan current parent name
  j = 0;
  i = 0;
  while(path[i] != 0x0){
    if(path[i] != '/'){
      filename[j++] = path[i];
    }else{
      for(; j < 14; j++){
        filename[j] = 0x0;
      }
      j = 0;
      for(k = 0;k<14;k++){
        parent[k] = filename[k];
      }
    }
    i++;
  }

  // pad with 0
  for(;j<14;j++){
    filename[j] = 0x0;
  }

  // mencari index parent
  idxParent = parentIndex;
  if(parent[0] != 0x0){
    for(i = 0;i<64;i++){
      if(files[i*16 + 1] == 0xFF){
        beda = 0;
        for(j = 0;j<14;j++){
          if(files[i*16 + 2 + j] != parent[j]){
            beda = 1;
            break;
          }
        }
        if(beda){
          continue;
        }
        idxParent = files[i*16];
      }
    }
  }

  // mencari apakah ada file yang sama
  sama = 0;
  for(i = 0;i<64;i++){
    if(files[i*16 + 1] != 0xFF && files[i*16] == idxParent){
      beda = 0;
      for(j = 0;j<14;j++){
        if(files[i*16 + 2 + j] != filename[j]){
          beda = 1;
          break;
        }
      }
      if(beda) continue;

      sama = 1;
      entryIndex = files[i*16 + 1];
      break;
    }
  }

  if(!sama){
    *result = -1;
    printString("Tidak ada file yang memenuhi!\n");
    return;
  }

  cnt = 0;

  for(i = 0;i<16;i++){
    sect = sectors[entryIndex*16 + i];

    if(sect == 0){
      break;
    }

    readSector(buffer + cnt, sect);
    cnt = cnt + 512;
  }

  *result = 1;
  printString("File berhasil dimuat!\n");
}



void clear(char *buffer, int length)
{

  int i;

  for (i = 0; i < length; i++)
  {
    buffer[i] = 0x00;
  }
}

void executeProgram(char *filename, int segment, int *success)
{

  int i;
  char buffer[512*16];

  readFile(&buffer, filename, success, 0xff);

  if (*success)
  {
    for (i = 0; i < 512*16; i++)
    {
      putInMemory(segment, i, buffer[i]);
    }

    launchProgram(segment);
    printString("Sukses\n\r");
  }
  else
  {
    printString("Gagal\n\r");
  }
}