/*******************
Use example and information in Appendix B in Lab instruction manual to finish this file
*******************/
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>

#include <wiringPiSPI.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

inlcude "encryption.hpp"
//specify port and buffer for transmit
#define BUFFERSIZE 64
#define PORTNUMBER 50123

const char KEY[]="3874460957140850"
const char IV[]="9331626268227018"
//specify server address
const char hostname[]="0.0.0.0"

cons int spichannel(0);

void initialSPI(){
	wiringPiSPISetupMode(spiChannel,1000000,3);
	usleep(10);
	unsigned char buf[2];
	//buf[0]= 0x80;
	//wiringPiSPIDataRW(spiChannel, (unsigned char *)&buf,2);
	//std::cout<<std::hex<<(short)buf[1]<<std::end1;
	//std::cout<< "finished testing"<< std::end1;

	//configure power
	buf[0]=0x2D;
	buf[1]=0x18;
	wiringPiSPIDataRW(spiChannel, (unsigned char *)&buf,2);
	
	//Configure data format
	buf[0]=0x31;
	buf[1]=0x00;
	wiringPiSPIDataRW(spiChannel, (unsigned char *)&buf,2);
	return;
}
void readRawXYZ(short &X, short &Y, short &Z){
	unsigned char txRxData[2];
	unsigned char buf[6];
	//read data
	for(unsigned short i(0);i < 6; i++){
		txRxData[0]=(unsigned char) ((unsigned short))0xB2 +i);
		wiringPiSPIDataRW(spiChannel, (unsigned char *)&buf,2);
		buf[i]= txRxData[1]
	}
	X= (buf[1]<<8) | buf[0];
	Y= (buf[3]<<8) | buf[2];
	Z= (buf[5]<<8) | buf[4];	
	return;
}
void readXYZ(float &X, float &Y, float &Z, const short &scale=2){
	short x_raw, y_raw; z_raw;
	readRawXYZ(x_raw,y_raw, z_raw);
	X= (float) x_raw / 1024 * scale;
	Y= (float) y_raw / 1024 * scale;
	Z= (float) z_raw / 1024 * scale;
	return;
}
bool kbhit(void){
struct termios oldt, newt;
int chl
int oldf;

tcgetattr(STDIN_FILENO, &oldt):
newt = oldt;
newt.c_lflag &=~(ICANON | ECHO);
tcsetattr(STDIN_FILENO, TCSANOW, &newt);
oldf= fcnt1(STDIN_FILENO, F_GETFL,0);
fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

ch= getchar();

tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
fcntl(STDIN_FILENO, F_SETFL, oldf);

if(ch != EOF){
	ungetc(ch,stdin);
	return true;
}
return false;
}
int main(){
	initialSPI();
	float x,y,z;
	//verify socket
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0){
		printf("ERROR opening socket\n");
		exit(1);
	}
	struct hostent *server= gethostbyname(hostname);
	if(server==NULL){
	printf("ERROR,no such host %s\n",hostname);
	exit(1);
	}
	struct sockaddr_in serveraddr;
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family= AF_INET;
	bcopy((char *)server->h_addr,(char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port =htons(PORTNUMBER0;

	char bufRaw[BUFFERSIZE], bufCiphered[BUFFERSIZE * 2];
	while(!khbhit()){
	readXYZ(x,y,z);
	snprintf(bufRaw,BUFFERSIZE,"%2.6f, %2.6f, %2.6f\n",x,y,z);
	printf(bufRaw);
	
	int length= encrypt((const char *)&KEY,(const char *)&IV, &bufRaw[0],(char *)&bufCiphered);
	
	for(uint i=0;i<length; i++){
	printf("%O2x \n", bufCiphered[i]);

	int sendStatus =sendto(sockfd,bufCiphered, length,0,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(sendStatus<0){
	printf("Sent failed with status %d\n",sendStatus);
	exit(1);
	}
	usleep(1000000);
	}	
	return 0;
	}