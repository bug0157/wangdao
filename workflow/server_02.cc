#include <signal.h>
#include <workflow/WFFacilities.h>

static WFFacilities::WaitGroup waitGroup(1);
struct SeriesContext{
    WFHttpTask *serverTask;
    std::string name;
    std::string key;
};

void sigHandler(int num){
    waitGroup.done();
    fprintf(stderr, "wait group is done\n");
}

void redisCallback(WFRedisTask *redisTask){
    protocol::RedisRequest *req = redisTask->get_req();
    protocol::RedisResponse *resp = redisTask->get_resp();
    int state = redisTask->get_state();
    int error = redisTask->get_error();
    protocol::RedisValue value;//value对象专门用来村塾redis任务的结果
    switch (state) {
        case WFT_STATE_SYS_ERROR:
            fprintf(stderr, "system error:%s\n", strerror(error));
            break;
        case WFT_STATE_DNS_ERROR:
            fprintf(stderr, "system error:%s\n", strerror(error));
            break;
        case WFT_STATE_SUCCESS:
            break;
    }
    
    if (state != WFT_STATE_SUCCESS) {
        fprintf(stderr, "Failed\n");
        return;
    }else {
        fprintf(stderr, "Success!\n");
    }
    SeriesContext *context = static_cast<SeriesContext *>(series_of(redisTask)->get_context());
    std::string name = context->name;
    std::string key = context->key;
    WFHttpTask *serverTask = context->serverTask;
    if (value.is_nil()) {
        auto resp2client = serverTask->get_resp();
        resp2client->add_header_pair("Content-Type", "text/plain");
        resp2client->append_output_body("you are not login yet!");
    }
    else {
        std::string storedKey = value.string_value();
    }
}

int main(){
    signal(SIGINT, sigHandler);
    waitGroup.wait();
}
