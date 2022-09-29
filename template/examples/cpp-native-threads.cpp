#include <string>
#include <iostream>
#include <thread>

using namespace std;

// The function we want to execute on the new thread.
void task1(string msg)
{
    cout << "task1 says: " << msg;
    for (size_t i = 0; i < 1000; i++)
    {
      
    }
    
}

int main()
{
    // Constructs the new thread and runs it. Does not block execution.
    thread new_thread(task1, "Hello");
    fprintf(stderr, "Hey there\n");
    fprintf(stderr, "Hey there\n");
    fprintf(stderr, "Hey there\n");
    new_thread.join();
}