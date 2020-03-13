void bacaInput(char *buff);
int isDir(char *dirPath);
int isFile(char *filePath);
void charRemnant(char *str, char *target, int i);
void showFolderContent(char* str);
int cekNamaFile(char* files, char* buff, int i);
void printNumber(int a);

char buff[100];
char absPath[100];

int main(){

	char curDir;

	printString("\n\n\r");
	printString("Selamat datang di shell v.0.1\n\r");
	printString("fny_os@bapak_imba:");
	printString("/");
	printString("$ ");

	curDir = 0xff;

	while(1){

		interrupt(0x21,1,buff,0,0);
		bacaInput(buff, &curDir);
		printString("\n\r");
		printString("fny_os@bapak_imba:");
		// printString(path(curDir));
		printString("$ ");
		clear(buff,100);
		// printString(buff);
		printNumber(curDir);

	}

}

void bacaInput(char* buff, char* curDir){

	char map[512];
  char files[512 * 2];
  char sectors[512];

	int baca = 1;
	int idx = 0;
	char option[20];
	int i;
	int j;
	int k;
	char pathParams[150];

	readSector(map, 256);
	readSector(files, 257);
	readSector(files + 512, 258);
	readSector(sectors, 259);

	// while(baca){
	// 	if(buff[idx]==0x0){
	// 		baca = 0;
	// 	} else {
	// 		idx = idx+1;
	// 	}
	// }

	if(strcmp(buff,"halo")){
		printString("\n\r");
		printString("selamat datang di FNY OS, OS masa depan! :v");
		printString("\r");
	}

	else if(buff[0]=='e'&&buff[1]=='c'&&buff[2]=='h'&&buff[3]=='o'){
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

	else if(buff[0]=='m'&&buff[1]=='k'&&buff[2]=='d'&&buff[3]=='i'&&buff[4]=='r'){

		int entryIdx;

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



		} else {
			printString("Command '");
			printString(buff);
			printString("' tidak ditemukan!!!\n\r");
		}	
	}

	else if(buff[0]=='.'&&buff[1]=='/'){
		//for running program inside directory
		charRemnant(buff, pathParams, 2);
		
	}

	else if(buff[0]=='c'&&buff[1]=='d'){

		int dir[512];
		int i;
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
					idx++;
				} else {
					pathParams[j] = 0x0;
					if(strcmp(pathParams,"..")){
						*curDir = files[(*curDir) * 16];
					} else {
						i = 0;
						while(i<64 && cekNamaFile(files, pathParams, i) == 0){
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
					idx++;
				}
			}
			pathParams[j] = 0x0;
			if(strcmp(pathParams,"..")){
				*curDir = files[(*curDir) * 16];
			} else {
				i = 0;
				while(i<64 && files[i*16] != *curDir && cekNamaFile(files, pathParams, i) == 0){
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

	else if(buff[0]=='l'&&buff[1]=='s'){

		readSector(map, 256);
	  readSector(files, 257);
	  readSector(files + 512, 258);
	  readSector(sectors, 259);

	  printString("\n\r");
	  showFolderContent(*curDir);

	}

	// else if(buff[0]=='r'&&buff[1]=='m'){

	// 	readSector(map, 256);
	//   readSector(files, 257);
	//   readSector(files + 512, 258);
	//   readSector(sectors, 259);

	// 	//delete file in directory
	// 	if(buff[2]==' '){

	// 		idx = 3;
	// 		j = 0;
	// 		while(buff[idx]!=0x0){
	// 			option[j] = buff[idx];
	// 			j += 1;
	// 			idx += 1;
	// 		}
	// 		option[j] = 0x0;

	// 		//cari file bernama option di folder saat ini. Kalau ada, delete file.

	// 	}
	// 	else if (buff[2]=='\0'){
	// 		printString("rm: missing operand");
	// 		printString("\r\n");
	// 		printString("Try 'rm --help' for more information.");
	// 		printString("\r\n");
	// 	}

	// }

	// else if(buff[0]=='m'&&buff[1]=='k'&&buff[2]=='d'&&buff[3]=='i'&&buff[4]=='r'){

	// 	int entryIdx;

	// 	idx = 6;
	// 	j = 0;


	// 	if(buff[5]==' '){
	// 		while(buff[idx]!=0x0){
	// 			option[j] = buff[idx];
	// 			idx += 1;
	// 			j += 1;
	// 		}
	// 		option[j] = 0x0;

	// 		printString(option);


	// 		//create folder dengan nama option

	// 		for(i = 0; i < 64; i+=16){
	// 			if(files[i]==0x0 && files[i+1]==0x0 && files[i+2]==0x0){
	// 				entryIdx = i;
	// 				break;
	// 			}
	// 		}

	// 		if(i == 64){
	// 			printString("Full memory:(\n\r");
	// 			return;
	// 		}

	// 		files[entryIdx] = curDir;
	// 		files[entryIdx+1] = 0xff;
	// 		for(i=0; i<14; i++){
	// 			files[entryIdx + i + 2] = option[i];
	// 		}

	// 	} else if(buff[5]==0x0){
	// 		printString("mkdir: missing operand");
	// 		printString("\r\n");
	// 		printString("Try 'mkdir --help' for more information.");
	// 		printString("\r\n");
	// 	}

	
	// }

	else{
		printString("\n\rCommand '");
		printString(buff);
		printString("' tidak ditemukan!!!\n\r");
	}

		writeSector(map, 256);
	  writeSector(files, 257);
	  writeSector(files + 512, 258);
	  writeSector(sectors, 259);


	return;

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

// char* append(char* s1, char* s2){
// 	char[200] s_res;

// 	int j = 0;
// 	int idx = 0;

// 	while(s1[idx]!=0x0){
// 		s_res[j] = s1[idx];
// 		j++;
// 		idx++;
// 	}

// 	while(s2[idx]!=0x0){
// 		s_res[j] = s2[idx];
// 		j++;
// 		idx++;
// 	}

// 	s_res[j] = 0x0;
// }

void charRemnant(char *str, char *target, int i){
	int j;
	for(j=0; str[i]!='\0'; j++){
		target[j] = str[i];
		i++; 
	}
	target[j] = '\0';
}





// bool isDir(char *dirPath){
// }




// bool isFile(char *filePath){
// }


// char* path(int i){

//   char files[512 * 2];
//   int par;
//   char absolutePath[200];

//   absolutePath[0] = '/';

// 	readSector(files, 257);
// 	readSector(files + 512, 258);

// 	while(i != 0xff){

// 		i = files[i * 16];
		

// 	}

// 	return absolutePath;

// }

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
	// files[i*16+j] = 0x0;

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