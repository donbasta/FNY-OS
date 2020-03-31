void bacaInput(char *buff);
int isDir(char *dirPath);
int isFile(char *filePath);
void charRemnant(char *str, char *target, int i);
void showFolderContent(char* str);
int cekNamaFile(char* files, char* buff, int i);
void printNumber(int a);
void generatePath(char curDir);

char buff[100];
char absPath[100];
char history[100][100];
char cmd[100];
int cnt=-999, head = 0, tail = 0;

// Kalo dah ada include apus aja
extern char STATUS;

int main(){
	char curDir;
	int i;
	makeInterrupt21();
	printString("\n\n\r");
	printString("Selamat datang di shell v.0.1\n\r");
	printString("fny_os@bapak_imba:");
	// printString("/");
	printString("/");
	printString("$ ");

	curDir = 0xff;

	while(1){

		// interrupt(0x21,1,buff,0,0);
		for(i = 0; i<100; i++){
			buff[i] = 0x0;
		}
		readString(buff);
		bacaInput(buff, &curDir);
		printString("\n\r");
		printString("fny_os@bapak_imba:");
		generatePath(curDir);
		printString("$ ");
		// printNumber(curDir);
	}
}

void bacaInput(char* buff, char* curDir){

	char map[512];
	char files[512 * 2];
	char sectors[512];

	int baca = 1;
	int idx = 0;
	char option[20], pathParams[150];
	int i, j, k, success;

	clear(option, 20);

	readSector(map, 256);
	readSector(files, 257);
	readSector(files + 512, 258);
	readSector(sectors, 259);

	if(STATUS!='\0'){
		if(STATUS == 'U'){
			//int i;
			if(cnt ==-999){
				cnt = tail;
			}
			else{
				tail--;
				if(tail<0)
					tail +=100;
			}
			cnt = tail;
		}
		else if(STATUS == 'D'){
			//int i; 
			if(cnt !=-999){
				cnt = mod(cnt+1, 100);
			}
			if(cnt == tail)
				cnt = -999;
		}
		else if(STATUS == 'T'){
            showSuggestion(buff, option, files);
		}

		if(cnt!=-999){
			for(i=0;cmd[i]!='\0';i++){
			cmd[i] = history[tail][i];
			}
			cmd[i] = '\0';
		}
		else
		{
			cmd[0] = '\0';
		}
	}
	else{
		if(strcmp(buff,"halo")){
			printString("\n\r");
			printString("selamat datang di FNY OS, OS masa depan! :v");
			printString("\r");
		}
		else if(buff[0]=='e'&&buff[1]=='c'&&buff[2]=='h'&&buff[3]=='o'){
			echoText(buff, option);
		}

		else if(buff[0]=='m'&&buff[1]=='k'&&buff[2]=='d'&&buff[3]=='i'&&buff[4]=='r'){
            makeDir(buff, option, files);
		}

		else if(buff[0]=='.'&&buff[1]=='/'){
            executeFile(buff, option, files, curDir, &success);
		}

		else if(buff[0]=='c'&&buff[1]=='d'){
            changeDir(buff, files, curDir);
		}

		else if(buff[0]=='l'&&buff[1]=='s'){
			traverseDir();
		}

		else{
            executeInPath(buff, option, files, &success);
		}

		writeSector(map, 256);
		writeSector(files, 257);
		writeSector(files + 512, 258);
		writeSector(sectors, 259);

		//int i;
		tail = mod((tail+1),100);
		if(tail==head){
			head = mod((head+1),100);
		}
		for(i=0;cmd[i]!='\0';i++){
			history[tail][i] = cmd[i];
		}
		history[tail][i] = '\0';
		cnt = -999;
	}
	return;
}

void showSuggestion(char *buff, char *option, char *files){
    int idx, i, j;


}

void echoText(char *buff, char *option){
    int idx, j;
    if(buff[4]==0x0){
        printString("\r\n");
    } else if(buff[4]==' '){
        idx = 5;
        j = 0;
        while(buff[idx]!=0x0){
            option[j] = buff[idx];
            j++;
            idx++;
        }
        option[j] = 0x0;
        printString("\n\r");
        printString(option);
        printString("\r");
    } else {
        printString("Command '");
        printString(buff);
        printString("' tidak ditemukan!!!\n\r");
    }	
}

void makeDir(char *buff, char *option, char *files){
    int idx, i, j, entryIdx;
    if(buff[5]==0x0){
        printString("\r\n");
    } else if(buff[5]==' '){
        idx = 6;
        j = 0;
        while(buff[idx]!=0x0){
            option[j] = buff[idx];
            j++;
            idx++;
        }
        option[j] = 0x0;
        for(;j<14;j++){
            option[j] = 0x0;
        }

        for(i = 0; i < 512 * 2; i = i + 16){
            if(files[i]==0x0 && files[i+1]==0x0 && files[i+2]==0x0){
                entryIdx = i;
                break;
            }
        }

        if(i == 512 * 2){
            printString("Full memory:(\n\r");
            return;
        }

        files[entryIdx] = *curDir;
        files[entryIdx+1] = 0xff;
        for(i=0; i<14; i++){
            files[entryIdx + i + 2] = option[i];
        }
    } 
    else {
        printString("\n\rCommand '");
        printString(buff);
        printString("' tidak ditemukan!!!\n\r");
    }	
}

void executeFile(char *buff, char *option, char *files, char *curDir, int *success){
    int i, j=0, idx = 2;
    while(buff[idx]!=0x0){
        option[j] = buff[idx];
        j++;
        idx++;
    }
    for(;j<14;j++){
        option[j] = 0x0;
    }

    i = 0;
    while(i<64 && (files[i*16] != *curDir || cekNamaFile(files, option, i) == 0 || files[i*16+1] == 0xff)){
        i++;
    }
    if(i == 64) {
        printString("\r\n");
        printString("program tidak valid");
        return;
    } else {
        executeProgram(option, 0x4000, &success);
    }
}

void changeDir(char *buff, char *files, char *curDir){
    int dir[512];
    int i,j, idx;
    char filename[20];
    char pathParams[50];

    //for changing directories

    if(buff[2]==0x0){
        *curDir = 0xff;
    } else if(buff[2]==' '){
        idx = 3;
        j = 0;
        while(buff[idx]!=0x0){
            if(buff[idx]!='/'){
                pathParams[j] = buff[idx];
                j++;
            } else {
                pathParams[j] = 0x0;
                if(strcmp(pathParams,"..")){
                    if(*curDir != 0xff){
                        *curDir = files[(*curDir) * 16];
                    }
                } else {
                    i = 0;
                    while(i<64 && (files[i*16] != *curDir || files[i*16+1] != 0xff || cekNamaFile(files, pathParams, i) == 0)){
                        i++;
                    }
                    if(i == 64) {
                        printString("\r\n");
                        printString("path tidak valid");
                        break;
                    } else {
                        *curDir = i;
                    }
                }
                clear(pathParams);
                j = 0;
            }
            idx++;
        }
        pathParams[j] = 0x0;
        if(strcmp(pathParams,"..")){
            if(*curDir != 0xff){
                *curDir = files[(*curDir) * 16];
            }
        } 
        else {
            i = 0;
            while(i<64 && (files[i*16] != *curDir || files[i*16+1] != 0xff || cekNamaFile(files, pathParams, i) == 0)){
                i++;
            }
            if(i == 64) {
                printString("\r\n");
                printString("path tidak valid");
            } else {
                *curDir = i;
            }
        }
    }
}

void traverseDir(){
    readSector(map, 256);
    readSector(files, 257);
    readSector(files + 512, 258);
    readSector(sectors, 259);

    printString("\n\r");
    showFolderContent(*curDir);
}

void executeInPath(char *buff, char *option, char *files, int *success){
    int j=0, idx =0;
    while(buff[idx] != 0x0){
        option[j] = buff[idx];
        j++;
        idx++;
    }
    for(;j<14;j++){
        option[j]=0x0;
    }
    for(i=0; i<64; i++){
        if(files[i*16]==5 && cekNamaFile(files, option, i)){
            printString("\n\r");
            executeProgram(option, 0x4000, &success);
        }
    }
    if(success == 1){
        printString("\n\rCommand '");
        printString(buff);
        printString("' tidak ditemukan!!!\n\r");
    }
}



int strcmp(char* s1, char* s2){
	for(; *s1 == *s2; ++s1, ++s2){
		if(*s1 == 0x0 || *s2 == 0x0){
			break;
		}
	}
	if(*s1 != *s2) return 0;
	else return 1;
}

void charRemnant(char *str, char *target, int i){
	int j;
	for(j=0; str[i]!='\0'; j++){
		target[j] = str[i];
		i++; 
	}
	target[j] = '\0';
}

void showFolderContent(int curDir){
	char files[512 * 2];
	char filename[20];
	int i;
	int j;

	readSector(files, 257);
	readSector(files + 512, 258);

	for(i=0; i<1024; i = i+16){
		if(files[i] == curDir && files[i+2] != 0x0){
			int k = 0;
			for(j=i+2; j<i+16; j++){
				filename[k++] = files[j];
			}
			printString(filename);
			printString("\n\r");
		}
	}
}

int cekNamaFile(char* files, char* buff, int i){

	int j;
	char filename[20];
	int k = 0;

	for(j=2; j<16; j++){
		filename[k++] = files[i*16+j];
	}

	filename[k] = 0x0;

	if(strcmp(filename, buff)){
		return 1;
	} else {
		return 0;
	}
}

void printNumber(int a){

	char pad = 0xe;
	int digit[10];

	int satuan = mod(a, 10);
	int temp = div(a, 10);
	int puluhan = mod(temp, 10);
	int ratusan = div(temp, 10);

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

	interrupt(0x10, digit[ratusan], 0x0, 0x0, 0x0); // interrupt digit puluhan
	interrupt(0x10, digit[puluhan], 0x0, 0x0, 0x0); // interrupt digit satuan
	interrupt(0x10, digit[satuan], 0x0, 0x0, 0x0); // interrupt digit satuan
 	printString("\r\n");
  
}

void generatePath(char curDir){
	char listedPath[1000];
	char files[512 * 2];
	char path[100];
	int i,j,k;
	i = 0;

	readSector(files, 257);
	readSector(files + 512, 258);

	while(curDir != 0xFF){
		path[i++] = curDir;
		curDir = files[curDir * 16];
	}

	j = 0;
	while(i > 0){
		listedPath[j++] = '/';
		for(k = 0; k < 14 && files[path[i-1]*16 + 2 + k] != 0x0;k++){
			listedPath[j++] = files[path[i-1] * 16 + 2 + k];
		}
		i--;
	}

	if(j>0){
		printString(listedPath);
	}else{
		printString("/");
	}
}