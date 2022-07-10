#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <workflow/WFFacilities.h>
#include <workflow/WFTask.h>

static WFFacilities::WaitGroup waitGroup(1);
struct FileData{
    char *buf;
    int fd;
};

void sigHandler(int num){
    waitGroup.done();
    fprintf(stderr, "wait group is done\n");
}

void callback(WFFileIOTask *IOTask){
    FileData * filedata = static_cast<FileData *>(IOTask->user_data);
    fprintf(stderr, "buf = %s\n", filedata->buf);
    free(filedata->buf);
    close(filedata->fd);
}

int main(){
    signal(SIGINT, sigHandler);
    int fd = open("file1", O_RDONLY);
    char *buf = new char[10];
    FileData *filedata = new FileData();
    filedata->buf = buf;
    filedata->fd = fd;
    WFFileIOTask *IOTask = WFTaskFactory::create_pread_task(fd, buf, 5, 0, callback);
    IOTask->user_data = filedata;
    IOTask->start();
    waitGroup.wait();
}
