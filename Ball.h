#pragma once
#include <vector>
#include "Graphics.h"

struct float3
{
   float x = 0.0f, y = 0.0f, z = 0.0f;
};
struct BALL_INFO
{
   float mass = 1.0f, radius = 1.0f, elasticity = 0.5f;
   float3 pos = { }, vel = { }, acc = { };
   BALL_INFO* collidedWith = nullptr;
   D2D1::ColorF color = D2D1::ColorF::Red;
};
class Ball
{
private:
   BALL_INFO* biBallInfo;
public:
   static std::vector<Ball*> balls;
public:
   Ball(BALL_INFO* ballInfo) : biBallInfo{ ballInfo } { balls.push_back(this); }
   void update(float timeElapsed);
   BALL_INFO* getBallInfo() { return this->biBallInfo; }
   void collide(BALL_INFO* info, BALL_INFO* other);
   void checkCollision(float3 pos, float radius);
   void draw(Graphics* g, D2D1::ColorF color);
   float dist(float3 pos1, float3 pos2);


};

