#ifndef FEATURES_H_
#define FEATURES_H_
#include "base/kaldi-common.h"
#include "feat/feature-fbank.h"
#include "feat/wave-reader.h"
#include "util/common-utils.h"

class FeatComputer
{
public:
    FeatComputer();
    void compute_fbank(std::string wavfile, kaldi::Matrix<kaldi::BaseFloat> *Output);
    void wav2fbank(std::string wavfile, std::string dstfile);
    void compute_signals(kaldi::SubVector<kaldi::BaseFloat> signals,
                                       kaldi::Matrix<kaldi::BaseFloat> *Output,
                                       kaldi::BaseFloat sample_rate);
    ~FeatComputer()
    {
        if (fbank_computer)
        {
            delete fbank_computer;
            fbank_computer = nullptr;
        }
    }

private:
    kaldi::FbankOptions fbank_opts;
    kaldi::Fbank *fbank_computer;
    bool subtract_mean = false;
};

#endif
