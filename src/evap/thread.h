#ifndef THREAD_H
#define THREAD_H

#include <QThread>

template <typename TWorker>
class Thread : public QThread
{
public:
    explicit Thread(TWorker *worker, QObject *parent = 0)
        : QThread(parent), _worker(worker)
    {
        _worker->moveToThread(this);
        start();
    }

    ~Thread() {
        quit();
        wait();
    }

    TWorker* worker() const {
        return _worker;
    }

protected:
    void run() override {
        QThread::run();
        _worker->deleteLater();
    }

private:
    TWorker *_worker;
};


#endif // THREAD_H
