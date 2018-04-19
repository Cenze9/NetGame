#include <iostream>
#include <stdio.h>
#include "game.h"
#include <SDL.h>
#include <enet/enet.h>

int EnetStart();
void Enet();
void X_PosCheck();

ENetAddress address;
ENetHost *client;
ENetPeer *peer;
ENetEvent event;

bool unIdentified = true;
int eventStatus;
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
        //Enet();
        if (roundCounter == 2)
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
    //172.31.16.26
    enet_address_set_host(&address, "192.168.1.5");
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
    int dataP[6];

    // If we had some event that interested us
    while ((eventStatus = enet_host_service(client, &event, 0)) > 0) 
    {
        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
        {
            printf("(Client) We got a new connection from %x\n",
                event.peer->address.host);
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE:

            int* p = (int*)event.packet->data;
            for (int i = 0; i < 6; i++)
            {
                dataP[i] = (int)p[i];
            }

            Game->ballX = dataP[3];
            Game->ballY = dataP[4];

            if (unIdentified)
            {
                clientID = dataP[5];
                unIdentified = false;
            }

            break;
        }
    }

    if (clientID == 1)
    {
        if (abs(dataP[2]) < 700)
        {
            tempY = dataP[2];
        }

        dataP[1] = Game->Player1->posY;
        dataP[2] = -2000;
    }
    else if (clientID == 2)
    {
        if (abs(dataP[1]) < 700)
        {
            tempY = dataP[1];
        }

        dataP[2] = Game->Player1->posY;
        dataP[1] = -2000;
    }

    //Send Packet
    ENetPacket *packet = enet_packet_create((void*)dataP, (sizeof(dataP) + 1), ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);

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