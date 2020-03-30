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
      if( ch == 0x0 ){
        interrupt(0x10, 0xe * 0x100 + 0x41, 0x0, 0x0, 0x0);
      }else{
        string[ptr++] = ch;
        interrupt(0x10, 0xe * 0x100 + ch, 0x0, 0x0, 0x0);
      }
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

void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
  char map[512];
  char files[1024];
  char sector[512];
  char buf[512];
  int i;
  int j;
  int k;
  int l;
  int m;
  int n;
  int ada;
  int sect;
  int pos;
  int sama;


  readSector(map, 256);
  readSector(files, 257);
  readSector(files + 512, 258);
  readSector(sector, 259);

  j = 0;
  l = 0;
    
  while (!l && j<64) {
    if (files[j*16] == 0x0 && files[j*16 + 1] == 0x0 && files[j*16 + 2] == 0x0) {
      l = 1;
    } else {
      j++;
    }
  }

    ada = 0;
    i = 0;
    while (ada == 0 && i < 64) {

        if (files[16 * i] == parentIndex) {
          
            if (files[16 * i + 2] != 0x0 && files[16 * i + 1] != 0xff) {
                j = 0;
                sama = 1;             
                while (sama == 1 && j < 14) {
                    if (files[j + 16*i + 2] != path[j]) {
                        sama = 0;
                    } else if (files[j + 16*i + 2] == 0x0 && path[j] == 0x0) {
                        j = 13;
                    }
                    j++;
                }

                if (sama) {
                    ada = 1;
                    *sectors = -1;
                    return;
                }
            }
        }
        
        i++;
    }

  if (l && !ada) {
    k = 0;
    pos = 0;
    while (!pos && k < 256) {
      if (map[k] == 0x0) {
        pos = 1;
      } else {
        k++;
      }
    }
    if (240 >= k) {
            for (i = 0; i < 32; i++) {
                if (sector[i*16] == 0x0) break;
            }
      for (m = 0; m < 16; m++) {
        files[j + m] = 0x0;
      }
      for (m = 2; m < 16; m++) {
                files[j + m] = path[m - 2];
            }
      files[j] = parentIndex;
      files[j + 1] = i;
            
      n = 0;
      m = 0;
      sect = 0;
      while (buffer[n] != 0x0) {
        int l = 0;
        clear(buf, 512);
        while (buffer[n] != 0x0 && l < 512) {
          buf[l] = buffer[n];
          n++;
          l++;
        }
        map[k] = 0xff;
        sector[i * 16 + sect] = k;
        writeSector(buf, k);
        k++;
        sect++;
      }
      
      writeSector(map, 256);
      writeSector(files, 257);
      writeSector(files+512, 258);
      writeSector(sector, 259);

            *sectors = 1;
    } else {
            *sectors = -3;
    }
  } else {
        *sectors = -2;
  }

  clear(path, 10);
  
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
printString(path);
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
printString(filename);
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
*result = 0;
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