#ifndef _LOG_PTHREADS_HH_
#define _LOG_PTHREADS_HH_

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <string>

namespace log {
    namespace threading {
        
        //return the thread ID
        std::string getThreadId();
    
        class Mutex {
            private:
                pthread_mutex_t mutex;
            
            public:
                inline Mutex()
                {
                    ::pthread_mutex_init(&mutex, NULL);
                }

                inline void lock()
                {
                    ::pthread_mutex_lock(&mutex);
                }

                inline void unlock()
                {
                    ::pthread_mutex_unlock(&mutex);
                }

                inline ~Mutex()
                {
                    ::pthread_mutex_destroy(&mutex);
                }

            private:
                Mutex(const Mutex& m);
                Mutex& operator=(const Mutex& m);

        };

        class ScopedLock {
            private:
                Mutex& _mutex;

            public:
                inline ScopedLock(Mutex& mutex)
                    : _mutex(mutex)
                {
                    _mutex.lock();
                }

                inline ~ScopedLock()
                {
                    _mutex.unlock();
                }
        };

        template<typename T>
        class ThreadLocalDataHolder {
            private:
                pthread_key_t _key;

            public:
                typedef T data_type;

                inline ThreadLocalDataHolder()
                {
                    ::pthread_key_create(&_key, freeHolder);
                }

                inline static void freeHolder(void* p)
                {
                    assert(p != nullptr);
                    delete  reinterpret_cast<T*>(p);
                }

                inline ~ThreadLocalDataHolder()
                {
                    T* data = get();
                    if(data != nullptr)
                    {
                        delete data;
                    }
                    ::pthread_key_delete(_key);
                }
                
                inline void reset(T* p = nullptr)
                {
                    T* data = get();
                    if(data != nullptr)
                    {
                        delete data;
                    }
                    ::pthread_setspecific(_key, p);
                }

                inline T* get() const
                {
                    return reinterpret_cast<T*>(::pthread_getspecific(_key));
                }

                inline T* operator->() const { return get(); }
                inline T& operator*() const {return *get(); }

                inline T* release() 
                {
                    T* result = get();
                    ::pthread_setspecific(_key, nullptr);
                    return result;
                }
        };
    }
}

#endif
