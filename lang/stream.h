
#ifndef REPHP_LANG_STREAM_H
#define REPHP_LANG_STREAM_H

#include "../engine/engine.h"

namespace rephp {

namespace lang {

using namespace std;
using namespace rephp::engine;

template<typename T1, typename T2>
class scalar_worker_add: public worker
{
public:
    bool operator() (shared_ptr<internal_value> self, vector<shared_ptr<internal_value>> parameters, shared_ptr<internal_value> return_value);
};

class stream_bundle: public bundle
{
public:
    stream_bundle();
};

};

};

#endif