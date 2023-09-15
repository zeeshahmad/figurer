#ifndef PYTHONUSER_H
#define PYTHONUSER_H


#include "pythonthread.h"

class PythonUser
{
public:
    PythonUser(PythonThread* pythonThread);
    PythonThread* pythonThread;
};

#endif // PYTHONUSER_H
