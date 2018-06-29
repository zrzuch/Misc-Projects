/*
  Author     : Gary M. Zoppetti
  Course     : CSCI 476
  Description: Illustrate 'fork' and 'wait' to create processes and
               wait for them to terminate. 
*/

/************************************************************/
// System includes

#include <iostream>
#include <cstdlib>
#include <cassert>

#include <unistd.h>
#include <sys/wait.h>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

// DO NOT DO THIS!
using namespace std;

/************************************************************/
// Function prototypes/global vars/typedefs

int g_sharedUntilWritten = 1984;

/************************************************************/

int
main (int argc, char* argv[])
{
  pid_t childPid = fork ();
  // Fork result of -1 indicates failure
  assert (childPid != -1); 
  if (childPid == 0)
  {
    cout << "Child process:  pid = " << getpid ()
	 << "; ppid = " << getppid () << endl;
    cout << "Global var = " << g_sharedUntilWritten << endl;
    // "exit" will cause child process to terminate
    exit (5); 
  }
  cout << "Parent process: pid = " << getpid ()
       << "; child pid = " << childPid << endl;
  cout << "Global var = " << g_sharedUntilWritten << endl;
    
  // Wait for any child to exit
  int childStatus;
  childPid = waitpid (-1, &childStatus, 0);
  cout << "Finished waiting for child pid = " << childPid
       << "; exit status = " << WEXITSTATUS (childStatus) << endl;
    
  return EXIT_SUCCESS;
}

/************************************************************/
/************************************************************/
