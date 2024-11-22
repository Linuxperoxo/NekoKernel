void k_print(char* __text__)
{
  unsigned char* __video_mem = (unsigned char*) 0xb8000;

  for(unsigned int __i = 0; *__text__ != '\0'; __i++)
  {
    __video_mem[__i] = *__text__;
    ++__text__;
    ++__i;
    __video_mem[__i] = 0x07;
  }
}

void k_clean(void)
{
  unsigned char* __video_mem = (unsigned char*) 0xb8000;

  for(unsigned int __i = 0; __i < (80*25*2); __i++)
  {
    __video_mem[__i] = ' ';
    ++__i;
    __video_mem[__i] = 0x07;
  }
}

void k_main(void)
{
  k_clean();
  k_print("Hello,World!\0");
  while(1);
}
