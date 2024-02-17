#include "Test.h"
#include "Pattern.h"

#include "dsp/distance_functions_f16.h"

class DistanceTestsF16:public Client::Suite
    {
        public:
            DistanceTestsF16(Testing::testID_t id);
            virtual void setUp(Testing::testID_t,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr);
            virtual void tearDown(Testing::testID_t,Client::PatternMgr *mgr);
        private:
            #include "DistanceTestsF16_decl.h"
            
            Client::Pattern<float16_t> inputA;
            Client::Pattern<float16_t> inputB;
            Client::Pattern<int16_t> dims;

            Client::LocalPattern<float16_t> output;
            Client::LocalPattern<float16_t> tmpA;
            Client::LocalPattern<float16_t> tmpB;

            // Reference patterns are not loaded when we are in dump mode
            Client::RefPattern<float16_t> ref;

            int vecDim;
            int nbPatterns;


    };
