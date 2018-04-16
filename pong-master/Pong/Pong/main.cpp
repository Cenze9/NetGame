#include "game.h"
#include <SDL.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <enet/enet.h>
#include <string.h>
#include <stdio.h>



int EnetStart();
void Enet();
void X_PosCheck();

ENetAddress address;
ENetHost *client;
ENetPeer *peer;
char message[1024];
ENetEvent event;
int eventStatus;
bool unIdentified = true;
int clientID = 0;
int tempY = 0;

int roundCounter = 0;

game *Game = nullptr;

int main(int argc, char* arg[])
{
	
	const int fps = 60;
	const int frameDelay = 1000 / fps;

	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last = 0;

	Uint32 frameStart;
	int frameTime;

	EnetStart();

	Game = new game();
	Game->Init("Pong: 1 vs. 1");

	while (Game->Running())
	{
		frameStart = SDL_GetTicks();

		Game->HandleEvents();
		if (roundCounter == 10)
		{
			Enet();
			roundCounter = 0;
		}
		X_PosCheck();
		Game->Update(clientID, tempY);
		Game->Render();
		//printf("%d", clientID);

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) 
		{
			SDL_Delay(frameDelay - frameTime);
		}
		roundCounter++;
	}
	
	atexit(enet_deinitialize);
	Game->Clean();

	return 0;
}

int EnetStart()
{  //init
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	//Create a host using enet_host_create
	client = enet_host_create(NULL, 1, 2, 57600 / 8, 14400 / 8);

	
	if (client == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENet server host\n");
		exit(EXIT_FAILURE);
	}

	enet_address_set_host(&address, "192.168.1.3");
	address.port = 2317;

	// Connect and user service
	peer = enet_host_connect(client, &address, 0, 0);

	if (peer == NULL) {
		fprintf(stderr, "No available peers for initializing an ENet connection");
		exit(EXIT_FAILURE);
	}

	eventStatus = 1;

	X_PosCheck();

	return 0;
}

void Enet() 
{
	SDL_Delay(1000/60);
	eventStatus = enet_host_service(client, &event, 1000/60);
	int dataP[6];


	// If we had some event that interested us
	if (eventStatus > 0) {
		switch (event.type) {
		case ENET_EVENT_TYPE_CONNECT:
		{
			printf("(Client) We got a new connection from %x\n",
				event.peer->address.host);
			break;
		}
		case ENET_EVENT_TYPE_RECEIVE:
			
			
			
			//std::cout << "receive" << std::endl;
			int* p = (int*)event.packet->data;
			for (int i = 0; i < 6; i++)
			{
				dataP[i] = p[i];
			}

			for (int i = 0; i < 6; i++)
			{
				std::cout << dataP[i] << std::endl;

			}
			//std::cout << dataP[0] << std::endl;
			//std::cout << p[0] << std::endl;
			//if (clientID==1)
			//{
			//	Game->Player2->posY = dataP[1];
			//}
			//else if (clientID == 2)
			//{
			//	Game->Player2->posY = dataP[0];
			//}

			
			//Game->Player2->posY = dataP[1];
			//std::cout << dataP[1] << std::endl;

            

			Game->ballX = dataP[3];
			Game->ballY = dataP[4];

			if (unIdentified)
			{
				
				clientID = dataP[5];
				unIdentified = false;
				
			}

            if (clientID == 1) { tempY = dataP[2]; }

			/*
			if (data[4] == 1) 
			{
				Game->Player1->posX = 15;
			}
			else
			{
				Game->Player1->posX = 700 - 25;
			}*/
			// Lets broadcast this message to all
			// enet_host_broadcast(client, 0, event.packet);
			/*
			case ENET_EVENT_TYPE_DISCONNECT:
			printf("(Client) %s disconnected.\n", event.peer->data);

			// Reset client's information
			event.peer->data = NULL;
			break;
			*/
			break;
		}
	}

	
	//dataP[1] = Game->Player1->posY;
	if (clientID == 1)
	{
		
		Game->Player2->posY = dataP[2];
	}
	else if (clientID == 2)
	{
		dataP[2] = Game->Player1->posY;
		Game->Player2->posY = dataP[1];
	}
	//	
	//if (Game->Player1->posY != tempY) 
	//{
		//tempY = Game->Player1->posY;
	dataP[1] = Game->Player1->posY;
		ENetPacket *packet = enet_packet_create((void*)dataP, (sizeof(dataP)+1), ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		enet_peer_send(peer, 0, packet);
		enet_peer_send(peer, 0, packet);
		enet_peer_send(peer, 0, packet);
		std::cout << "I sent data" << std::endl;
	//}
}

void X_PosCheck()
{
	if (clientID == 1)
	{
		Game->Player1->posX = 15;
		Game->Player2->posX = 775;
	}
	if (clientID == 2)
	{
		Game->Player2->posX = 15;
		Game->Player1->posX = 775;
	}
}









//client____________________________
/*
int main(int argc, char **argv)
{
	ENetAddress address;
	ENetHost *client;
	ENetPeer *peer;
	char message[1024];
	ENetEvent event;
	int eventStatus;

	// a. Initialize enet
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENet.\n");
		return EXIT_FAILURE;
	}

	atexit(enet_deinitialize);

	// b. Create a host using enet_host_create
	client = enet_host_create(NULL, 1, 2, 57600 / 8, 14400 / 8);

	if (client == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENet server host\n");
		exit(EXIT_FAILURE);
	}

	enet_address_set_host(&address, "localhost");
	address.port = 1234;

	// c. Connect and user service
	peer = enet_host_connect(client, &address, 2, 0);

	if (peer == NULL) {
		fprintf(stderr, "No available peers for initializing an ENet connection");
		exit(EXIT_FAILURE);
	}

	eventStatus = 1;

	while (1) {
		eventStatus = enet_host_service(client, &event, 50000);

		// If we had some event that interested us
		if (eventStatus > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
			{
				printf("(Client) We got a new connection from %x\n",
					event.peer->address.host);

				float data[2];
				data[0] = 0;
				data[1] = 0;
				ENetPacket *packet = enet_packet_create((void*)data, sizeof(data), ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(event.peer, 0, packet);

				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
				printf("(Client) Message from server : %s\n", event.packet->data);
				// Lets broadcast this message to all
				// enet_host_broadcast(client, 0, event.packet);
				enet_packet_destroy(event.packet);
				break;
				/*
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("(Client) %s disconnected.\n", event.peer->data);

				// Reset client's information
				event.peer->data = NULL;
				break;
				
			}
		}
	}
}
*/