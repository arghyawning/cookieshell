#include "headers.h"

// Function to remove HTML tags from a string
void remove_tags(char *str)
{
    int in_tag = 0;
    char *p = str;

    while (*p)
    {
        if (*p == '<')
            in_tag = 1;
        else if (*p == '>')
            in_tag = 0;
        else if (!in_tag)
            putchar(*p);
        p++;
    }
}

void iman(char *subcom)
{
    char *comm = strtok(subcom, " \t\n");
    comm = strtok(NULL, " \t\n");

    if (comm == NULL)
    {
        printf(ERROR_COLOR "no command entered\n" DEFAULT_COLOR);
        return;
    }
    trimstr(comm);

    struct addrinfo hints, *res;
    int status, socketfd;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo("man.he.net", "80", &hints, &res);
    if (status != 0)
    {
        printf(ERROR_COLOR "getaddrinfo error: %s\n" DEFAULT_COLOR, gai_strerror(status));
        return;
    }

    socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socketfd == -1)
    {
        printf(ERROR_COLOR "Could not create socket\n" DEFAULT_COLOR);
        freeaddrinfo(res);
        return;
    }

    if (connect(socketfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        printf(ERROR_COLOR "Could not connect to the address\n" DEFAULT_COLOR);
        close(socketfd);
        freeaddrinfo(res);
        return;
    }

    char request[1024];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=0 HTTP/1.1\r\nHost: man.he.net\r\n\r\n", comm);

    if (send(socketfd, request, strlen(request), 0) == -1)
    {
        printf(ERROR_COLOR "Could not send request\n" DEFAULT_COLOR);
        close(socketfd);
        freeaddrinfo(res);
        return;
    }

    // receiving the response
    char response[4096];
    int received, in_pre = 0;
    while ((received = recv(socketfd, response, 4096, 0)) > 0)
    {
        response[received] = '\0';
        if (strstr(response, "No manual entry for") != NULL)
        {
            printf(ERROR_COLOR "Error: No manual entry found for '%s'\n" DEFAULT_COLOR, comm);
            close(socketfd);
            freeaddrinfo(res);
            return;
        }
        // printf("%s", response);

        // Parse and print the content inside <PRE> tags
        char *start = strstr(response, "<PRE>");
        char *end = strstr(response, "</PRE>");
        if (start && end)
        {
            in_pre = 1;
            start += strlen("<PRE>");

            // Remove HTML tags from the content
            remove_tags(start);
        }
        else if (in_pre && end)
        {
            in_pre = 0;
        }
    }

    if (received == -1)
    {
        printf(ERROR_COLOR "Could not receive response\n" DEFAULT_COLOR);
        close(socketfd);
        freeaddrinfo(res);
        return;
    }

    close(socketfd);
    freeaddrinfo(res);
}