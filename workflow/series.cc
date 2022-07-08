#include <signal.h>
#include <workflow/WFFacilities.h>

static WFFacilities::WaitGroup waitGroup(1);

void sigHandler(int num){
    waitGroup.done();
    fprintf(stderr, "wait group is done\n");
}

void callback(WFRedisTask *redisTask){
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
    
    std::string cmd;
    req->get_command(cmd);
    if (cmd == "SET") {
        //firstTask的基本工作做完了
        //创建新任务，把新任务加入到本序列的末尾
        fprintf(stderr, "first task callback begins\n");
        WFRedisTask *secondTask = WFTaskFactory::create_redis_task("redis://127.0.0.1:6379", 0, callback);
        protocol::RedisRequest *req = secondTask->get_req();
        req->set_request("GET", {"43key"});
        SeriesWork *series = series_of(redisTask);
        series->push_back(secondTask);
        sleep(10);
        fprintf(stderr, "first task callback ends\n");
    }
    else {
        //secondTask的基本工作完成了
        fprintf(stderr, "second task callback ends\n");
        fprintf(stderr, "redis request, cmd = %s\n", value.string_value().c_str());
        if (value.is_string()) {
            fprintf(stderr, "value is a string, value = %s\n", value.string_value().c_str());
        }
        else if (value.is_array()) {
            fprintf(stderr, "value is string array\n");
            for (size_t i = 0; i < value.arr_size(); ++i) {
                fprintf(stderr, "value at %lu = %s\n", i, value.arr_at(i).string_value().c_str());
            }
        }
    }
    fprintf(stderr, "second task callback ends\n");
}


int main(){
    signal(SIGINT, sigHandler);
    //创建redis任务
    WFRedisTask *redisTask = WFTaskFactory::create_redis_task("redis://127.0.0.1:6379", 0, callback);
    //设置redis任务的属性
    protocol::RedisRequest *req = redisTask->get_req();
    //req->set_request("SET", {"43key", "100"});
    //req->set_request("GET", {"43key", "100"});
    req->set_request("SET", {"43t"});
    //启动redis任务
    redisTask->start();
    waitGroup.wait();
}
