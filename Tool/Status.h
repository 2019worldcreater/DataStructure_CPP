//
// Created by HP on 2020-11-1.
//

#ifndef CLIONCPP_STATUS_H
#define CLIONCPP_STATUS_H
namespace StatusTool {
    enum Status {
        ERROR, OK
    };

    bool isStatusOK(Status status);
}
#endif //CLIONCPP_STATUS_H
