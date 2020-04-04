/* Function Declaration */
void handleInterrupt21(int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
// void readSector(char *buffer, int sector);
// void writeSector(char *buffer, int sector);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void clear(char *buffer, int length);
// void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void executeProgram(char *filename, int segment, int *success, int parentIndex);

void bacaInput(char *buff);
void showFolderContent(char* str);
int cekNamaFile(char* files, char* buff, int i);
// void printNumber(int a);
void generatePath(char curDir);
int strcmp(char* s1, char* s2);

char map[512];
char files[512 * 2];
char sectors[512];
char freeSector[512];

char buff[100];
char absPath[100];
// char history[100][100];

// int cnt=-999, head = 0, tail=0;

extern char stat = '\0';
int delete = 0;
int copy = 0;
int move = 0;
char filename1[50];
char pathParams1[14];
char pathParams2[14];
int pindahFolder = 0;
int parentCopy = 0xFF;

void main()
{
  int *success;
  char com[20];
  char bufferCopy[512*16];
  char curDir = 0xff;
  int i;
  // char stat = 0x0;
  for(i = 0;i<50;i++){
    filename1[i] = 0x0;
  }
  makeInterrupt21();

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
  readString(com);
  if(com[0]=='c'&&com[1]=='m'&&com[2]=='d'){
    // executeProgram("shell", 0x2000, success, curDir);

      printString("\n\n\r");
      printString("Selamat datang di shell v.0.1\n\r");

      curDir = 0xff;

      while(1){
        // interrupt(0x21,1,buff,0,0);
        if(stat == '\0'){
          for(i = 0; i<100; i++){
            buff[i] = 0x0;
          }
          printString("\n\r");
          printString("bapak_imba@fny_os:");
          generatePath(curDir);
          printString("$ ");
        }
        readString(buff);
        bacaInput(buff, &curDir);
        if(delete){
          if(copy || move){
            deleteFile(pathParams2, success, curDir);
            move = 0;
          }
          else{
            deleteFile(filename1,success,curDir);
            for(i = 0;i<50;i++){
              filename1[i] = 0x0;
            }
            
          }
          delete = 0;
        }
        if(copy){
          readFile(bufferCopy, pathParams1, success, curDir);
          if(pindahFolder){
            writeFile(bufferCopy, pathParams1, success, parentCopy);
            pindahFolder = 0;
          }else{
            writeFile(bufferCopy, pathParams2, success, curDir);
          }
          copy = 0;
        }

        // printNumber(curDir);
      }
  }

  while (1);

}