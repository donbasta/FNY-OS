char stat;

void printString(char *string)
{
  char ah = 0xe;
  int al;
  int ax;

  while (*string != '\0')
  {
    al = *string;
    ax = ah * 256 + al;
    interrupt(0x10, ax, 0x0, 0x0, 0x0);
    // if (al == 0xd) interrupt(0x10, 0xe*0x100+10, 0x0, 0x0);
    ++string;
  }

  return;
}

void readString(char *string)
{
  int ptr = 0;
  int i;
  char ch = 0x0;
  stat = '\0';

  i = 0;
  while(string[i] != 0x0){
    ptr++;
    i++;
  }
  
  while (1)
  {
    ch = interrupt(0x16, 0x0, 0x0, 0x0, 0x0);
    if (ch == 0xd){
      stat = '\0';
      string[ptr] = '\0';
      break;
    }
       //jika user menekan tombol enter
    if (ch == 0x8)
    {
      if (ptr > 0)
      {
        interrupt(0x10, 0xe * 0x100 + ch, 0x0, 0x0, 0x0);
        interrupt(0x10, 0xe * 0x100 + 32, 0x0, 0x0, 0x0);
        interrupt(0x10, 0xe * 0x100 + ch, 0x0, 0x0, 0x0);
        --ptr;
      }
      else
        continue;
    }
    else if(ch == 0x0){ // kalau neken tombol up
        stat = 'U';
        break;
    }
    // else if(ch == 0x8){ // kalau neken tombol down
    //     stat = 'D';
    //     break;
    // }
    else if(ch == 0x9){ // kalau neken tombol tab
        stat = 'T';
        string[ptr] = '\0';
        break;
    }
    // else if(ch == 0x8){ // kalau neken tombol <-
        
    // }
    // else if(ch == 0x8){ // kalau neken tombol ->
        
    // }
    else
    {
      string[ptr++] = ch;
      interrupt(0x10, 0xe * 0x100 + ch, 0x0, 0x0, 0x0);
    }
  }
}
