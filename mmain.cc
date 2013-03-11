/*
 * mmain.cc
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 *
 */
#include "FileManager.h"
#include "SBB_util.h"
#include "sbb_socket.h"

#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


using namespace std;

int main(int argc, char* argv[])
{
	/* 
	 * get an internet domain socket 
	 */
	int sd;
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* 
	 * set up the socket structure 
	 */
	struct sockaddr_in	sock_addr;

	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;

#ifdef SBB_ANY
	/* set to INADDR_ANY if want server to be open to any client on any machine */
	sock_addr.sin_addr.s_addr = INADDR_ANY;
#else
	char	hostname[128];
	/*
	 *  we'll default to this host and call a section 3 func to get this host
	 */
	if( gethostname(hostname,sizeof(hostname)) ){
		fprintf(stderr," SBB gethostname(...) failed errno: %d\n", errno);
		exit(1);
	}
	//printf("SBB gethostname() local hostname: \"%s\"\n", hostname);

	/*
	 * set up socket structure for our host machine
	 */
	struct hostent *hp;
	if ((hp = gethostbyname(hostname)) == 0) {
		fprintf(stderr,"SBB gethostbyname(...) failed errno: %d exiting...\n", errno);
		exit(1);
	}
	sock_addr.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
#endif
	sock_addr.sin_port = htons(PORT);

	/* 
	 * bind the socket to the port number 
	 */
	if (bind(sd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) == -1) {
		perror("bind");
		exit(1);
	}

	/* 
	 * advertise we are available on this socket/port
	 */
	if (listen(sd, 5) == -1) {
		perror("listen");
		exit(1);
	}

	/* 
	 * wait for a client to connect 
	 */
	struct sockaddr_in	sock_addr_from_client;
        socklen_t addrlen = sizeof(sock_addr_from_client); 
	int sd_current;
	if ((sd_current = accept(sd, (struct sockaddr *)  &sock_addr_from_client, &addrlen)) == -1) {
		fprintf(stderr,"SBB accept(...) failed errno: %d  exiting...\n", errno);
		exit(1);
	}

	/*
	 * block on socket waiting for client message
	 */
	int ret = 0;
        char	msg[MSGSIZE];
	//fprintf(stderr," SBB: sizeof msg: %d\n", sizeof(msg));

	//printf("gonna enter while\n");
	char* result = new char[8192];

	SBB_util clock;
	FileManager fileManager;
	double* timeResultsPointer;// = clock.end_clock();

	while (ret = recv(sd_current, msg, sizeof(msg), 0) > 0) {
        	//printf("server received msg: [%s] from client\n", msg);
		//printf("1st char is %c\n",msg[0]);
		//printf("2nd char is %c\n",msg[1]);

//////////////////////////////
		//initial dynamic/reset has been hit
		if (msg[0] == 'i'){
			fileManager.processTradingBooks(argv);
			fileManager.back_To_Original();
			const double* resultsPointer_dynamicTable = fileManager.returnDynamicResults();
			char* curr_dyn_number = new char[20];
			for(unsigned i = 0; i < 12; i++)
			{
				sprintf(curr_dyn_number,"%.3f ",resultsPointer_dynamicTable[i]);
				strcat(result, curr_dyn_number);
				memset(curr_dyn_number,'\0', 20);
			}
		}
		
		//do static stuff
		if (msg[0] == 's'){
			clock.start_clock();

			fileManager.processTradingBooks(argv);
			const vector<double>* resultsPointer = fileManager.returnStaticResults(99);
			char* curr_static_number = new char[20];
			for(unsigned i = 0; i < resultsPointer->size(); i++)
			{
				sprintf(curr_static_number,"%.3f ",resultsPointer->at(i));
				strcat(result, curr_static_number);
				memset(curr_static_number,'\0', 20);
			}
			
			timeResultsPointer = clock.end_clock();
			sprintf(curr_static_number,"%.3f %.3f %.3f", timeResultsPointer[0], timeResultsPointer[1], timeResultsPointer[2]);
			strcat(result,curr_static_number);
		}

		//do calculate stuff
		else if (msg[0] == 'c') {
			// bucket_risk = dv01_2yr * amount
			// amount = bucket_risk/dv01_2yr
			fileManager.processTradingBooks(argv);
			char * curr_piece;
			curr_piece = strtok(msg," ");
			int delta_2yr = 0;
			int delta_5yr = 0;
			int delta_10yr = 0;
			int delta_30yr = 0;
			for (unsigned i = 0; i < 9; i++)
			{
			   //printf ("%s\n",curr_piece);
			   if (i == 2)
			      delta_2yr = atoi(curr_piece);
			   else if(i == 4)
			      delta_5yr = atoi(curr_piece);
			   else if(i == 6)
			      delta_10yr = atoi(curr_piece);
			   else if(i == 8)
			      delta_30yr = atoi(curr_piece);
                           curr_piece = strtok (NULL, " ");
			}
			clock.start_clock();

			char* curr_bump_number = new char[10];
			char* hedges = new char[40];
			memset(hedges,'\0', 40);
			char* time_data_calc = new char[40];

			//2yr
			const double* dynamicResultsPointer = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(delta_2yr, "2_yr");

			for(unsigned i = 0; i < 2; i++)
			{
				sprintf(curr_bump_number,"%.3f ",dynamicResultsPointer[i]);
				strcat(result, curr_bump_number);
				memset(curr_bump_number,'\0', 10);
			}
			sprintf(curr_bump_number,"%.3f ",dynamicResultsPointer[2]);
			strcat(hedges, curr_bump_number);
			memset(curr_bump_number,'\0', 10);

			//5yr
			const double* dynamicResultsPointer_5yr = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(delta_5yr, "5_yr");
			for(unsigned i = 0; i < 2; i++)
			{
				sprintf(curr_bump_number,"%.3f ",dynamicResultsPointer_5yr[i]);
				strcat(result, curr_bump_number);
				memset(curr_bump_number,'\0', 10);
			}
			sprintf(curr_bump_number,"%.3f ",dynamicResultsPointer_5yr[2]);
			strcat(hedges, curr_bump_number);
			memset(curr_bump_number,'\0', 10);

			//10yr
			const double* dynamicResultsPointer_10yr = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(delta_10yr, "10_yr");
			for(unsigned i = 0; i < 2; i++)
			{
				sprintf(curr_bump_number,"%.3f ",dynamicResultsPointer_10yr[i]);
				strcat(result, curr_bump_number);
				memset(curr_bump_number,'\0', 10);
			}
			sprintf(curr_bump_number,"%.3f ",dynamicResultsPointer_10yr[2]);
			strcat(hedges, curr_bump_number);
			memset(curr_bump_number,'\0', 10);

			//30yr
			const double* dynamicResultsPointer_30yr = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(delta_30yr, "30_yr");
			for(unsigned i = 0; i < 2; i++)
			{
				sprintf(curr_bump_number,"%.3f ",dynamicResultsPointer_30yr[i]);
				strcat(result, curr_bump_number);
				memset(curr_bump_number,'\0', 10);
			}
			sprintf(curr_bump_number,"%.3f ",dynamicResultsPointer_30yr[2]);
			strcat(hedges, curr_bump_number);
			memset(curr_bump_number,'\0', 10);

			//add shifted_2_yr_hedges
			strcat(result, hedges);

			//add timing data
			timeResultsPointer = clock.end_clock();
			sprintf(time_data_calc,"%.3f %.3f %.3f", timeResultsPointer[0], timeResultsPointer[1], timeResultsPointer[2]);
			strcat(result,time_data_calc);

		}

		//do 50% up stuff
		else if (msg[0] == 'u') {
			//2yr_risk, 2yr_mktvalue, 5yr_risk, 5yr_mktvalue...
			//2yr_hedge, 5yr_hedge...
			clock.start_clock();

			fileManager.processTradingBooks(argv);
			char* curr_bump_up_number = new char[10];
			char* hedges = new char[40];
			memset(hedges,'\0', 40);
			char* time_data_bump_up = new char[40];

			//2yr
			const double* dynamicResultsPointer = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(50, "2_yr");

		        for(unsigned i = 0; i < 2; i++)
        		{
				sprintf(curr_bump_up_number,"%.3f ",dynamicResultsPointer[i]);
				strcat(result, curr_bump_up_number);
				memset(curr_bump_up_number,'\0', 10);
		        }
			sprintf(curr_bump_up_number,"%.3f ",dynamicResultsPointer[2]);
			strcat(hedges, curr_bump_up_number);
			memset(curr_bump_up_number,'\0', 10);

			//5yr
			const double* dynamicResultsPointer_5yr = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(50, "5_yr");
		        for(unsigned i = 0; i < 2; i++)
        		{
				sprintf(curr_bump_up_number,"%.3f ",dynamicResultsPointer_5yr[i]);
				strcat(result, curr_bump_up_number);
				memset(curr_bump_up_number,'\0', 10);
		        }
			sprintf(curr_bump_up_number,"%.3f ",dynamicResultsPointer_5yr[2]);
			strcat(hedges, curr_bump_up_number);
			memset(curr_bump_up_number,'\0', 10);

			//10yr
			const double* dynamicResultsPointer_10yr = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(50, "10_yr");
		        for(unsigned i = 0; i < 2; i++)
        		{
				sprintf(curr_bump_up_number,"%.3f ",dynamicResultsPointer_10yr[i]);
				strcat(result, curr_bump_up_number);
				memset(curr_bump_up_number,'\0', 10);
		        }
			sprintf(curr_bump_up_number,"%.3f ",dynamicResultsPointer_10yr[2]);
			strcat(hedges, curr_bump_up_number);
			memset(curr_bump_up_number,'\0', 10);

			//30yr
			const double* dynamicResultsPointer_30yr = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(50, "30_yr");
		        for(unsigned i = 0; i < 2; i++)
        		{
				sprintf(curr_bump_up_number,"%.3f ",dynamicResultsPointer_30yr[i]);
				strcat(result, curr_bump_up_number);
				memset(curr_bump_up_number,'\0', 10);
		        }
			sprintf(curr_bump_up_number,"%.3f ",dynamicResultsPointer_30yr[2]);
			strcat(hedges, curr_bump_up_number);
			memset(curr_bump_up_number,'\0', 10);

			//add shifted_2_yr_hedges
			strcat(result, hedges);

			//add timing data
			//double*
			timeResultsPointer = clock.end_clock();
			sprintf(time_data_bump_up,"%.3f %.3f %.3f", timeResultsPointer[0], timeResultsPointer[1], timeResultsPointer[2]);
			strcat(result,time_data_bump_up);
		}

		//do 50% down stuff
		else if (msg[0] == 'd') {
			//2yr_risk, 2yr_mktvalue, 5yr_risk, 5yr_mktvalue...
			//2yr_hedge, 5yr_hedge...
			//SBB_util clock;
			clock.start_clock();

			//FileManager fileManager;

			fileManager.processTradingBooks(argv);
			char* curr_bump_down_number = new char[10];
			char* hedges = new char[40];
			memset(hedges,'\0', 40);
			char* time_data_bump_down = new char[40];

			//2yr
			const double* dynamicResultsPointer = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(-50, "2_yr");

		        for(unsigned i = 0; i < 2; i++)
        		{
				sprintf(curr_bump_down_number,"%.3f ",dynamicResultsPointer[i]);
				strcat(result, curr_bump_down_number);
				memset(curr_bump_down_number,'\0', 10);
		        }
			sprintf(curr_bump_down_number,"%.3f ",dynamicResultsPointer[2]);
			strcat(hedges, curr_bump_down_number);
			memset(curr_bump_down_number,'\0', 10);

			//5yr
			const double* dynamicResultsPointer_5yr = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(-50, "5_yr");
		        for(unsigned i = 0; i < 2; i++)
        		{
				sprintf(curr_bump_down_number,"%.3f ",dynamicResultsPointer_5yr[i]);
				strcat(result, curr_bump_down_number);
				memset(curr_bump_down_number,'\0', 10);
		        }
			sprintf(curr_bump_down_number,"%.3f ",dynamicResultsPointer_5yr[2]);
			strcat(hedges, curr_bump_down_number);
			memset(curr_bump_down_number,'\0', 10);

			//10yr
			const double* dynamicResultsPointer_10yr = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(-50, "10_yr");
		        for(unsigned i = 0; i < 2; i++)
        		{
				sprintf(curr_bump_down_number,"%.3f ",dynamicResultsPointer_10yr[i]);
				strcat(result, curr_bump_down_number);
				memset(curr_bump_down_number,'\0', 10);
		        }
			sprintf(curr_bump_down_number,"%.3f ",dynamicResultsPointer_10yr[2]);
			strcat(hedges, curr_bump_down_number);
			memset(curr_bump_down_number,'\0', 10);

			//30yr
			const double* dynamicResultsPointer_30yr = FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(-50, "30_yr");
		        for(unsigned i = 0; i < 2; i++)
        		{
				sprintf(curr_bump_down_number,"%.3f ",dynamicResultsPointer_30yr[i]);
				strcat(result, curr_bump_down_number);
				memset(curr_bump_down_number,'\0', 10);
		        }
			sprintf(curr_bump_down_number,"%.3f ",dynamicResultsPointer_30yr[2]);
			strcat(hedges, curr_bump_down_number);
			memset(curr_bump_down_number,'\0', 10);

			//add shifted_2_yr_hedges
			strcat(result, hedges);

			//add timing data
			//double*
			timeResultsPointer = clock.end_clock();
			sprintf(time_data_bump_down,"%.3f %.3f %.3f", timeResultsPointer[0], timeResultsPointer[1], timeResultsPointer[2]);
			strcat(result,time_data_bump_down);
		}

		//do graph stuff
		else if (msg[0] == 'g') {
			fileManager.processTradingBooks(argv);
			const long* frequencyDataPointer = fileManager.returnGraphData();

			char* curr_graph_number = new char[20];
		        for(unsigned i = 0; i < 12; i++)
		        {
				sprintf(curr_graph_number,"%i ",frequencyDataPointer[i]);
				strcat(result, curr_graph_number);
				memset(curr_graph_number,'\0', 20);
		        }

		}

		//do exit stuff
		else if (msg[0] == 'x') {
			close(sd_current); 
			close(sd);
			exit(0);
		}
///////////////////////

		strcpy(msg,result);
		if (send(sd_current, msg, strlen(msg), 0) == -1) 
			fprintf(stderr,"SBB send(...) failed errno: %d exiting...\n", errno);
		//"flush" buffer
		memset(result, '\0', MSGSIZE);
		memset(msg, '\0', MSGSIZE);
	}//end of while
	if( -1 == ret ) {
		fprintf(stderr,"SBB recv(...) returned failed - errno: %d exiting...\n", errno);	
		exit(1);
	}

	close(sd_current); 
	close(sd);

	return 0;
}
