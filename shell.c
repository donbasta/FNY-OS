// void print(char buff);
void bacaInput(char *buff);
// int div(int a, int b);
int strcmp(char* s1, char* s2);
// void printChar(char ch);
// void clear(char*, int);

bool isDir(char *dirPath);

bool isFile(char *filePath);

void charRemnant(char *str, char *target, int i);

main(){
	char buff[100];
	char path[200];
	char path[0]='/';
	char path[1]='\0';
	//for(int i=0;i<200;i++)
	printString("Selamat datang di shell v.0.1");
	//printString("\n\r");
	//printString("fny_os@bapak_imba:");

	while(1){

		interrupt(0x21,1,buff,0,0);
		printString("\n\r");
		printString("fny_os@bapak_imba:");
		printString(path);
		bacaInput(buff, path);
		// clear(buff,100);

	}

}

void bacaInput(char* buff, char *path){

	int baca = 1;
	int idx = 0;
	char option[20];
	int j;
	char pathParams[150];

	//baca string command karakter per karakter
	while(baca){
		if(buff[idx]==0x0){
			baca = 0; //selesai baca command
		} else {
			idx = idx+1;
		}
	}

	if(strcmp(buff,"halo")){
		printString("\n\r");
		printString("selamat datang di FNY OS, OS masa depan! :v");
		printString("\r");
	}

	if(buff[0]=='e'&&buff[1]=='c'&&buff[2]=='h'&&buff[3]=='o'&&buff[4]==' '){
		idx = 5;
		j = 0;
		while(buff[idx]!=0x0){
			option[j] = buff[idx];
			j++;
			idx++;
		}
		option[j] = 0x0;
		// printString(option[0]);

		// printString("hahaha");
		printString("\n\r");
		// printString("aha2");
		printString(option);
		// printString("aha3");
		printString("\r");
	}

	else if(buff[0]=='.'&&buff[1]=='/'){
		//for running program inside directory
		charRemnant(buff, pathParams, 2);
		
	}

	else if(buff[0]=='c'&&buff[1]=='d'&&buff[2]==' '){
		//for changing directories
		idx = 3;
		j = 0;
		while(buff[idx]!=0x0){
			option[j] = buff[idx];
			j++;
			idx++;
		}
		option[j] = 0x0;
	}

	else if(buff[0]=='l'&&buff[1]=='s'&&buff[2]==' '){
		//showing all files/folders inside a directory
		if(buff[2]==' '){

		}
		else if (buff[2]=='\0'){

		}
		else{
			
		}
	}

	else if(buff[0]=='c'&&buff[1]=='a'&&buff[2]=='t'&&buff[3]=' '){
		//nge cat file
		if(buff[2]==' '){

		}
		else if (buff[2]=='\0'){

		}
		else{
			
		}
	}
	else if(buff[0]=='r'&&buff[1]=='m'){
		//delete file in directory
		if(buff[2]==' '){

		}
		else if (buff[2]=='\0'){

		}
		else{

		}
	}
	else{
		printString("Command ");
		printString(buff);
		printString(" tidak ditemukan!!!\n");
	}


	return;

}

int strcmp(char* s1, char* s2){
	for(; *s1 == *s2; ++s1, ++s2){
		if(*s1 == 0x0 || *s2 == 0x0){
			// return 0; //yang satu adalah prefix dari yang satunya
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

bool isDir(char *dirPath){

}

bool isFile(char *filePath){

}