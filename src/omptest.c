#include <stdio.h>
#include <omp.h>

//int main() {
//    int n = 1000;
//
//    printf("Num de threads: %d", omp_get_num_threads());
//
//    omp_set_num_threads(25);
//
//
//#pragma omp parallel for
//    for (int i = 0; i < n; ++i) {
//        int thread_id = omp_get_thread_num();
//        printf("Thread %d, iteração %d\n", thread_id, i);
//    }
//
//    return 0;
//}
