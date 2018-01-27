#include <SDL.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <enet/enet.h>
#include <string.h>
#include <stdio.h>
#include <vector>

#define SDL_main main
// server.c
#include <enet/enet.h>
#include <stdio.h>

struct object{
	int Id; 
	int Px; 
	int Py;
	ENetPeer* info;

	object(int i, int x, int y) :Id(i), Px(x), Py(y){}
};


int main(int argc, char **argv)
{
	ENetAddress address;
	ENetHost *server;
	ENetEvent event;
	int playernum=0;
	int eventStatus;
	std::vector<object> objects;
	int PaddleW = 10;
	int PaddleH = 70;


	// a. Initialize enet
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENet.\n");
		return EXIT_FAILURE;
	}

	atexit(enet_deinitialize);

	// b. Create a host using enet_host_create
	address.host = ENET_HOST_ANY;
	address.port = 1234;

	server = enet_host_create(&address, 32, 2, 0, 0);

	if (server == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENet server host\n");
		exit(EXIT_FAILURE);
	}
	object Ball(playernum, 0, 0);
	Ball.info = 0;
	objects.push_back(Ball);
	playernum++;
	// c. Connect and user service
	eventStatus = 1;

	SDL_Rect pallo = {objects[0].Px,objects[0].Py,10,10};
	SDL_Rect play1 = { -100,-100, PaddleW,PaddleH };
	SDL_Rect play2 = { -100,-100, PaddleW,PaddleH };
	


	while (1) {
		eventStatus = enet_host_service(server, &event, 50000);
		
		if (objects.size() >= 2)
		{
			play1.x = objects[1].Px;
			play1.y = objects[1].Py;
		}
		if (objects.size() >= 3)
		{
			play2.x = objects[2].Px;
			play2.y = objects[2].Py;
		}

		//__________collision hell__________











		// If we had some event that interested us
		if (eventStatus > 0) {
			switch (event.type) {
			
			case ENET_EVENT_TYPE_CONNECT:
			{
				printf("(Server) We got a new connection from %x\n",
					event.peer->address.host);
				object test(playernum, 0, 0);
				test.info = event.peer;
				objects.push_back(test);
				playernum++;


				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
			{
				
				//int *data = (int*)event.packet->data;
				/*
				int Py = data[0];
				int Bx = data[1];
				int By = data[2];
				*/
				if (objects.size() >= 2)
				{
					for (int i = 0; i < objects.size(); i++)
					{
						if (objects[i].info == event.peer)
						{
							int *data = (int*)event.packet->data;
							objects[i].Px == data[0];
							objects[i].Py == data[1];
						}

					}
				}
				int *data = (int*)event.packet->data;
				
				

				

				// Lets broadcast this message to all
				if (playernum >= 2)
				{
					if (objects.size() >= 3)
					{
						for (int i = 0; i < objects.size(); i++)
						{
							if (objects[i].info != event.peer || objects[i].info != 0)
							{
								if (objects[i].Id == 1)
								{
									data[4] == 1;
									ENetPacket* packit = enet_packet_create(data, sizeof(data), ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);
									enet_peer_send(objects[i].info, sizeof((void*)data), packit);
								}
								else if (objects[i].Id == 2)
								{
									data[4] == 2;
									ENetPacket* packit = enet_packet_create(data, sizeof(data), ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);
									enet_peer_send(objects[i].info, sizeof((void*)data), packit);
								}
							}

						}
					}

				}
				else
				{
					ENetPacket* packit = enet_packet_create((void*)data, sizeof(data), ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);
					enet_host_broadcast(server, 0, packit);
				}

				break;
			}

			case ENET_EVENT_TYPE_DISCONNECT:
			{
				//printf("%s disconnected.\n", event.peer->data);

				// Reset client's information
				event.peer->data = NULL;

				playernum--;

				break;
			}
			}
		}
	}

}