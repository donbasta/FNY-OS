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

void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
  char map[512];
  char dir[1024];
  char sector[512];
  int i, j, avail, found, isExist;


  readSector(map, 0x100);
  readSector(dir, 0x101);
  readSector(dir + 512, 0x102);
  readSector(sector, 0x103);
  j = 0;
  found = 0;
    
    
    // TODO : check if folder exists
    
    
  // printString("Searching for empty directory :\n\r");
  while (!found && j < 1024) {
    if (dir[j] == 0x0 && dir[j + 1] == 0x0 && dir[j + 2] == 0x0) {
      found = 1;
    } else {
      j += 16;
    }
  }
  // Cari apakah nama file ada di dalam dir
    // Potongan 32 bytes, 12 bytes file name, 20 bytes sector
    isExist = 0;
    i = 0;
    while (isExist == 0 && i < 64) {
        // Cek apakah pada terdapat file pada baris ke i
        if (dir[16 * i] == parentIndex) {
            // writeLine("Same dir!");
            if (dir[16 * i + 2] != 0x0 && dir[16 * i + 1] != 0xFF) {
                int curPos = i * 16 + 2;
                
                // Terdapat file, cek kesamaan nama
                int j = 0;
                int isSame = 1;
                // printString("\r\nFile found with name : \0");
                // printString(dirBuf + 16 * i + 2);
                // printString(" <====> \0");
                // printString(filename);
                // printString("\r\n\0");
                while (isSame == 1 && j < 14) {
                    if (dir[j + curPos] != path[j]) {
                        isSame = 0;
                    } else if (dir[j + curPos] == '\0' && path[j] == '\0') {
                        j = 13;
                    }
                    j++;
                }

                if (isSame) {
                    // printString("Same!\r\n\0");
                    isExist = 1;
                    *sectors = -1;
                    return;
                    // if (dirBuf[i * 16 + 1] == 0x15) { writeLine("On sector 21"); }
                }
            }
        }
        
        i++;
    }
  // printString("Done Searching\n\r");
  if (found && !isExist) {
    int k = 0;
    int stop = 0;
    // printString("There is an empty directory\n\r");
    // printString("Searching for empty sector map\n\r");
    while (!stop && k < 256) {
      if (map[k] == 0x0) {
        stop = 1;
      } else {
        k++;
      }
    }
    // printString("Done searching\n\r");
    if ((256 - k) >= 16) {
      int m, n, o, sectCount;
      char temp[512];
      // printString("there is an empty sector\n\r");
            for (o = 0; o < 32; o++) {
                if (sector[o*16] == 0x0) break;
            }
      for (m = 0; m < 16; m++) {
        dir[j + m] = 0x0;
      }
      for (m = 2; m < 16; m++) {
                dir[j + m] = path[m - 2];
            }
      // mark entry
      dir[j] = parentIndex;
      dir[j + 1] = o;
      // printString("done renaming\n\r");
            
      n = 0;
      m = 0;
            sectCount = 0;
      // printString("putting buffer into sectors\n\r");
      while (buffer[n] != 0x0) {
        int l = 0;
        clear(temp, 512);
        while (buffer[n] != 0x0 && l < 512) {
          temp[l] = buffer[n];
          n++;
          l++;
        }
        map[k] = 0xFF;
        sector[o * 16 + sectCount] = k;
        writeSector(temp, k);
        k++;
                sectCount++;
      }
      writeSector(sector, 0x103);
      writeSector(dir+512, 0x102);
      writeSector(dir, 0x101);
      writeSector(map, 0x100);
      // printString("DONEEE\n\r");
            *sectors = 1;
    } else {
      //terminate
            *sectors = -3;
      return;
    }
  } else {
    //terminate
        *sectors = -2;
    return;
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