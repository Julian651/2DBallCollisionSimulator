#include "Simulation.h"
#include "Frame.h"
#include "Ball.h"
#include "Random.h"
#include <vector>
#include <iostream>


std::vector<Ball*> balls(0);
Random* RNG = new Random(0);


Simulation::Simulation()
{

   m_bAlive = true;
   m_bStarted = false;
}

bool Simulation::isStarted()
{
   return m_bStarted;
}

bool Simulation::isAlive()
{
   return m_bAlive;
}

void Simulation::run()
{
   //std::cout << "Running..." << std::endl;

   std::unique_lock<std::mutex> mlock(m_mtx);

   if (m_bAlive)
   {
      m_started.wait(mlock, [this] { return m_bStarted; });
      std::thread thread_one(&Simulation::runSimulation, this);
      thread_one.detach();
      m_paused.wait(mlock, [this] { return !m_bStarted; });
      m_stopped.wait(mlock, [this] { return !m_bAlive; });
   }

   //std::cout << "Ending..." << std::endl;
}

void Simulation::start()
{
   // Make this thread sleep for half a second
   //std::this_thread::sleep_for(std::chrono::milliseconds(500));

   //std::cout << "Starting..." << std::endl;

   std::lock_guard<std::mutex> guard(m_mtx);
   m_bStarted = true;
   m_started.notify_all();
}

void Simulation::pause()
{
   // Make this thread sleep for half a second
   //std::this_thread::sleep_for(std::chrono::milliseconds(500));

   //std::cout << "Pausing..." << std::endl;

   std::lock_guard<std::mutex> guard(m_mtx);
   m_bStarted = false;
   m_paused.notify_one();
}

void Simulation::stop()
{
   // Make this thread sleep for half a second
   //std::this_thread::sleep_for(std::chrono::milliseconds(500));
   //std::cout << "Stopping..." << std::endl;
   m_bAlive = false;
   m_bStarted = true;
   m_started.notify_one();
   std::this_thread::sleep_for(std::chrono::milliseconds(1));
   m_bStarted = false;
   m_paused.notify_one();
   std::lock_guard<std::mutex> guard(m_mtx);
   m_stopped.notify_one();

   for (int i = 0; i < (int)balls.size(); i++)
   {
      delete balls[i];
      balls[i] = nullptr;
   }
   delete RNG;
}

void Simulation::runSimulation()
{
   int count = 0;
   while (m_bAlive)
   {
      std::cout << "is Alive" << std::endl;
      if (m_bStarted)
      {
         //std::cout << "Running simulation... " << count << std::endl;
         for (int i = 0; i < (int)balls.size(); i++)
         {
            balls[i]->update(1.0f / 60.0f);
         }
         //std::this_thread::sleep_for(std::chrono::milliseconds(10));
         count++;
      }
      else
      {
         std::unique_lock<std::mutex> mlock(m_mtx);
         m_started.wait(mlock, [this] { return m_bStarted; });
      }
      Frame::repaint();
   }
}

void Simulation::paint()
{
   Graphics* g = Frame::getGraphics();
   
   g->beginDraw();
   g->clear();

   //g->DrawFish(100.0f, 100.0f, 1.0f, 20.0f);
   for (int i = 0; i < (int)balls.size(); i++)
   {
      balls[i]->draw(g, balls[i]->getBallInfo()->color);
   }
   if (!m_bStarted) 
   {
      g->setBrush(D2D1::ColorF::Black);
      g->DrawString(L"Paused. Press 'SPACE' to unpause.", 0, 0, 30.0f);
   }
   else
   {
      g->setBrush(D2D1::ColorF::Black);
      g->DrawString(L"Press 'SPACE' to pause.", 0, 0, 30.0f);
   }
   g->setBrush(D2D1::ColorF::Black);
   g->DrawString(L"Left click to add a ball.", 600.0f, 15.0f, 15.0f);
   g->endDraw();
}

void Simulation::addBall(int x, int y)
{
   BALL_INFO* bi = new BALL_INFO;
   bi->mass = RNG->nextFloat() * 50.f + 5.0f;
   bi->radius = bi->mass;
   bi->pos = { (float)x, (float)y, 0 };
   bi->vel = { (RNG->nextFloat() - 0.5f) * 1000.0f , (RNG->nextFloat() - 0.5f) * 1000.0f, 0 };
   //bi->vel = { };
   bi->acc = { 0.0f, 98.f, 0.0f};
   bi->color = D2D1::ColorF::ColorF(RNG->nextFloat(), RNG->nextFloat(), RNG->nextFloat());
   balls.push_back(new Ball(bi));
}
