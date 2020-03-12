#include "feat-computer.h"

int main(int argc, char *argv[])
{
    const char *usage = "Usage: compute-feats <wavfile> <dstfile>\n";
    using namespace kaldi;
    ParseOptions po(usage);
    po.Read(argc, argv);
    if (po.NumArgs() != 2)
    {
        po.PrintUsage();
        return 1;
    }
    std::string wavfile = po.GetArg(1), dstfile = po.GetArg(2);
    FeatComputer computer;
    computer.wav2fbank(wavfile, dstfile);

    return 0;
}
