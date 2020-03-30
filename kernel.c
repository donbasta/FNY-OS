/* Function Declaration */
void handleInterrupt21(int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void clear(char *buffer, int length);
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void executeProgram(char *filename, int segment, int *success, int parentIndex);

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

void readSector(char *buffer, int sector)
{
  interrupt(0x13, 0x201, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void writeSector(char *buffer, int sector)
{
  interrupt(0x13, 0x301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}



void clear(char *buffer, int length)
{

  int i;

  for (i = 0; i < length; i++)
  {
    buffer[i] = 0x00;
  }
}

void executeProgram(char *filename, int segment, int *success, int parentIndex)
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