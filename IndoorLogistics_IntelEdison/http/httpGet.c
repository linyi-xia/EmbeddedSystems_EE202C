#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void error(const char *msg) { perror(msg); exit(0); }

void httpGET(int num, char* response)
{
    /* first what are we going to send and where are we going to send it? */
    int portno =        8080;
    char *host =        "169.232.190.123";
    char *message_fmt = "GET /database?Car_ID=%d HTTP/1.0\r\n\r\n";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char message[1024];

    //if (argc < 3) { puts("Parameters: <key> <UWVCMAFKPNB7SW0L>"); exit(0); }

    /* fill in the parameters */
    sprintf(message,message_fmt,num);
    printf("Request:\n%s\n",message);

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    //memset(response,0,sizeof(response));
    total = 4096-1;
    received = 0;
    do {
        bytes = read(sockfd,response-received,total-received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    if (received == total)
        error("ERROR storing complete response from socket");

    /* close the socket */
    close(sockfd);

    /* process response */
    //printf("Response:\n%s\n",response);

    return;
}

void parsestring(char* thestring, char* returnstring)   //parse{}out from the original string
{
    char* current = thestring;
    while(current[0] != '{')  //not equal
    {
        current++;
    }
    if (current[0] == '{')
    {
        char* cur = current;
        int count=0;
        while(cur[0] != '}')
        {
            cur++;
            count++;
        }
        strncpy(returnstring, current+1, count-1);
    }
}
int parsestatus(char* getstring)
{
    if (getstring == NULL)
    {
        printf("no input string");
        return 0;
    }
    else
    {
        char currentchar = getstring[0];
        char* current = getstring;
        int count_comma=0;
        while(count_comma<3)
        {
            if (current[0] == ',')
            {
                count_comma++;
                current++;
            }
            else
            {
                current++;
            }
        }
        if (count_comma ==3)
        {
            while(current[0]!=':')
            {
                current++;
            }
            char status= *(current+1);
            if (status == '1')
                return 1;
            else
                return 0;
        }
    }
}
void parseTurnByTurn(char* fullstring, char* turnbyturn)
{
    if (fullstring == NULL)
    {
        printf("no input string");
        return;
    }
    else
    {
        char* current = fullstring;
        int count_comma=0;
        while(count_comma<4)
        {
            if (current[0] == ',')
            {
                count_comma++;
                current++;
            }
            else
            {
                current++;
            }
        }
        if (count_comma ==4)
        {
            while(current[0]!=':')
            {
                current++;
            }
            if (current[0]==':')
            {
                char* cur = current+2;
                int count=0;
                while(cur[0] != '\"')
                {
                    cur++;
                    count++;
                }
                strncpy(turnbyturn, current+2, count);
            }
            
        }
    }
}
void parsechunks (char* getstring, char*chunkoffive, int status)
{
    if (status ==0)
    {
        printf("no mission.");
        return;
    }

    strncpy(chunkoffive, getstring, 5);
    return ; 
}

void parseBeaconID(char* chunkoffive, char*BeaconID)
{
    if (chunkoffive == NULL)
    {
        printf("no input string");
        return ;
    }
    else
    {
        
        strncpy(BeaconID, chunkoffive, 4);
        return ;
    }
}
void ParseTurn(char* chunkoffive, char*turn)
{
    if (chunkoffive == NULL)
    {
        printf("no input string");
        return ;
    }
    else
    {
        strncpy(turn, chunkoffive+4, 1);
        return ;
    }
}
int main(){
    char response[4096];
	memset(response,0,sizeof(response));
    httpGET(1001, response);
    printf("response=%s\n",response);
    char* thestring;
    thestring = (char*) malloc(128);
    parsestring(response, thestring);   //all the strings in the {""}
    printf("thestring=%s\n",thestring);
    char* turnbyturn;
    turnbyturn = (char*) malloc(64);
    char* chunkof5;
    chunkof5 = (char*) malloc(5);
    char* beaconid;
    beaconid = (char*) malloc(4);
    char* turn;
    turn = (char*) malloc(1);
    int status = parsestatus(thestring);
    printf("status=%d\n", status);
    if (status == 0)
        return -1;
    parseTurnByTurn(thestring, turnbyturn);
    printf("turnbyturn=%s\n",turnbyturn);
    char* currentchar = turnbyturn;
    parsechunks(currentchar, chunkof5, status);
    parseBeaconID(chunkof5,beaconid);
    ParseTurn(chunkof5, turn);
    printf("chunkof5:%s, beaconid:%s, turn:%s\n", chunkof5, beaconid, turn);
    while (currentchar+5 != NULL)
    {
        currentchar = currentchar+5;
        parsechunks(currentchar, chunkof5, status);
        parseBeaconID(chunkof5,beaconid);
        ParseTurn(chunkof5, turn);
        printf("chunkof5:%s, beaconid:%s, turn:%s\n", chunkof5, beaconid, turn);
    }
    if (currentchar+5 ==NULL)
    {
        parsechunks(currentchar, chunkof5, status);
        parseBeaconID(chunkof5,beaconid);
        ParseTurn(chunkof5, turn);
        printf("chunkof5:%s, beaconid:%s, turn:%s\n", chunkof5, beaconid, turn);
    }
    return 0;
}
