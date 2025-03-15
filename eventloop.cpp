#include "eventloop.h"
#include "exceptionhandler.h"

class eventloopP
{
public:
    SafeQueue<ICommand *> *cmds;
    StateStatus *status;
    eventloopP(SafeQueue<ICommand *> *cmds, StateStatus *status) :
        cmds(cmds),
        status(status)
    {
    }
};

eventloop::eventloop(SafeQueue<ICommand *> *cmds, StateStatus *status) :
    imp(new eventloopP(cmds, status))
{
}

eventloop::~eventloop() { delete imp; }

void eventloop::start(SafeQueue<ICommand *> *cmds, StateStatus *status)
{
    //std::cout << "__________" << std::endl;
    //std::cout << "I am here!" << std::endl;
    bool stop = false;
    int ic = 0;
    SafeQueue<ICommand*> cmds_1;
    std::exception ex;
    ExceptionHandler* handler = new ExceptionHandler(0, ex);

    std::thread t1(
                [&cmds, &stop, &ic, &ex, &handler, &status, &cmds_1](){
        try {
                while(!stop)
                {
                    ic += 1;
                    ICommand* cmd = cmds->front();
//                    std::cout << "Start execute ";
                    cmd->execute();
                    cmds->pop();
                    if(cmd->get_Id_cmd() == CommandHardStop)
                    {
                        stop = true;
                        std::cout << "in queue after HardStop : " << std::endl;
                        while(!cmds->empty())
                        {
                            ICommand* cmdh = cmds->front();
                            cmdh->execute();
                            cmds->pop();
                        }
                    }
                    if(cmd->get_Id_cmd() == CommandSoftStop)
                    {
                        stop = true;
                        while(!cmds->empty())
                        {
                            std::cout << cmds->size() << " -> " << cmds_1.size() << std::endl;
                            ICommand* cmdss = cmds->front();
                            cmds_1.push(cmdss);
                            cmds->pop();
                        }
                        while(!cmds_1.empty())
                        {
                            ICommand* cmds = cmds_1.front();
                            cmds->execute();
                            cmds_1.pop();
                        }
                    }
                    if(cmds->empty())
                        stop = true;
                }
        } catch( std::exception ex) {
            handler->executeRepeat(handler, cmds, ex);
        }
    });
    t1.join();
}

void eventloop::execute(ICommand *cmd)
{
//    std::cout << "Start execute ";
    cmd->execute();
    std::cout << std::endl;
}
