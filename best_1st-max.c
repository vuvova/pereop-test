#include <stdio.h>

#include "w.h"

#define GUESSES (sizeof(guesses)/sizeof(guesses[0]))
#define WORDS (sizeof(words)/sizeof(words[0]))

int cmps[WORDS];

int compare(const char *g, const char *w) {
#define one(G,W1,W2,W3,W4) (\
  (g[G] == w[G] ? 1 : \
   g[G] == w[W1] || g[G] == w[W2] || g[G] == w[W3] || g[G] == w[W4] ? 2 : 3) \
   << (G*2))

  return one(0,1,2,3,4) |
         one(1,0,2,3,4) |
         one(2,0,1,3,4) |
         one(3,0,1,2,4) |
         one(4,0,1,2,3);
}

int main() {
  int mincnt=WORDS*WORDS, ming=0;
  printf("%lu %lu\n", GUESSES, WORDS);
  for (int g=0; g < GUESSES; g++) {
    int cnt=0;
    for (int w=0; w < WORDS; w++) {
      cmps[w]=compare(guesses[g], words[w]);
    }
    for (int w=0; w < WORDS; w++) {
      int cntw=0;
      for (int ww=0; ww < WORDS; ww++) {
        cntw+= cmps[w] == cmps[ww];
      }
      if (cnt < cntw) cnt=cntw;
    }
    if (cnt > mincnt) continue;
    mincnt=cnt;
    ming=g;
    printf("%s %d\n", guesses[ming], mincnt);
  }
}
/*
 aesir 167
 reais 167
 serai 167
*/
