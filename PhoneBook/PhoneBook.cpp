#include "Controller.h"
#include "DisplayView.h"

int main()
{
    DisplayView displayView;
    CommandView commandView;

    // Get unique instance of Repository
    Repository* repository = Repository::GetInstance();

    // DisplayView subscribes to Repository
    repository->Subscribe(&displayView);

    // Get unique instance of Controller
    Controller* controller = Controller::GetInstance();

    // Start running controller
    controller->Run(repository, &commandView);
}

