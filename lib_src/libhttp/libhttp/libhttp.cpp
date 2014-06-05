// libhttp.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "libhttp.h"
#include "server.hpp"
#include "utils.h"


namespace http{
    namespace server2{

        class httpserver : public boost::noncopyable
        {
        public:
            void reset(const std::string& addr, const std::string& port, 
                                 const std::string& doc_root, std::size_t num_threads,
								 FUN_DO_API fun, void* p)
            {
                _run?(stop(), _run = false):0;
                s.reset(new server(addr, port, doc_root, num_threads, fun, p));
            }
            void run()
            {
                m_thread.reset(new boost::thread(boost::bind(&httpserver::s_run, this)));
                _run = true;
            }
            void stop()
            {
                m_thread->interrupt();
                _run = false;
            }

            httpserver()
                : _run(false)
            {
            }
        private:
            bool _run;
            boost::scoped_ptr<server> s;
            boost::scoped_ptr<boost::thread> m_thread;
            void s_run()
            {
                s->run();
            }

        };
    }
}

static http::server2::httpserver hs;

LIBHTTP_API int http_run(const std::string& port, const unsigned num_threads, const std::string& doc_root, FUN_DO_API fun, void* p)
{
	printf("http listen: 0.0.0.0:%s\ndoc_root: %s\n", port.c_str(), doc_root.c_str());
	hs.reset("0.0.0.0", port, doc_root, num_threads, fun, p);
    hs.run();
	return 0;
}

LIBHTTP_API int http_stop()
{
    hs.stop();
    return 0;
}

LIBHTTP_API const char* http_current_dir()
{
	return current_dir().c_str();
}