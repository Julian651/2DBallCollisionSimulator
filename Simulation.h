#pragma once
#include <mutex>
#include <condition_variable>

class Simulation
{
private:
   std::mutex m_mtx;
   std::condition_variable m_started;
   std::condition_variable m_paused;
   std::condition_variable m_stopped;

   bool m_bAlive;
   bool m_bStarted;
public:
   Simulation();
   bool isStarted();
   bool isAlive();
   void run();
   void start();
   void pause();
   void stop();
   void runSimulation();
   void addBall(int x, int y);
   void paint();
};

