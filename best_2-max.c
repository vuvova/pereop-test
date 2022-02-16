#include <stdio.h>
#include <pthread.h>
#include <err.h>

#include "w.h"

#define GUESSES (sizeof(guesses)/sizeof(guesses[0]))
#define WORDS (sizeof(words)/sizeof(words[0]))

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

int cmp2(const char *g1, const char *g2, const char *w) {
  return (compare(g1, w) << 10) | compare(g2, w);
}

#define N 32
#define STEP (GUESSES/N+1)

void *dwim(void *v) {
  int mincnt=__INT_MAX__;
  int g1m=0, g2m=0, start=(int)(long long)v, end=start+STEP;
  int cmps[WORDS];
  char fname[100];
  sprintf(fname, "out-%05d.log", start);
  FILE *f=fopen(fname, "w");
  setbuf(f, 0);
  for (int g1= start; g1 < end; g1++) {
    for (int g2=g1+1; g2 < GUESSES; g2++) {
      int cnt= 0;
      for (int w=0; w < WORDS; w++) {
        cmps[w]= cmp2(guesses[g1], guesses[g2], words[w]);
      }
      for (int w=0; w < WORDS; w++) {
        int cntw=0;
        for (int ww=0; ww < WORDS; ww++) {
          cntw+= cmps[w] == cmps[ww];
        }
        if (cnt < cntw) cnt=cntw;
      }
      if (cnt < mincnt) {
        g1m= g1;
        g2m= g2;
        mincnt= cnt;
      }
    }
    fprintf(f, "%5d> %s %s %f\n", g1, guesses[g1m], guesses[g2m], ((double)mincnt));
  }
  return 0;
}

int main() {
  pthread_t th[N];
  for (int i=0; i < N; i++) {
    if (pthread_create(&th[i], 0, dwim, (void*)(STEP*i)))
      err(1, "pthread_create");
  }
  for (int i=0; i < N; i++) {
    pthread_join(th[i], 0);
  }
}

/*
 clied ratos 16.000000
 dials toyer 16.000000
 doilt races 16.000000
 loans tired 16.000000
 raced toils 16.000000

 ...and likely others
*/
