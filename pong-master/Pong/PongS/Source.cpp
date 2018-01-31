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

int PaddleW = 10;
int PaddleH = 70;
float bVelX = 0;
float bVelY = 0;

int CheckCollision(object paddle, object ball);

int main(int argc, char **argv)
{
	ENetAddress address;
	ENetHost *server;
	ENetEvent event;
	ENetPacket* packit;
	int playernum=0;
	int eventStatus;
	std::vector<object> objects;
	int data[5];
	


	// a. Initialize enet
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENet.\n");
		return EXIT_FAILURE;
	}



	// b. Create a host using enet_host_create
	address.host = ENET_HOST_ANY;
	address.port = 2317;

	server = enet_host_create(&address, 3, 0, 0, 0);
	printf("Ready", playernum,"\n");
	if (server == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENet server host\n");
		exit(EXIT_FAILURE);
	}
	object Ball(playernum, 0, 0);
	Ball.info = 0;
	objects.push_back(Ball);
	// c. Connect and user service
	eventStatus = 1;

	SDL_Rect pallo = {objects[0].Px,objects[0].Py,10,10};
	SDL_Rect play1 = { -100,-100, PaddleW,PaddleH };
	SDL_Rect play2 = { -100,-100, PaddleW,PaddleH };
	


	while (1) {
		eventStatus = enet_host_service(server, &event, 5);
		
		if (objects.size() >= 2)
		{
			play1.x = objects[1].Px;
			play1.y = objects[1].Py;
		}
		if (objects.size() >= 3)
		{
			play2.x = objects[2].Px;
			play2.y = objects[2].Py;

			// CheckCollision(objects[1], objects[0]);
			// CheckCollision(objects[2], objects[0]);

			// objects[0].Px += bVelX*(32.0f* 0.001f) * 5;
			// objects[0].Py += bVelY*(32.0f* 0.001f) * 5;
		}


		// If we had some event that interested us
		if (eventStatus > 0) {
			switch (event.type) {
			
			case ENET_EVENT_TYPE_CONNECT:
			{
				printf("(Server) We got a new connection from %x\n",
				event.peer->address.host);
				int Id_Data[5]{ 0,50,0,0,0 };
				object test(playernum, 0, 0);
				test.info = event.peer;
				objects.push_back(test);

				playernum++;

				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
			{
				//std::cout << "receive" << std::endl;
				//int *data = (int*)event.packet->data;
				/*
				int Py = data[0];
				int Bx = data[1];
				int By = data[2];
				*/
				if (objects.size() > 2)
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
				
				

				int *p = (int*)event.packet->data;
				

				//memcpy

				for (int i = 0; i < 5; i++)
				{
					data[i] = p[i];
				}



				//data[1] = 50;
				/*
				if (objects.size()==2)
				{
					data[4] == 1;
				}
				else if (objects.size()==3)
				{
					data[4] == 2;
				}

				*/

				
				//printf("%d\n", data[4]);
				
				

				data[4] = playernum;

				printf("%d", playernum, "\n");
				
				
				packit = enet_packet_create((void*)data, sizeof(data), ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);
				enet_host_broadcast(server, 0, packit);
				
				/*
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
				*/

				break;
			}

			case ENET_EVENT_TYPE_DISCONNECT:
			{
				//printf("%s disconnected.\n", event.peer->data);
				printf("someone left\n");
				// Reset client's information
				event.peer->data = NULL;
				printf("%d", playernum, "\n");
				playernum--;
				printf("%d", playernum, "\n");
				break;
			}

			data[4] = playernum;

			printf("%d", playernum, "\n");


			packit = enet_packet_create((void*)data, sizeof(data), ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);
			enet_host_broadcast(server, 0, packit);

			}
		}
	}

	atexit(enet_deinitialize);

}


int CheckCollision(object paddle, object ball)
{
	if (ball.Px<15 || ball.Px>(775 + PaddleW))
	{
		return 3;
	}



	int by0 = ball.Py;
	int by1 = ball.Py + 10;
	int ry0 = paddle.Py;
	int ry1 = paddle.Py + PaddleH;

	bool happened = ((ry0 < by0 && by0 < ry1) || (ry0 < by1 && by1 < ry1));

	if (!happened) {
		return 0;
	}

	

	int mby = by0 + 5;
	int mry = ry0 + 5;
	float mid_distance = mry - mby;
	float RACKET_HITBACK_MAXANGLE = 85.0f*M_PI / 180.0f;
	float angle = RACKET_HITBACK_MAXANGLE * (mid_distance / (PaddleH/2.0f+5.0f));

	bVelY = -sinf(angle); // Y increases as you go down, not up.
	bVelX = bVelX < 0 ? cosf(angle) : -cosf(angle);

	return 1;


}