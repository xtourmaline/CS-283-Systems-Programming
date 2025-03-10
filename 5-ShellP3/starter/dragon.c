#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// lam said i should make a static
const char *DRAGON =
"73 1@4%23 \n"
"70 6%25 \n"
"69 6%26 \n"
"66 1%1 7%11 1@14 \n"
"65 10%8 7%11 \n"
"40 7%2 4%1@9 12%1@4 6%2 1@4%8 \n"
"35 22%6 28%10 \n"
"33 26%3 12%1 15%11 \n"
"32 29%1 19%5 3%12 \n"
"30 28%1@1 1@18%8 2%12 \n"
"29 33%1 22%16 \n"
"29 58%14 \n"
"29 50%1@6%1@14 \n"
"7 8%1@11 16%8 26%6 2%16 \n"
"5 13%9 2%1@12%11 11%1 12%6 1@1%16 \n"
"3 10%3 3%8 14%12 24%24 \n"
"2 9%7 1%9 13%13 12%1@11%23 \n"
"10%1@16 1%1 13%12 1@25%21 \n"
"9%1@17 2%1@12%12 1@28%18 \n"
"8%1@19 15%11 33%14 \n"
"11%18 15%10 35%6 4%2 \n"
"10%1@19 1@14%9 12%1@1 4%1 17%3 8%\n"
"11%18 17%8 13%6 18%1 9%\n"
"10%1@2%1@16 16%1@7 14%5 24%2 2%\n"
"2 10%18 1%1 14%1@8 14%3 26%1 2%\n"
"3 12%2 1@11 18%8 40%2 3%1 \n"
"4 13%1 2%2 1%2 1%1@1 18%10 37%4 3%1 \n"
"5 18%1 22%11 1@31%4 7%1 \n"
"6 39%14 28%8 3%3 \n"
"7 1@35%18 25%15 \n"
"9 32%22 19%2 7%10 \n"
"12 26%27 15%2 1@9%9 \n"
"15 20%11 1@1%1@1%18 1@18%3 3%8 \n"
"19 15%8 10%20 15%4 1%9 \n"
"17 36%22 14%12 \n"
"17 26%2 4%1 3%22 10%2 3%1@10 \n"
"22 19%1 6%1 2%26 13%1@10 \n"
"82 7%1@11 \n";

// EXTRA CREDIT - print the drexel dragon from the readme.md
extern void print_dragon() {
  int acc_idx = 0;
  char acc[8] = {0}; // hold the number in

  for (size_t i = 0; i < strlen(DRAGON); ++i) {
      if (DRAGON[i] == '\n') {
      printf("\n");
      continue;
      }

      if (DRAGON[i] == ' ' || DRAGON[i] == '%' || DRAGON[i] == '@') {
      int count = atoi(acc);

      for (; count != 0; --count)
          printf("%c", DRAGON[i]);

      memset(acc, '\0', 8);
      acc_idx = 0;
      }
      else {
      // no range check bcuz the input we use doesn't overflow it ever.
      acc[acc_idx++] = DRAGON[i];
      }
  }
}
