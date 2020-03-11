void bacaInput(char *buff);
bool isDir(char *dirPath);
bool isFile(char *filePath);
void charRemnant(char *str, char *target, int i);
void showFolderContent(char* str)




main(){
	char buff[100];
	char path[200];
	char path[0]='/';
	char path[1]='\0';

	printString("Selamat datang di shell v.0.1\n\r");

	char map[512];
  char files[512 * 2];
  char sectors[512];


	while(1){

		interrupt(0x21,1,buff,0,0);
		printString("\n\r");
		printString("fny_os@bapak_imba:");
		printString(path);
		printString("$ ");
		bacaInput(buff, path);

	}

}

void bacaInput(char* buff, char *path){

	char map[512];
  char files[512 * 2];
  char sectors[512];

	int baca = 1;
	int idx = 0;
	char option[20];
	int j;
	int k;
	char pathParams[150];

	while(baca){
		if(buff[idx]==0x0){
			baca = 0;
		} else {
			idx = idx+1;
		}
	}

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

	else if(buff[0]=='.'&&buff[1]=='/'){
		//for running program inside directory
		charRemnant(buff, pathParams, 2);
		
	}

	else if(buff[0]=='c'&&buff[1]=='d'){

		int dir[512];

		readSector(map, 256);
	  readSector(files, 257);
	  readSector(files + 512, 258);
	  readSector(sectors, 259);

		//for changing directories

	  if(buff[2]==0x0){
	  	//langsung ke home, ubah si path
	  } else if(buff[2]==' '){
	  	idx = 3;
			j = 0;
			while(buff[idx]!=0x0){
				pathParams[j] = buff[idx];
				j++;
				idx++;
			}
			pathParams[j] = 0x0;

			j=0;

			while(pathParams[j]!=0x0){

				k = 0;
				while(pathParams[j]!='/' && pathParams[j]!=0x0){
					dir[k] = pathParams[j];
					k++;
					j++;
				}
				pathParams[k] = 0x0;
				j++;

				//cari pathParams di current path;
				//kalau ada update pathnya, kalau nggak, error

			}
	  }

		


	}

	else if(buff[0]=='l'&&buff[1]=='s'&&buff[2]==' '){


		readSector(map, 256);
	  readSector(files, 257);
	  readSector(files + 512, 258);
	  readSector(sectors, 259);


		//showing all files/folders inside a directory
		if(buff[2]==' '){

		}
		else if (buff[2]=='\0'){

		}
		else{
			
		}
	}

	else if(buff[0]=='c'&&buff[1]=='a'&&buff[2]=='t'&&buff[3]=' '){

		readSector(map, 256);
	  readSector(files, 257);
	  readSector(files + 512, 258);
	  readSector(sectors, 259);

		//nge cat file
		if(buff[2]==' '){

		}
		else if (buff[2]=='\0'){

		}
		else{
			
		}
	}

	else if(buff[0]=='r'&&buff[1]=='m'){

		readSector(map, 256);
	  readSector(files, 257);
	  readSector(files + 512, 258);
	  readSector(sectors, 259);

		//delete file in directory
		if(buff[2]==' '){

			idx = 3;
			j = 0;
			while(buff[idx]!=0x0){
				option[j] = buff[idx];
				j += 1;
				idx += 1;
			}
			option[j] = 0x0;

			//cari file bernama option di folder saat ini. Kalau ada, delete file.

		}
		else if (buff[2]=='\0'){
			printString("rm: missing operand");
			printString("\r\n");
			printString("Try 'rm --help' for more information.");
			printString("\r\n");
		}

	}

	else if(buff[0]=='m'&&buff[1]=='k'&&buff[2]=='d'&&buff[3]=='i'&&buff[4]=='r'){

		readSector(map, 256);
	  readSector(files, 257);
	  readSector(files + 512, 258);
	  readSector(sectors, 259);

		idx = 6;
		j = 0;

		if(buff[5]==' '){
			while(buff[idx]!=0){
				option[j] = buff[idx];
				idx += 1;
				j += 1;
			}
			option[j] = 0x0;


			//create folder dengan nama option


		} else if(buff[5]==0x0){
			printString("mkdir: missing operand");
			printString("\r\n");
			printString("Try 'mkdir --help' for more information.");
			printString("\r\n");
		}

		

	}

	else{
		printString("Command '");
		printString(buff);
		printString("' tidak ditemukan!!!\n\r");
	}


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

char* append(char* s1, char* s2){
	char[200] s_res;

	int j = 0;
	int idx = 0;

	while(s1[idx]!=0x0){
		s_res[j] = s1[idx];
		j++;
		idx++;
	}

	while(s2[idx]!=0x0){
		s_res[j] = s2[idx];
		j++;
		idx++;
	}

	s_res[j] = 0x0;
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







// void showFolderContent(char* ){
// }



// char* path(int i){

// 	// char map[512];
//   char files[512 * 2];
//   // char sectors[512];

//   int par;
//   char absolutePath[200];

// 	// readSector(map, 256);
// 	readSector(files, 257);
// 	readSector(files + 512, 258);
// 	// readSector(sectors, 259);

// 	while(par != 0xff){

// 		// absolutePath = / + namafile ke i + absolutePath

// 		par = files[i * 16];
// 		i = par;

// 	}

// 	return absolutePath;


// }

