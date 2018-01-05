#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX_LINE_LENGTH (1024)

void send_response(FILE *f, char *key, char *value) {
  fprintf(f, "HTTP/1.1 200 OK\r\n");
  fprintf(f, "Content-Type: application/json\r\n");
  fprintf(f, "\r\n");
  fprintf(f, "{\"%s\": \"%s\"}", key, value);
}

int extract_ip_address_from_header(char *line, char *address) {
  int found = 0;
  char *ptr;
  char *name;
  char *value;

  name = strndup(line, MAX_LINE_LENGTH);
  ptr = index(name, (int)':');
  if (ptr == NULL) {
    return 0;
  }
  // end the string at the colon
  *ptr = '\0';

  // get the value part of the header field
  ptr = index(line, (int) ':');
  value = strndup(ptr + 2, MAX_LINE_LENGTH);

  // most ugly way to remove \r\n from the end of the string
  value[strlen(value)-2] = '\0';

  if (strncmp(name, "X-Forwarded-For", MAX_LINE_LENGTH) == 0) {
    found = 1;
    strncpy(address, value, MAX_LINE_LENGTH);
  }

  free(name);
  free(value);

  return found;
}

char * get_ip_address(FILE *f) {
  int address_found = 0;
  char *res;
  char *ip_address = malloc (sizeof (char) * MAX_LINE_LENGTH);
  char header_line[MAX_LINE_LENGTH];

  do {
    res = fgets(header_line, MAX_LINE_LENGTH, f);

    if (res != NULL) {
      printf("%s", res);
      if (!address_found) {
        address_found = extract_ip_address_from_header(res, ip_address);
      }
    }
  } while (res != NULL && strcmp(header_line, "\r\n") != 0);

  return ip_address;
}

int open_connection(int port) {
  int sock;
  struct sockaddr_in addr_in;

  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1) {
    printf("Failed to create socket (%d)\n", errno);
    exit(EXIT_FAILURE);
  }

  addr_in.sin_family = AF_INET;
  addr_in.sin_port = htons(port);
  addr_in.sin_addr.s_addr = INADDR_ANY;

  bind(sock, (struct sockaddr *) &addr_in, sizeof(struct sockaddr_in));

  if (listen(sock, 8) == -1) {
    printf("Failed to get socket to listen (%d)\n", errno);
    exit(EXIT_FAILURE);
  }

  return sock;
}

void accept_client(int sock) {
  struct sockaddr_in client_addr;
  socklen_t clientaddr_len;
  FILE *f;

  int client_sock = accept(sock, (struct sockaddr *)&client_addr, &clientaddr_len);
  if (client_sock == -1) {
    printf("Failed to accept connection (%d)\n", errno);
    exit(EXIT_FAILURE);
  }

  f = fdopen(client_sock, "w+");

  send_response(f, "myaddress", get_ip_address(f));

  fclose(f);

  // stdout needs to be flushed in order for heroku to read the logs
  fflush(stdout);
}

int main(int argc, char **argv) {
  int port;

  if (argc != 2) {
    printf("%s [port-number]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  port = atoi(argv[1]);
  int sock = open_connection(port);

  while (1) {
    accept_client(sock);
  }

  close(sock);
  exit(EXIT_SUCCESS);
}