// loadFile.c
// Michael Black, 2007
//
// Loads a file into the file system
// This should be compiled with gcc and run outside of the OS

#include <stdio.h>

void main(int argc, char* argv[]) {
  int i;

  if (argc < 2) {
    printf("Specify file name to load\n");
    return;
  }

  // open the source file
  FILE* loadFil;
  loadFil = fopen(argv[1], "r");
  if (loadFil == 0) {
    printf("File not found\n");
    return;
  }

  // open the floppy image
  FILE* floppy;
  floppy = fopen("system.img", "r+");
  if (floppy == 0) {
    printf("system.img not found\n");
    return;
  }

  // load the disk map
  char map[512];
  fseek(floppy, 512 * 0x100, SEEK_SET);
  for (i = 0; i < 512; i++) map[i] = fgetc(floppy);

  // load the directory
  char files[512 * 2];
  fseek(floppy, 512 * 0x101, SEEK_SET);
  for (i = 0; i < 512 * 2; i++) files[i] = fgetc(floppy);

  char sectors[512];
  fseek(floppy,512 * 0x103, SEEK_SET);
  for(i = 0;i < 512;i++) sectors[i] = fgetc(floppy);

  
  // find a free entry in the directory
  for (i = 0; i < 512 * 2; i = i + 0x10)
    if (files[i] == 0 && files[i+1] == 0 && files[i + 2] == 0) break;
    
  if (i == 512 * 2) {
    printf("Not enough room in directory lol\n");
    return;
  }

  int dirindex = i;

  int entryIndex;
  // cari indeks sektor yang kosong pada sectors
  for(i = 0; i < 32; i++){
    if(sectors[i * 16] == 0){
      entryIndex = i;
      break;
    }
  }
  if(i == 32){
    printf("Tidak ada entry kosong!\n");
    return;
  }

  files[dirindex + 1] = entryIndex;
  files[dirindex] = 0xff; //asumsi parentnya root

  // fill the name field with 00s first
  for (i = 0; i < 14; i++) files[dirindex + i + 2] = 0x00;
  // copy the name over
  for (i = 0; i < 14; i++) {
    if (argv[1][i] == 0) break;
    files[dirindex + i + 2] = argv[1][i];
  }

  // find free sectors and add them to the file
  int sectcount = 0;
  int index = 0;
  while (!feof(loadFil)) {
    if (sectcount == 16) {
      printf("Not enough space in directory entry for file\n");
      return;
    }

    // find a free map entry
    for (i = 0; i < 512; i++)
      if (map[i] == 0) break;
    if (i == 512) {
      printf("Not enough room for file\n");
      return;
    }

    // mark the map entry as taken
    map[i] = 0xFF;

    // mark the sector in the directory entry
    sectors[entryIndex * 16 + index] = i;
    index++;
    sectcount++;

    printf("Loaded %s to sector %d\n", argv[1], i);

    // move to the sector and write to it
    fseek(floppy, i * 512, SEEK_SET);
    for (i = 0; i < 512; i++) {
      if (feof(loadFil)) {
        fputc(0x0, floppy);
        break;
      } else {
        char c = fgetc(loadFil);
        fputc(c, floppy);
      }
    }
  }

  // write the map and directory back to the floppy image
  fseek(floppy, 512 * 0x100, SEEK_SET);
  for (i = 0; i < 512; i++) fputc(map[i], floppy);

  fseek(floppy, 512 * 0x101, SEEK_SET);
  for (i = 0; i < 512 * 2; i++) fputc(files[i], floppy);

  fseek(floppy, 512 * 0x103, SEEK_SET);
  for(i = 0; i < 512 ; i++) fputc(sectors[i], floppy);

  fclose(floppy);
  fclose(loadFil);
}
