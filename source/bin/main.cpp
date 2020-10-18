#include "evr_threadpool.h"

using namespace std;

void testeA()
{
    printf("A\n");
}

void testeB()
{
    printf("note\n");
}

int main(int argc, char *const argv[])
{
    // int Num_Threads = thread::hardware_concurrency();

    ThreadPool tp(1,1);
    tp.executeJob(testeA, testeB);

    return 0;
}