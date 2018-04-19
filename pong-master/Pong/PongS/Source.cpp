#include <SDL.h>
#include <iostream>

#include <cstring>
#include <enet/enet.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <math.h>

#define SDL_main main
// server.c

struct object {
    int Id;
    int Px = 0;
    int Py = 0;
    ENetPeer* info;

    object(int i, int x, int y) :Id(i), Px(x), Py(y) {}
};



int PaddleW = 10;
int PaddleH = 70;
float bVelX = -1;
float bVelY = 0;
int CheckCollision(object paddle, object ball);

int main(int argc, char **argv)
{
    ENetAddress address;
    ENetHost *server;
    ENetEvent event;
    ENetPacket* packit;
    int playernum = 0;
    int eventStatus;
    std::vector<object> objects;
    int data[6];



    // a. Initialize enet
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occured while initializing ENet.\n");
        return EXIT_FAILURE;
    }



    // b. Create a host using enet_host_create
    address.host = ENET_HOST_ANY;
    address.port = 2317;

    server = enet_host_create(&address, 3, 0, 0, 0);

    if (server == NULL) {
        fprintf(stderr, "An error occured while trying to create an ENet server host\n");
        exit(EXIT_FAILURE);
    }
    object Ball(playernum, 0, 0);
    Ball.info = 0;
    objects.push_back(Ball);
    // c. Connect and user service
    eventStatus = 1;

    SDL_Rect pallo = { objects[0].Px,objects[0].Py,10,10 };


    data[0] = 0;
    printf("Ready %d", playernum, "\n");
    while (1) {

        eventStatus = enet_host_service(server, &event, 1000);

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


                int *p = (int*)event.packet->data;


                //memcpy
                //only need [1] and [2] spots from players
                if (abs((int)p[1]) < 700 && abs((int)p[1]) > -100)
                {
                    data[1] = (int)p[1];
                }
                if (abs((int)p[2]) < 700 && abs((int)p[1]) > -100)
                {
                    data[2] = (int)p[2];
                }

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

                for (int i = 0; i < objects.size(); i++)
                {
                    if (objects[i].info == event.peer)
                    {

                    }
                }

                break;
            }

            }
        }






        objects[0].Px += bVelX;
        objects[0].Py += bVelY;


        // objects[0].Px += bVelX*(32.0f* 0.001f) * 5;
        // objects[0].Py += bVelY*(32.0f* 0.001f) * 5;



        if (objects.size() >= 2)
        {
            objects[1].Px = 15;


            objects[1].Py = data[1];


            if (objects[0].Px < 100)
            {
                CheckCollision(objects[1], objects[0]);
            }
        }
        if (objects.size() >= 3)
        {
            objects[2].Px = 775;


            objects[2].Py = data[2];


            if (objects[0].Px > 690)
            {
                CheckCollision(objects[2], objects[0]);
            }
            printf("Two players \n");

            printf("%d,%d\n", objects[1].Py, objects[2].Py);



        }


        //  BALL LOCATION CHECK

        if (objects[0].Px<0)
        {
            data[0] = 3;
            objects[0].Px = 400;
            objects[0].Py = 350;
        }
        else if (objects[0].Px >800)
        {
            data[0] = 4;
            objects[0].Px = 400;
            objects[0].Py = 350;
        }

        if (objects[0].Py < 0 && bVelY<0)
        {
            bVelY = (bVelY * -1);
        }
        else if (objects[0].Py > 690 && bVelY>0)
        {
            bVelY = (bVelY * -1);
        }



        printf("Ball: \n");

        printf("Xv:%2f Yv:%2f\n", bVelX, bVelY);
        printf(" X:%d  Y:%d\n", objects[0].Px, objects[0].Py);



        data[3] = objects[0].Px;
        data[4] = objects[0].Py;
        data[5] = playernum;

        packit = enet_packet_create((void*)data, (sizeof(data) + 1), ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);
        enet_host_broadcast(server, 0, packit);

    }

    atexit(enet_deinitialize);
    return 0;
}


int CheckCollision(object paddle, object ball)
{
    int by0 = ball.Py;
    int by1 = ball.Py + 10;
    int bx0 = ball.Px;
    int bx1 = ball.Px + 10;

    int ppx = ball.Px + 5;
    int ppy = ball.Py + 5;

    int ry0 = paddle.Py;
    int ry1 = paddle.Py + 70;
    int rx0 = paddle.Px;
    int rx1 = paddle.Px + 10;

    bool happened = false;

    if (rx1 < 50) // Left Paddle (Player 1)
    {
        if (rx1 >= bx0)
        {

            if (ry0 <= by1 && ry1 >= by0)
            {
                happened = true;
            }
        }
    }
    if (rx0 > 750) // Right Paddle (Payer 2)
    {

        if (rx0 <= bx1)
        {

            if (ry0 <= by1 && ry1 >= by0)
            {
                happened = true;


            }
        }
    }

    /*
    if(ppx > rx0 && ppx < rx1)
    {
    if (ppy > ry0 && ppy < ry1)
    {
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    }
    }
    */



    /* if (pp > 600) {
    happened = (rx0 > pp);
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    }
    else if (pp < 200)
    {
    happened = (rx0 > pp);
    }
    */
    //bool happened = ((ry0 < by0 && by0 < ry1) || (ry0 < by1 && by1 < ry1) ));



    if (!happened) {
        return 0;

    }

    int mby = by0 + 5;
    int mry = ry0 + 35;
    float mid_distance = mry - mby;
    float RACKET_HITBACK_MAXANGLE = 85.0f*M_PI / 180.0f;
    float angle = RACKET_HITBACK_MAXANGLE * (mid_distance / (70 / 2.0f + 5.0f));

    bVelY = round(-sin(angle)); // Y increases as you go down, not up.
    bVelX = (-bVelX);
    //bVelX = bVelX < 0 ? cosf(angle) : -cosf(angle);


    return 1;


}