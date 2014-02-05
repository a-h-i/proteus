#include "prot.hpp"
#include "recorder/microphone.hpp"
#include <chrono>
Prot::Prot( const std::string &grammar, const std::string &dict ) : mic(this), m() {
    // configure sphinx
    cmd_ln_t *config = cmd_ln_init( NULL, ps_args(), TRUE,
                                    "-fsg", grammar.c_str(),
                                    "-dict", dict.c_str(),
                                    NULL );
    ps = ps_init( config );
}

const char *Prot::recogWord() {
    mic.start();
    std::unique_lock<std::mutex> lk( m );
    char const *hyp;
    char const *uttid;
    ps_start_utt(ps, NULL);
    bool first = true;
    auto start = std::chrono::system_clock::now();
    while(true) {
        cv.wait( lk );
        std::list<std::vector<std::int16_t> *> data;
        {
            std::lock_guard<std::mutex> lock(bufferM);
            data.splice(data.end(), frames);

        }
        


    for(auto buff : data) {    
        ps_process_raw(ps, &(*buff)[0], buff->size(), FALSE, FALSE);
        delete buff;
        if(first) {
            start = std::chrono::system_clock::now();
            first = false;
        }
        auto end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        if(elapsed.count() >= 1200) {
             ps_end_utt(ps);
             hyp = ps_get_hyp(ps, NULL, &uttid);
             mic.stop();
             return hyp;
        }
    }

    }  
}
void Prot::notify() {
    if(mic.isRecording()) {
        cv.notify_one();
    }
}

void Prot::buffer(std::vector<std::int16_t> *buff) {
    std::lock_guard<std::mutex> lock(bufferM);
    frames.push_back(buff);
}