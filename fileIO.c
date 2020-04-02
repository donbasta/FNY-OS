void deleteFile(char *path, int *sectors, char parentIndex){
  char map[512];
  char files[512 * 2];
  char sectors[512];

  int i,j,k,l,m;
  int beda;

  int sama;
  int cnt;
  int sect;
  int entryIndex;
  char idxParent;
  char parent[14];
  char filename[14];
  readSector(map, 256);
  readSector(files, 257);
  readSector(files + 512, 258);
  readSector(sectors, 259);

  for(i = 0;i<14;i++){
    parent[i] = 0x0;
    filename[i] = 0x0;
  }

  // mengambil current file name dan current parent name
  j = 0;
  i = 0;
  idxParent = parentIndex;
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
      for(l = 0;l < 64;l++){
        if(files[l*16 + 1] == 0xFF){
          beda = 0;
          for(m = 0;m<14;m++){
            if(files[l*16 + 2 + m] != parent[m]){
              beda = 1;
              break;
            }
          }

          if(beda) continue;

          if(files[l*16] == idxParent){
            idxParent = files[l*16];
          }else{
            *sectors = -1;
            printString("File tidak ditemukan\n");
            return;
          }
        }
      }
    }
    i++;
  }

  // pad with 0
  for(;j<14;j++){
    filename[j] = 0x0;
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
      // hapus entri pada files
      for(k = 0;k<16;k++){
        files[i*16 + k] = 0x0;
      }
      break;
    }
  }

  if(!sama){
    *sectors = -1;
    printString("Tidak ada file yang memenuhi!\n");
    return;
  }

  // hapus entri pada sectors dan map
  for(i = 0;i<16;i++){
    sect = sectors[entryIndex*16 + i];
    sectors[entryIndex*16 + i] = 0x0;
    if(sect == 0){
      break;
    }

    map[sect] = 0x0;
  }

  writeSector(map, 256);
  writeSector(files, 257);
  writeSector(files + 512, 258);
  writeSector(sectors, 259);
}

void writeFile(char *buffer, char *path, int *sectors, char parentIndex){
  char map[512];
  char files[512 * 2];
  char sectors[512];
  char freeSector[512];

  int i,j,k;
  int entryIndex;
  int entryFile;
  int sama;
  int beda;

  char parent[14];
  char filename[14];
  char idxParent;

  readSector(map, 256);
  readSector(files, 257);
  readSector(files + 512, 258);
  readSector(sectors, 259);

  for(i = 0;i<14;i++){
    parent[i] = 0x0;
    filename[i] = 0x0;
  }
  
  // mengambil current file name dan current parent name
  j = 0;
  for(i = 0 ;i < sizeof(path); i++){
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
  }
  // pad with 0
  for(;j<14;j++){
    filename[j] = 0x0;
  }

  // mencari index parent
  idxParent = parentIndex;
  if(parent[0] != 0x0){
    for(i = 0;i<64;i++){
      if(files[i*16 + 1] == 0xFF){
        int beda = 0;
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
      break;
    }
  }

  if(sama){
    *sectors = -1;
    printString("File sudah ada!\n");
    return;
  }

  // mencari sector index yang kosong
  for(i = 0;i<32;i++){
    if(sectors[i*16]==0x0){
      entryIndex = i;
      break;
    }
  }

  // cek apakah terdapat entri yang masih kosong
  entryFile = -1;
  for(i = 0;i<64;i++){
    if(files[i*16 + 2] == 0x0){
      entryFile = i;
      break;
    }
  }

  if(entryFile == -1){
    *sectors = -2;
    printString("Tidak cukup entri di files");
    return;
  }
  // cek sector yang masih kosong
  j = 0;
  for(i = 0;i<512;i++){
    if(map[i] == 0x0){
      freeSector[j++] = i;
    }
  }

  // sector yang dibutuhkan oleh buffer

  k = 0;
  i = 0;
  while(buffer[k] != 0x0){
    i++;
    k+=512;
  }

  if(i > j){
    *sectors = -3;
    printString("Alokasi gagal!!\n");
    return;
  }

  files[entryFile*16] = idxParent;
  files[entryFile*16 + 1] = entryIndex;

  for(i = 0; i<14;i++){
    files[entryFile*16 + 2 + i] = filename[j];
  }

  // mengisi sector yang kosong pada sectors dengan indeks entryIndex;
  j = 0;
  i = 0;
  while(buffer[i] != 0x0){
    map[freeSector[j]] = 0xFF;
    sectors[entryIndex*16 + j] = freeSector[j];
    writeSector(buffer + i, freeSector[j]);
    j++;
    i += 512;
  }

  *sectors = j;

  // pad with 0
  for(; j<16;j++){
    sectors[entryIndex*16 + j] = 0x0;
  }

  writeSector(map, 256);
  writeSector(files, 257);
  writeSector(files + 512, 258);
  writeSector(sectors, 259);
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
    *result = -1;
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
