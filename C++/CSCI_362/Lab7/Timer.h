/*
  Filename   : Timer.h
  Author     : Zachary Zuch
  Course     : CSCI 362
  Assignment : Lab6
  Description: A Timer class for timing algorithms
*/   

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef TIMER_H         
#define TIMER_H

/************************************************************/
// System includes

#include <ctime>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

/************************************************************/

class Timer
{
public:
 
  Timer ();

  // Resets elapsed time to zero
  void
  reset ();

  void
  start ();

  void
  stop ();

  // Return elapsed time in milliseconds
  double
  getElapsedMs () const;

private:

  double MILLISECONDS_PER_NANOSECOND = 100000.0;

  struct timespec m_start;
  struct timespec m_stop;

};

/************************************************************/

#endif

/************************************************************/
