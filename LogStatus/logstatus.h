#pragma once

#ifndef LOG_STATUS_H_
#define LOG_STATUS_H_

#include <iostream>
#include <string>

class log_status {
    private:
        std :: string area = "";
    public:
        log_status();
        ~log_status();
        log_status(const std :: string area);
        void status(const std :: string message);
};

#endif // LOG_STATUS_H_
