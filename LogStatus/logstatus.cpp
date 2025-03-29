#include "logstatus.h"

log_status :: log_status()
{

}

log_status :: ~log_status()
{

}

log_status :: log_status(const std :: string area)
{
    this -> area = area;
}

void log_status :: status(const std :: string message)
{
    std :: cout << area << ": " << message << '\n';
}
