void echoText(char* buff, char* option);
void makeDir(char *buff, char *option, char *files, char *curDir);
void executeFile(char *buff, char *option, char *files, char *curDir, int *success);
void changeDir(char *buff, char *files, char *curDir);
void traverseDir(int curDir);

char stat;
char history[100][100];
char cmd[100];
int cnt=-999, head = 0, tail=0;

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

	if(stat!='\0'){
		if(stat == 'U'){
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
		else if(stat == 'D'){
			//int i; 
			if(cnt !=-999){
				cnt = mod(cnt+1, 100);
			}
			if(cnt == tail)
				cnt = -999;
		}
		else if(stat == 'T'){

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
			makeDir(buff, option, files, *curDir);
		}

		else if(buff[0]=='.'&&buff[1]=='/'){
			executeFile(buff, option, files, curDir, &success);
		}

		else if(buff[0]=='c'&&buff[1]=='d'){
			changeDir(buff, files, curDir);
		}

		else if(buff[0]=='l'&&buff[1]=='s'){
			traverseDir(curDir);
		}

		else if(buff[0]=='m'&&buff[1]=='v'){

			char pathParams1[20];
			char pathParams2[20];
			int idx1, idx2;
			int sect;

			//for moving file or renaming file if folder doesn't exist

			if(buff[2]==' '){
				idx = 3;
				j = 0;
				while(buff[idx]!=' '){
					pathParams1[j] = buff[idx];
					j++;
					idx++;
				}
				for(;j<14;j++){
					pathParams1[j] = 0x0;
				}
				idx++;
				j = 0;
				while(buff[idx]!=0x0){
					pathParams2[j] = buff[idx];
					j++;
					idx++;
				}
				for(;j<14;j++){
					pathParams2[j] = 0x0;
				}

				//cari indeks dari file bernama pathParams1 di folder as a (if gaada = -1)
				//cari indeks dari file bernama pathParams2 di folder as b (if gaada = -1)

				idx1 = -1;
				idx2 = -1;

				// printNumber(curDir);

				for(i=0; i<64; i++){
					// if(files[i*16] == curDir && files[i*16+2] != 0x0){
					// 	printString(pathParams1);
						if(cekNamaFile(files, pathParams1, i) == 1) {
							idx1 = i;
						}
						if (cekNamaFile(files, pathParams2, i) == 1) {
							idx2 = i;
						}
					// }
				}

				//kalau pathParams1 ga ada, keluarin pesan error no such file or directory
				//else 
					// kalau pathParams2 ga ada, ubah nama file di indeks a dari pathParams1 jadi pathParams2
					// kalau ada, bagi kasus:
						// kalau pathParams1 itu folder, pathParams2 juga folder, pindahing parent a jadi b
						// kalau pathParams1 itu folder, pathParams2 file, gabisa
						// kalau pathParams1 itu file, pathParams2 folder, pindahin parent a jadi b
						// kalau dua2nya file, isinya pathParams2 pindah ke pathParams1, caranya pin
								//ubah nama file pathParams1 jadi pathParams2, hapus file pathParams2

				// printNumber(idx1);
				// printNumber(idx2);

				if(idx1 == -1){
					printString("\n\rfile/folder ");
					printString(pathParams1);
					printString(" tidak ditemukan\n\r");
				} else {
					if(idx2 == -1){
						//ganti nama aja
						for(j=0; j<14; j++){
							files[idx1*16+j+2] = pathParams2[j];
						}
					} else {
						if(files[idx1*16+1] == 0xff && files[idx2*16+1] == 0xff){
							files[idx1*16] = idx2;
							// printString("pemindahan berhasil\n\r");
						} else if(files[idx1*16+1] == 0xff && files[idx2*16+1] != 0xff){
							printString("Tidak bisa melakukan mv dari folder ke file, gomennasai\n\r");
						} else if(files[idx1*16+1] != 0xff && files[idx2*16+1] == 0xff){
							files[idx1*16] = idx2;
						} else {
							for(j=0; j<14; j++){
								files[idx1*16+j+2] = pathParams2[j];
							}
							deleteFile(pathParams2, &sect, curDir);
							// for(j=0; j<16; j++){
							// 	files[]
							// }
							// deleteFile(pathParams2, sectors, curDir);
							// printString("under construction");
						}
					}
				}
			}
		}

		else if(buff[0]=='c'&&buff[1]=='p'){

			//belum support nerima path sama copy entire directory (yg butuh flag -r)
			char pathParams1[20];
			char pathParams2[20];
			int idx1, idx2;

			//for moving file or renaming file if folder doesn't exist

			if(buff[2]==' '){
				idx = 3;
				j = 0;
				while(buff[idx]!=' '){
					pathParams1[j] = buff[idx];
					j++;
					idx++;
				}
				for(;j<14;j++){
					pathParams1[j] = 0x0;
				}
				idx++;
				j = 0;
				while(buff[idx]!=0x0){
					pathParams2[j] = buff[idx];
					j++;
					idx++;
				}
				for(;j<14;j++){
					pathParams2[j] = 0x0;
				}

				//cari indeks dari file bernama pathParams1 di folder as a (if gaada = -1)
				//cari indeks dari file bernama pathParams2 di folder as b (if gaada = -1)

				idx1 = -1;
				idx2 = -1;

				for(i=0; i<64; i++){
					// if(files[i*16] == curDir && files[i*16+2] != 0x0){
					// 	printString(pathParams1);
						if(cekNamaFile(files, pathParams1, i) == 1) {
							idx1 = i;
						}
						if (cekNamaFile(files, pathParams2, i) == 1) {
							idx2 = i;
						}
					// }
				}

				if(idx1 == -1){
					printString("\n\rfile/folder ");
					printString(pathParams1);
					printString(" tidak ditemukan\n\r");
				} else {
					if(idx2 == -1){
						if(files[idx1*16+1]==0xff){ //kalau dia merupakan folder
							printString("maaf, belum support copy folder\r\n");
						} else if(files[idx2*16+1]!=0xff){
							
						}
					} else {
						if(files[idx1*16+1] == 0xff && files[idx2*16+1] == 0xff){
							files[idx1*16] = idx2;
							// printString("pemindahan berhasil\n\r");
						} else if(files[idx1*16+1] == 0xff && files[idx2*16+1] != 0xff){
							printString("Tidak bisa melakukan mv dari folder ke file, gomennasai\n\r");
						} else if(files[idx1*16+1] != 0xff && files[idx2*16+1] == 0xff){
							files[idx1*16] = idx2;
						} else {
							for(j=0; j<14; j++){
								files[idx1*16+j+2] = pathParams2[j];
							}
							// for(j=0; j<16; j++){
							// files[]
							// }
							// deleteFile(pathParams2, sectors, curDir);
							// printString("under construction");
						}
					}
				}
			}
		}

		else if(buff[0]=='c'&&buff[1]=='a'&&buff[2]=='t'){

			int dir[512];
			int i;
			char filename[50];
			char bufferfile[16 * 512];
			int posSector;

			// printString("debug\n\r");

			if(buff[3]==' '){
				idx = 4;
				j = 0;
				while(buff[idx]!=0x0){
					filename[j] = buff[idx];
					j++;
					idx++;
				}
				for(;j<50;j++){
					filename[j] = 0x0;
				}

				idx = -1;

				for(i=0; i<64; i++){
					if(cekNamaFile(files, filename, i) == 1) {
						idx = i;
					}
				}

				if(idx == -1){
					printString("\n\rfile tidak ditemukan\n\r");
				} else if(files[idx*16+1] == 0xff){
					printString("tidak bisa melihat isi folder dengan command cat\n\r");
				} else {
					posSector = files[idx*16+1];
					j = 0;
					i = 0;
					while(sectors[posSector * 16 + j] != 0x0){
						readSector(bufferfile + 512 * i, sectors[posSector * 16 + j]);
						j++;
						i++;
					}
					printString("\n\r");
					printString(bufferfile);
				}
				//cari file di current directory yg namanya sesuai sama filename
				//readsector isi filenya ke array buff
				//printString buff
			}
		}

		else if(buff[0]=='r'&&buff[1]=='m'){
			//to be continued...
		}

		else{

			j=0;
			idx=0;
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
			printString("\n\rCommand '");
			printString(buff);
			printString("' tidak ditemukan!!!\n\r");
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

void makeDir(char *buff, char *option, char *files, int curDir){
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

        files[entryIdx] = curDir;
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

void traverseDir(int curDir){
    readSector(map, 256);
    readSector(files, 257);
    readSector(files + 512, 258);
    readSector(sectors, 259);

    printString("\n\r");
    showFolderContent(curDir);
}