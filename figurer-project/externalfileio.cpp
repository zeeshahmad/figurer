#include "externalfileio.h"
#include "iexternalfileparser.h"

ExternalFileIO::ExternalFileIO(IExternalFileParser& parser, QObject *parent)
    : QObject{parent}, parser{parser}
{

}
