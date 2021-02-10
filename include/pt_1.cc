#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 200000000
volatile int turn = 0;
volatile int flag[2] = { 0  };
int num = 0;

pthread_t thread[2];

void enter_critical_section(int id)
{
        flag[id] = true;
            turn = 1 - id;
                while(flag[1 - id] && turn == 1 - id);
                
}

void leave_critical_section(int id)
{
        flag[id] = false;
        
}


void *count0(void *t) {
    /* flag[0] = 1; */
    /* turn = 1; */
    /* while (flag[1] == 1 && turn == 1) continue; */
    enter_critical_section(0);
    for (int i = 0; i < N; i++) {
        num++;                      // critical section

    }
    leave_critical_section(0);
    /* flag[0] = 0; */

}

void *count1(void *t) {
    /* flag[1] = 1; */
    /* turn = 0; */
    /* while (flag[0] == 1 && turn == 0) continue; */
    enter_critical_section(1);
    for (int i = 0; i < N; i++) {
        num++;

    }
    leave_critical_section(1);
    /* flag[1] = 0; */

}

int main() {
    /* while (1) { */
        pthread_create(&thread[0], NULL, count0, NULL);
        pthread_create(&thread[1], NULL, count1, NULL);

        pthread_join(thread[0], NULL);
        pthread_join(thread[1], NULL);
        printf("num = %d\n", num);
        /* if (num != 2000) { printf("!!!!!!"); exit(0);  } */
        /* num = 0; */

    /* } */
    return 0;

}
