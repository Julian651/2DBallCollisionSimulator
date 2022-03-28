#include "Ball.h"
#include "Frame.h"
#include <iostream>
#include <string>
#include <thread>

std::vector<Ball*> Ball::balls(0);

void Ball::update(float timeElapsed)
{

   // x = 0.5 * a * t * t + v0 * t + x0
   biBallInfo->pos.x = (0.5f * biBallInfo->acc.x * timeElapsed * timeElapsed) + (biBallInfo->vel.x * timeElapsed) + biBallInfo->pos.x;
   biBallInfo->pos.y = (0.5f * biBallInfo->acc.y * timeElapsed * timeElapsed) + (biBallInfo->vel.y * timeElapsed) + biBallInfo->pos.y;
   biBallInfo->pos.z = (0.5f * biBallInfo->acc.z * timeElapsed * timeElapsed) + (biBallInfo->vel.z * timeElapsed) + biBallInfo->pos.z;

   // v = a * t + v0
   biBallInfo->vel.x = biBallInfo->acc.x * timeElapsed + biBallInfo->vel.x;
   biBallInfo->vel.y = biBallInfo->acc.y * timeElapsed + biBallInfo->vel.y;
   biBallInfo->vel.z = biBallInfo->acc.z * timeElapsed + biBallInfo->vel.z;


   float3 pos2 = { };
   pos2.x = (0.5f * biBallInfo->acc.x * timeElapsed * timeElapsed) + (biBallInfo->vel.x * timeElapsed) + biBallInfo->pos.x;
   pos2.y = (0.5f * biBallInfo->acc.y * timeElapsed * timeElapsed) + (biBallInfo->vel.y * timeElapsed) + biBallInfo->pos.y;
   pos2.z = (0.5f * biBallInfo->acc.z * timeElapsed * timeElapsed) + (biBallInfo->vel.z * timeElapsed) + biBallInfo->pos.z;

   checkCollision(pos2, biBallInfo->radius);
}

void Ball::checkCollision(float3 pos, float radius)
{
   if (pos.x - biBallInfo->radius < 0 || pos.x + biBallInfo->radius > Frame::getWidth() - 16)
   {
      biBallInfo->vel.x = -biBallInfo->vel.x;
      biBallInfo->collidedWith = NULL;
   }
   if (pos.y - biBallInfo->radius < 0 || pos.y + biBallInfo->radius > Frame::getHeight() - 40)
   {
      biBallInfo->vel.y = -biBallInfo->vel.y;
      biBallInfo->collidedWith = NULL;
   }
   for (int i = 0; i < (int)balls.size(); i++)
   {
      float radius2 = balls[i]->biBallInfo->radius;

      float distance = dist(pos, balls[i]->biBallInfo->pos);
      bool touches = distance <= radius + radius2;
      bool intersects = distance > radius - radius2 && distance < radius + radius2;
      bool touchesInten = distance <= radius - radius2;
      if (balls[i] != this)
      {
         if (intersects && (biBallInfo->collidedWith != balls[i]->biBallInfo || balls[i]->biBallInfo->collidedWith != biBallInfo))
            collide(biBallInfo, balls[i]->biBallInfo);
      }
   }
}

float Ball::dist(float3 C1, float3 C2)
{
   float3 pos3 = { C2.x - C1.x, C2.y - C1.y, C2.z - C1.z };

   float dist = (float)sqrt(pos3.x * pos3.x + pos3.y * pos3.y);
   return dist;
}

void Ball::draw(Graphics* g, D2D1::ColorF color)
{
   //g->beginDraw();
   //g->clear();
   g->setBrush(color);
   float3 pos = biBallInfo->pos;
   g->FillCircle(pos.x, pos.y, biBallInfo->radius);;
   //g->endDraw();
}
void Ball::collide(BALL_INFO* info, BALL_INFO* other)
{
   float m1 = info->mass;
   float m2 = other->mass;
   float3 u1 = info->vel;
   float3 u2 = other->vel;
   
   float3 v1 = { };
   v1.x = (((m1 - m2) / (m1 + m2)) * u1.x) + (((2 * m2) / (m1 + m2)) * u2.x);
   v1.y = (((m1 - m2) / (m1 + m2)) * u1.y) + (((2 * m2) / (m1 + m2)) * u2.y);
   v1.z = (((m1 - m2) / (m1 + m2)) * u1.z) + (((2 * m2) / (m1 + m2)) * u2.z);

   float3 v2 = { };
   v2.x = (((2 * m1) / (m1 + m2)) * u1.x) + (((m2 - m1) / (m1 + m2)) * u2.x);
   v2.y = (((2 * m1) / (m1 + m2)) * u1.y) + (((m2 - m1) / (m1 + m2)) * u2.y);
   v2.z = (((2 * m1) / (m1 + m2)) * u1.z) + (((m2 - m1) / (m1 + m2)) * u2.z);

   info->vel = v1;
   other->vel = v2;

   info->collidedWith = other;
   other->collidedWith = info;

   //printf("Ball 1: m - %.2f; v.x - %.2f\nBall 2: m - %.2f; v.x - %.2f\n", info->mass, info->vel.x, other->mass, other->vel.x);
   //printf("Total momentum: %.2f\n\n", info->mass * info->vel.x + other->mass * other->vel.x);
}