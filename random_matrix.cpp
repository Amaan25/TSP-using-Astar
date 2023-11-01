// This program generates an adjacency matrix with random values for a complete graph
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;
#define SIZE 4      // define the number of nodes in the graph
#define LB 1        // define a lower bound for the values 
#define UB 9        // define an upper bound for the values

int generateRandomNumber() {
    return rand() % (UB - LB + 1) + LB; // gives a random number between LB and UB
}

int main() {
    srand(time(0)); // Seed the random number generator
    freopen("input.txt", "w", stdout);

    int g[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = i; j < SIZE; j++) {
            if (i == j) g[i][j] = 0;
            else {
                g[i][j] = generateRandomNumber();
                g[j][i] = g[i][j];
            }
        }
    }
    cout << SIZE << '\n';
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << g[i][j] << " ";
        }
        cout << '\n';
    }
    return 0;
}
