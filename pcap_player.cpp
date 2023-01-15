#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <pcap.h>
#include <string>
#include <unistd.h>

// TODO: Analyze this code and see if it actually works.
int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <pcap file> <ip address> <port>"
              << std::endl;
    return 1;
  }

  // Open the pcap file
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t *handle = pcap_open_offline(argv[1], errbuf);
  if (handle == nullptr) {
    std::cerr << "Error opening pcap file: " << errbuf << std::endl;
    return 1;
  }

  // Set the destination IP address and port
  in_addr addr;
  if (inet_aton(argv[2], &addr) == 0) {
    std::cerr << "Invalid IP address" << std::endl;
    return 1;
  }
  uint16_t port = static_cast<uint16_t>(atoi(argv[3]));

  // Read and send the packets from the pcap file
  struct pcap_pkthdr *header;
  const u_char *packet;
  while (pcap_next_ex(handle, &header, &packet) == 1) {
    // Create a raw socket
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sockfd < 0) {
      perror("socket");
      return 1;
    }

    // Set the destination address
    sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr = addr;
    dest_addr.sin_port = htons(port);

    // Send the packet
    if (sendto(sockfd, packet, header->len, 0, (sockaddr *)&dest_addr,
               sizeof(dest_addr)) < 0) {
      perror("sendto");
      return 1;
    }

    close(sockfd);
  }

  pcap_close(handle);
  return 0;
}
