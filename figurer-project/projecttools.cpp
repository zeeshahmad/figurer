#include "projecttools.h"


ProjectTools::ProjectTools(ProjectFileIO *io, ExternalFileScanner *scanner, QObject *parent)
    :QObject{parent}, io{io}, scanner{scanner}
{

}
