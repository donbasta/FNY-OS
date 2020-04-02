void clear(char *buffer, int length)
{
  int i;

  for (i = 0; i < length; i++)
  {
    buffer[i] = 0x00;
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

void showFolderContent(int curDir){
	char files[512 * 2];
	char filename[20];
	int i;
	int j;

	readSector(files, 257);
	readSector(files + 512, 258);

	printString("\n\r");
    for(i=0; i<1024; i = i+16){
		if(files[i] == curDir){
			printString(files + i + 2);
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
	for(;k<20;k++){
		filename[k] = 0x0;
	}
	if(strcmp(filename, buff)){
		return 1;
	} else {
		return 0;
	}
}

// void printNumber(int a){

// 	char pad = 0xe;
// 	int digit[10];
// 	int satuan = mod(a, 10);
// 	int temp = div(a, 10);
// 	int puluhan = mod(temp, 10);
// 	int ratusan = div(temp, 10);
// 	digit[0] = pad * 0x100 + '0';
// 	digit[1] = pad * 0x100 + '1';
// 	digit[2] = pad * 0x100 + '2';
// 	digit[3] = pad * 0x100 + '3';
// 	digit[4] = pad * 0x100 + '4';
// 	digit[5] = pad * 0x100 + '5';
// 	digit[6] = pad * 0x100 + '6';
// 	digit[7] = pad * 0x100 + '7';
// 	digit[8] = pad * 0x100 + '8';
// 	digit[9] = pad * 0x100 + '9';
// 	interrupt(0x10, digit[ratusan], 0x0, 0x0, 0x0); // interrupt digit puluhan
// 	interrupt(0x10, digit[puluhan], 0x0, 0x0, 0x0); // interrupt digit satuan
// 	interrupt(0x10, digit[satuan], 0x0, 0x0, 0x0); // interrupt digit satuan
//  	printString("\r\n");
// }

void generatePath(char curDir){
	char listedPath[1000];
	char files[512 * 2];
	char path[100];
	int i=0,j,k;

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