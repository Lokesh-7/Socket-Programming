#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>      
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>   
#include<string.h>
#include<ctype.h>   
#include <sys/stat.h>
#include <fcntl.h>
void error_handling(const char * msg);
int main(int argc,char *argv[])
{
    	int sockfd,portno,n;
    	char output[100],names[100][100],buffer[100001];
    	struct sockaddr_in serv_addr;
    	if(argc < 3)
    	{
        	fprintf(stderr,"usage %s hostname port\n",argv[0]);  
        	exit(1);
    	}
    	int sock_num;
    	portno = atoi(argv[2]);    
    	sockfd = socket(AF_INET,SOCK_STREAM,0);
    	sock_num = 0;   
    	if(sockfd < 0)
    	{
        	error_handling("Error opeing socket");
        }
	struct hostent *server;
	server = gethostbyname(argv[1]);     
	if(server == NULL)
	{
		fprintf(stderr,"Error no such host");
	}
	sock_num = portno ;
	bzero((char *)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;    
	char verser;
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);      
	sock_num = 0 ;
	serv_addr.sin_port = htons(portno);    
	if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
	{    
		error_handling("connection failed");
	}
	float percentage;
	int t =0,downloaded_size,count,buf_size;
    	while(1)
    	{
		count = 0;
		printf("Client> ");
		bzero(buffer,100000);    
		fgets(buffer,1000,stdin);
		char* tokens[10];	
		n = write(sockfd,buffer,strlen(buffer));	  
		int i = 0;
		if(n < 0)
		{
			error_handling("Error while writing");
		}
        	t = 0;
        	tokens[0] = strtok(buffer," ");  
        	while(tokens[i] != NULL)
        	{	
            		i = i+1;
            		tokens[i] = strtok(NULL," ");
            		char tok1;
			if(i != 0 && (tokens[i] != NULL))
			{
				strcpy(names[t],tokens[i]);
				t = t+1;
			}
        	}
        	int k = strncmp("exit",tokens[0],4); 
		if(k == 0)
		{
			printf("Client Exited...\n");
			break;
		}
        	if(strcmp("get",tokens[0]) == 0)
        	{
        		for(int j=0;j<i-1;j++)
            		{
                		int fp,filesize1,filesize,filesize2 = filesize;
				if(j == i-2)
				{
					names[j][strlen(names[j]) - 1] = '\0';
				}
				count = 0;
				read(sockfd,&filesize1,sizeof(int));
				read(sockfd,&filesize,sizeof(int));
				count = 0;
                		do
                		{
                    			count = count + 1;
                    			filesize2/= 10;
                		} while(filesize2 != 0);
                		int chec1=100;
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
				bzero(buffer,100000);
				if(filesize == -1)
				{
					printf("File not accessible or File not present\n");
					filesize1 = -1;
				}
				else
				{
					fp = open(names[j],O_WRONLY | O_TRUNC |O_CREAT , 0700);
				}
				percentage = 0;
				downloaded_size = 0;
				for(int k = 0;k<filesize1;k++)
				{
					if(k != filesize1 -1)
                    			{
                    				n = read(sockfd,buffer,buf_size);
						write(fp,buffer,buf_size);
						downloaded_size = downloaded_size + buf_size;
					}
					else
					{
                        			int l = filesize % buf_size;
                        			if(l == 0)
                        			{
                        				n = read(sockfd,buffer,buf_size);
							write(fp,buffer,buf_size);
							downloaded_size = downloaded_size + buf_size;
						}
						else
						{
                            				n = read(sockfd,buffer,l);
                            				buffer[l] = '\0';
							write(fp,buffer,l);
							downloaded_size = downloaded_size + l;
						}			
                    			}
                    			float per;
					percentage = (float) downloaded_size/filesize;
					percentage = percentage*chec1;
					sprintf(output,"downloading: %0.2f\r",percentage);
					write(1,output,strlen(output));
					usleep(10000);					
				}
				int chec2 = -1;
				if(filesize != chec2)
				{
                			printf("File has been received successfully\n");
                		}
                	}
        	}
        	else
        	{
            		printf("get command not used:try once again using get command\n");
            		continue;
        	}
       	bzero(buffer,100000);
	}
    	close(sockfd);
return 0;
}
void error_handling(const char * msg)
{
    	perror(msg);
    	exit(0);
}
