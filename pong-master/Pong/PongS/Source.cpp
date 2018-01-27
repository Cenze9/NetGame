#include <SDL.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <enet/enet.h>
#include <string.h>
#include <stdio.h>

#define SDL_main main

//
//void main() 
//{
//	if (enet_initialize() != 0)
//	{
//		fprintf(stderr, "An error occurred while initializing ENet.\n");
//	}
//	ENetAddress address;
//	ENetHost * server;
//	/* Bind the server to the default localhost.     */
//	/* A specific host address can be specified by   */
//	/* enet_address_set_host (& address, "x.x.x.x"); */
//	address.host = ENET_HOST_ANY;
//	/* Bind the server to port 1234. */
//	address.port = 1234;
//	server = enet_host_create(&address /* the address to bind the server host to */,
//		32      /* allow up to 32 clients and/or outgoing connections */,
//		2      /* allow up to 2 channels to be used, 0 and 1 */,
//		0      /* assume any amount of incoming bandwidth */,
//		0      /* assume any amount of outgoing bandwidth */);
//	if (server == NULL)
//	{
//		fprintf(stderr,
//			"An error occurred while trying to create an ENet server host.\n");
//		exit(EXIT_FAILURE);
//	}
//
//	ENetEvent event;
//	/* Wait up to 1000 milliseconds for an event. */
//	while (enet_host_service(server, &event, 1000) > 0)
//	{
//		switch (event.type)
//		{
//		case ENET_EVENT_TYPE_CONNECT:
//			printf("A new client connected from %x:%u.\n",
//				event.peer->address.host,
//				event.peer->address.port);
//			/* Store any relevant client information here. */
//			event.peer->data = "Client information";
//			break;
//		case ENET_EVENT_TYPE_RECEIVE:
//			printf("A packet of length %u containing %s was received from %s on channel %u.\n",
//				event.packet->dataLength,
//				event.packet->data,
//				event.peer->data,
//				event.channelID);
//			/* Clean up the packet now that we're done using it. */
//			enet_packet_destroy(event.packet);
//
//			break;
//
//		case ENET_EVENT_TYPE_DISCONNECT:
//			printf("%s disconnected.\n", event.peer->data);
//			/* Reset the peer's client information. */
//			event.peer->data = NULL;
//		}
//	}
//	
//	
//
//	enet_host_destroy(server);
//	atexit(enet_deinitialize);
//
//}
//


// server.c
#include <enet/enet.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	ENetAddress address;
	ENetHost *server;
	ENetEvent event;
	int eventStatus;

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

	// c. Connect and user service
	eventStatus = 1;

	while (1) {
		eventStatus = enet_host_service(server, &event, 50000);

		// If we had some event that interested us
		if (eventStatus > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				printf("(Server) We got a new connection from %x\n",
					event.peer->address.host);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
			{
				printf("(Server) Message from client : %s\n", event.packet->data);
				float *data = (float*)event.packet->data;
				float x = data[0];
				float y = data[1];
				printf("%.3f, %.3f\n", x, y);
				// Lets broadcast this message to all
				enet_host_broadcast(server, 0, event.packet);
				break;
			}

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);

				// Reset client's information
				event.peer->data = NULL;
				break;

			}
		}
	}

}







//client____________________________

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
				printf("(Client) We got a new connection from %x\n",
					event.peer->address.host);

				float data[2];
				data[0] = 0;
				data[1] = 0;
				ENetPacket *packet = enet_packet_create((void*)data, sizeof(data), ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(event.peer, 0, packet);

				break;

			case ENET_EVENT_TYPE_RECEIVE:
				printf("(Client) Message from server : %s\n", event.packet->data);
				// Lets broadcast this message to all
				// enet_host_broadcast(client, 0, event.packet);
				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("(Client) %s disconnected.\n", event.peer->data);

				// Reset client's information
				event.peer->data = NULL;
				break;
			}
		}
	}
}