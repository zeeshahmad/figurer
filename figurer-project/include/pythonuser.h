#ifndef PYTHONUSER_H
#define PYTHONUSER_H


#include "pycode.h"

class PythonUser
{
public:
    PythonUser(pycode::Worker * pythonWorker)
        :pythonWorker{pythonWorker} {}
    pycode::Worker * pythonWorker;
};

#endif // PYTHONUSER_H
