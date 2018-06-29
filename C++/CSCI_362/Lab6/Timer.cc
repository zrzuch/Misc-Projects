/*
  Filename   : Timer.cc
  Author     : Zachary Zuch
  Course     : CSCI 362
  Assignment : Lab6
  Description: The Timer class implementation file
*/   

/************************************************************/
// System includes

#include <ctime>
#include <time.h>

/************************************************************/
// Local includes

#include "Timer.h"

/************************************************************/

Timer::Timer ( )
	: m_start( ) , m_stop( )
{ }

void
Timer::reset ()
{
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_start);
  m_stop = m_start;
}

void
Timer::start ()
{
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_start);
}

void
Timer::stop ()
{
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_stop);
}

double
Timer::getElapsedMs () const
{
  return ( m_stop.tv_nsec - m_start.tv_nsec ) 
            / MILLISECONDS_PER_NANOSECOND;
}

/************************************************************/
