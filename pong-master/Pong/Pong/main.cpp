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

ENetAddress address;
	ENetHost *client;
	ENetPeer *peer;
	char message[1024];
	ENetEvent event;
	int eventStatus;

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
		Enet();
		Game->Update();
		Game->Render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) 
		{
			SDL_Delay(frameDelay - frameTime);
		}
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

	enet_address_set_host(&address, "127.0.0.1");
	address.port = 2317;

	// Connect and user service
	peer = enet_host_connect(client, &address, 0, 0);

	if (peer == NULL) {
		fprintf(stderr, "No available peers for initializing an ENet connection");
		exit(EXIT_FAILURE);
	}

	eventStatus = 1;
	return 0;
}

void Enet() 
{
	eventStatus = enet_host_service(client, &event, 5);
	int dataP[6];
	ENetPacket *packit;

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
			
			std::cout << "receive" << std::endl;
			int* data = (int*)event.packet->data;
			packit = event.packet;
			
			
			Game->Player2->posY = data[1];
			Game->ballX = data[2];
			Game->ballY = data[3];

			if (data[4] == 1) 
			{
				Game->Player1->posX = 15;
			}
			else
			{
				Game->Player1->posX = 700 - 25;
			}

			// Lets broadcast this message to all
			// enet_host_broadcast(client, 0, event.packet);
			
			break;
			/*
			case ENET_EVENT_TYPE_DISCONNECT:
			printf("(Client) %s disconnected.\n", event.peer->data);

			// Reset client's information
			event.peer->data = NULL;
			break;
			*/
		}
	}

	if (event.packet != nullptr)
	{
		int* datap = (int*)event.packet->data;
		dataP[1] = Game->Player1->posY;
		enet_packet_destroy(event.packet);
	}
	ENetPacket *packet = enet_packet_create((void*)dataP, sizeof(dataP), ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);

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