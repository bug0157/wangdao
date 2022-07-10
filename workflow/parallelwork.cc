#include <signal.h>
#include <workflow/WFFacilities.h>

static WFFacilities::WaitGroup waitGroup(1);
struct SeriesContext{
    std::string url;

};

void sigHandler(int num){
    waitGroup.done();
    fprintf(stderr, "wait group is done\n");
}

void parallelCallback(const ParallelWork *pwork){
    fprintf(stderr, "pwork callback\n");
}
void httpCallback(WFHttpTask *httpTask){
    SeriesContext *context = static_cast<SeriesContext *>(series_of(httpTask)->get_context());
    fprintf(stderr, "httpTask callback, url = %s\n", context->url.c_str());
}

int main(){
    signal(SIGINT, sigHandler);
    ParallelWork *pwork = Workflow::create_parallel_work(parallelCallback);

    std::vector<std::string> urlVec = {"http://www.baidu.com", "http://www.jd.com", "http://www.taobao.com"};
    for (size_t i = 0; i != urlVec.size(); ++i) {
        std::string url = urlVec[i];
        auto httpTask = WFTaskFactory::create_http_task(url, 0, 0, httpCallback);

        //修改任务的属性
        auto req = httpTask->get_req();
        req->set_header_pair("Accept", );
        SeriesContext *context = new SeriesContext;
        context->url = std::move(url);
        auto series = Workflow::create_series_work(httpTask, nullptr);
        //把存储响应内容的指针，拷贝到序列的context当中
        series->set_context(context);

        pwork->add_series(series);
    }
    
    Workflow::start_series_work(pwork, nullptr);
    waitGroup.wait();
}
