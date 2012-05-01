#ifndef _QUEUE_H
#define _QUEUE_H

#include "Parameters.h"

class CQueue {
public:

  void GoPAnDA();             //Function called to interpret params and start PAnDA
  void SetParams(char *file); //Loads params file

protected:
private:

  Parameter params; //analysis parameters

};

#endif