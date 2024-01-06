#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_PACKETS 100

typedef struct {
    int seq_num;
    char data[20];
} packet;

int main() {
    packet packets[MAX_PACKETS];
    int num_packets = 0;
    int window_size = 0;
    int timeout_duration = 0;
    int base = 0;
    int nextseqnum = 0;
    int expectedseqnum = 0;
    clock_t start_timer = 0;
    bool timer_running = false;

    // Get user input
    printf("Enter the maximum number of packets to generate (up to %d): ", MAX_PACKETS);
    scanf("%d", &num_packets);
    printf("Enter the window size: ");
    scanf("%d", &window_size);
    printf("Enter the timeout duration in seconds: ");
    scanf("%d", &timeout_duration);

    // Generating packets
    for (int i = 0; i < num_packets; i++) {
        packet p;
        p.seq_num = i;
        sprintf(p.data, "Packet %d", i);
        packets[i] = p;
    }

    // Simulation of transmission
    while (expectedseqnum < num_packets) {
        // Send packets within the window
        while (nextseqnum < base + window_size && nextseqnum < num_packets) {
            printf("Sending packet %d\n", nextseqnum);
            // simulate packet loss
            if (rand() % 10 != 0) {
                if (!timer_running) {
                    start_timer = clock();
                    timer_running = true;
                }
                nextseqnum++;
            } else {
                printf("Packet %d lost, resending packet %d\n", nextseqnum, nextseqnum);
            }
        }

        // Check for timeout
        if (timer_running && (double)(clock() - start_timer) / CLOCKS_PER_SEC >= timeout_duration) {
            printf("Timeout occurred, resending packets %d to %d\n", base, nextseqnum - 1);
            nextseqnum = base;
            timer_running = false;
        }

        // Receive ACKs
        while (expectedseqnum < nextseqnum) {
            printf("Waiting for ACK %d\n", expectedseqnum);
            // simulate ACK loss
            if (rand() % 10 != 0) {
                expectedseqnum++;
                if (timer_running && expectedseqnum == nextseqnum) {
                    timer_running = false;
                }
            } else {
                printf("ACK %d lost, resending packets %d to %d\n", expectedseqnum, base, nextseqnum - 1);
            }
        }

        // Update base
        if (!timer_running && base < num_packets) {
            base = expectedseqnum;
        }
    }

    printf("All packets transmitted successfully!\n");

    return 0;
}