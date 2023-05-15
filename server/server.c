#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<ctype.h>
void error_handling(const char *msg);
int main(int argc,char *argv[])
{   
    char output[100],buffer[100001],names[100][100];
    int sockfd,newsockfd,portno,n;
    struct sockaddr_in serv_addr,cli_addr;
    if(argc < 2)
    {
        fprintf(stderr,"Port number not provided--Program terminated\n");
        exit(1);
    }
    int sock_num;           
    socklen_t clilen;   
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    sock_num = 0;
    if(sockfd<0)
    {
        error_handling("Error opening socket");
    }
    bzero((char *)&serv_addr,sizeof(serv_addr));
    sock_num = portno;
    portno = atoi(argv[1]);  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons(portno);
    sock_num = 0;
    if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {       
        error_handling("Binding failed");
    }
    listen(sockfd,5);   
    clilen = sizeof(cli_addr);
    sock_num = 0;
    newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);   
    if(newsockfd < 0)
    {
        error_handling("Error on accept");
    }
    float percentage;
    int i,t =0,uploaded_size,count,buf_size;
    while(1)
    {
        bzero(buffer,100000);
        char* tokens[10];
        n = read(newsockfd,buffer,255);   
        if(n < 0)
        {
            error_handling("Error on reading");
        }
        i = 0;
        t = 0;
        tokens[0] = strtok(buffer," ");  
        while(tokens[i] != NULL)
        {
            i = i + 1;
            tokens[i] = strtok(NULL," ");
            if(tokens[i] != NULL)
            {
                strcpy(names[t],tokens[i]);
		t = t + 1;
            }
        }
        int k = strncmp("exit",tokens[0],4);  
        if(k == 0)
        {
            printf("Server Exit...\n");
            break;
        }
        if(strcmp("get",tokens[0]) == 0)
        {
            for(int j=0;j<i-1;j++)
            {
                int f;
                sock_num = 0;
                if(i-2 == j)
                {
			names[j][strlen(names[j]) - 1] = '\0';
		 }
                f = open(names[j],0400); 
                char tok;  
                bzero(buffer,100000);
                int filesize = lseek(f,(off_t)0,SEEK_END),filesize2 = filesize;
                count = 0;
                do
                {
                       count = count +1;
                    	filesize2/= 10;
                } while(filesize2 != 0);
                int chec1 = 100;
                if(count == 1)
                {
                    	buf_size = 1;
		}
		else if(count == 2)
		{
			buf_size = 1;
		}
              	else if(count == 3)
                {
                    	buf_size = 10;
		}
		else if(count == 4)
		{
			buf_size = 10;
		}
                else if(count == 5)
                {
                    	buf_size = 1000;
		}
		else if(count == 6)
		{
			buf_size = 1000;
		}
                else if(count == 7)
                {
                    	buf_size = 10000;
		}
		else if(count == 8)
		{
			buf_size = 10000;
		}
                else if(count >= 9)
                {
                    	buf_size = 100000;
		}
                int filesize1 = filesize/buf_size + ((filesize%buf_size) != 0);
                sock_num = 0;
                bzero(buffer,100000);  
                write(newsockfd,&filesize1,sizeof(int));
                lseek(f,(off_t)0,SEEK_SET);
                bzero(buffer,100000);
                sock_num = chec1;
                write(newsockfd,&filesize,sizeof(int));
                if(filesize == -1)
                {
                    filesize1 = -1;
                    printf("Sorry client but the file does not exist\n");
                }
                sock_num = 0;
                bzero(buffer,100000);
                percentage = 0;
                uploaded_size = 0;
                for(int k = 0;k<filesize1;k++)
                {
                    if(k != filesize1 -1)
                    {
                    	 read(f,buffer,buf_size);
                        n = write(newsockfd,buffer,buf_size);
                        uploaded_size = uploaded_size + buf_size;
                    }
                    else
                    {
                        int l = filesize % buf_size;
                        if(l == 0)
                        {
                            read(f,buffer,buf_size);
                            n = write(newsockfd,buffer,buf_size);
                            uploaded_size = uploaded_size + buf_size;
                        }
                        else
                        {
                            read(f,buffer,l);
                            n = write(newsockfd,buffer,l);
                            uploaded_size = uploaded_size + l;
                        }
                    }
                    float per;
                    percentage = (float) uploaded_size / filesize;
                    percentage = percentage*chec1;
                    sprintf(output,"uploading: %0.2f\r",percentage);
		     write(1,output,strlen(output));
                    usleep(10000);
                    bzero(buffer,100000);
                }   
                int chec2 = -1;
                bzero(buffer,100000); 
                if(filesize != chec2)
		 {
                    printf("File has been sent succesfully\n");    
                }
                sleep(0.1);
            }
        }
        else
        {
            printf("get command not used:try once again using get command\n");
            continue;
        }
    }    
    close(newsockfd);
    close(sockfd);
return 0;
}
void error_handling(const char *msg)
{
    perror(msg);
    exit(1); 
}
