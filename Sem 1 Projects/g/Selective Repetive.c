#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_SIZE 4
#define PACKET_SIZE 8
#define TOTAL_PACKETS 16
#define TIMEOUT 5 // in seconds

typedef struct Packet {
  int seq_num;
  char data[PACKET_SIZE];
  int ack_received;
  time_t send_time;
} Packet;

Packet packets[TOTAL_PACKETS];

void send_packet(int seq_num) {
  printf("Sending packet %d...\n", seq_num);
  packets[seq_num].send_time = time(NULL);
}

int receive_ack() {
  int ack_num;
  printf("Enter ACK number:: ");
  scanf("%d", &ack_num);
  return ack_num;
}

int main() {
  int base = 0, next_seq_num = 0;
  int i;

  // Initialize packets
  for (i = 0; i < TOTAL_PACKETS; i++) {
    packets[i].seq_num = i;
    packets[i].ack_received = 0;
  }

  while (base < TOTAL_PACKETS) {
    // Send new packets
    for (i = base; i < next_seq_num + WINDOW_SIZE && i < TOTAL_PACKETS; i++) {
      if (packets[i].ack_received == 0 && time(NULL) - packets[i].send_time > TIMEOUT) {
        send_packet(packets[i].seq_num);
      }
    }

    // Receive ACKs
    while (1) {
      int ack_num = receive_ack();
      if (ack_num >= base && ack_num < next_seq_num + WINDOW_SIZE) {
        packets[ack_num].ack_received = 1;
        if (ack_num == base) {
          // Slide the window
          while (packets[base].ack_received == 1 && base < TOTAL_PACKETS) {
            base++;
          }
          next_seq_num = base + WINDOW_SIZE;
        }
        break;
      }
    }
  }

  printf("All packets successfully transmitted!\n");

  return 0;
}
