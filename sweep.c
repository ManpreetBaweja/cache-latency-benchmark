#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

double measure(size_t kb, long long steps) {
    size_t count = kb * 1024 / sizeof(void *);
    void **arr = malloc(count * sizeof(void *));
    for (size_t i = 0; i < count; i++) arr[i] = &arr[i];
    for (size_t i = count - 1; i > 0; i--) {
        size_t r = ((size_t)rand() << 15) | (size_t)rand();
        size_t j = r % i;
        void *tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
    }

    void **p = arr;
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (long long i = 0; i < steps; i++) p = (void **)*p;
    QueryPerformanceCounter(&end);

    if (p == NULL) printf("never\n");
    free(arr);

    double secs = (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart;
    return secs * 1e9 / steps;
}

int main(void) {
    size_t sizes[] = {4, 8, 16, 24, 28, 32, 36, 40, 48, 56, 64, 80, 96, 128, 192, 256,
                  320, 384, 448, 512, 576, 640, 704, 768, 896, 1024, 1280, 1536,
                  2048, 3072, 4096, 6144, 8192, 10240, 12288, 14336, 16384,
                  18432, 20480, 24576, 32768, 49152, 65536, 131072};
    int nsizes = sizeof(sizes) / sizeof(sizes[0]);
    int reps = 5;

    srand(12345);
    printf("kb,ns\n");

    for (int s = 0; s < nsizes; s++) {
        long long steps = 200000000;
        double r[5];
        for (int k = 0; k < reps; k++) r[k] = measure(sizes[s], steps);

        for (int a = 1; a < reps; a++) {
            double v = r[a]; int b = a - 1;
            while (b >= 0 && r[b] > v) { r[b + 1] = r[b]; b--; }
            r[b + 1] = v;
        }
        printf("%zu,%.3f\n", sizes[s], r[reps / 2]);
        fflush(stdout);
    }
    return 0;
}