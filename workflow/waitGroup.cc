#include <signal.h>
#include <workflow/WFFacilities.h>

static WFFacilities::WaitGroup waitGroup(1);

void sigHandler(int num){
    waitGroup.done();
    fprintf(stderr, "wait group is done\n");
}

int main(){
    signal(SIGINT, sigHandler);
    waitGroup.wait();
}
