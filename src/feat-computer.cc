#include "feat-computer.h"

FeatComputer::FeatComputer()
{
    fbank_opts.use_energy = false;
    fbank_opts.mel_opts.low_freq = 20;
    fbank_opts.mel_opts.high_freq = 8000;
    fbank_opts.mel_opts.num_bins = 80;
    fbank_opts.frame_opts.samp_freq = 16000;
    fbank_opts.frame_opts.frame_shift_ms = 10.0;
    fbank_opts.frame_opts.frame_length_ms = 25.0;
    fbank_opts.frame_opts.preemph_coeff = 0.97;
    fbank_opts.frame_opts.window_type = "hamming";
    fbank_opts.frame_opts.remove_dc_offset = true;
    fbank_opts.frame_opts.dither = 0.0;

    fbank_computer = new kaldi::Fbank(fbank_opts);
}
void FeatComputer::compute_fbank(std::string wavfile, kaldi::Matrix<kaldi::BaseFloat> *Output)
{
    if (fbank_computer == nullptr)
    {
        KALDI_ERR << "fbank_computer is nullptr";
        return;
    }
    kaldi::Input wavKi;
    bool binary = true;
    wavKi.Open(wavfile, &binary);
    std::istream &wavis = wavKi.Stream();

    kaldi::WaveData waveData;
    waveData.Read(wavis);
    kaldi::SubVector<kaldi::BaseFloat> waveform(waveData.Data(), 0); //-1:expect mono
    try
    {
        fbank_computer->ComputeFeatures(waveform, waveData.SampFreq(), 1.0, Output);
    }
    catch (...)
    {
        KALDI_WARN << "Failed to compute features for wavfile: " << wavfile;
    }
}

void FeatComputer::wav2fbank(std::string wavfile, std::string dstfile)
{
    if (fbank_computer == nullptr)
    {
        KALDI_ERR << "fbank_computer is nullptr";
        return;
    }
    kaldi::Matrix<kaldi::BaseFloat> features;
    compute_fbank(wavfile, &features);
    KALDI_WARN << "feat_rows:" << features.NumRows() << ",cols:" << features.NumCols();
    if (subtract_mean)
    {
        kaldi::Vector<kaldi::BaseFloat> mean(features.NumCols());
        mean.AddRowSumMat(1.0, features);
        mean.Scale(1.0 / features.NumRows());
        for (kaldi::int32 i = 0; i < features.NumRows(); i++)
            features.Row(i).AddVec(-1.0, mean);
    }
    kaldi::BaseFloatMatrixWriter kaldi_writer;
    std::ostringstream dststr;
    dststr << "ark,t:" << dstfile;
    if (!kaldi_writer.Open(dststr.str()))
    {
        KALDI_ERR << "Could not initialize output with wspecifier ";
    }
    kaldi_writer.Write(wavfile, features);
}

void FeatComputer::compute_signals(kaldi::SubVector<kaldi::BaseFloat> signals,
                                   kaldi::Matrix<kaldi::BaseFloat> *output,
                                   kaldi::BaseFloat sample_rate)
{
    if (fbank_computer == nullptr)
    {
        KALDI_ERR << "fbank_computer is nullptr";
        return;
    }

    try
    {
        fbank_computer->ComputeFeatures(signals, sample_rate, 1.0, output);
        if (subtract_mean)
        {
            kaldi::Vector<kaldi::BaseFloat> mean(output->NumCols());
            mean.AddRowSumMat(1.0, *output);
            mean.Scale(1.0 / output->NumRows());
            for (kaldi::int32 i = 0; i < output->NumRows(); i++)
                output->Row(i).AddVec(-1.0, mean);
        }
    }
    catch (...)
    {
        KALDI_WARN << "Failed to compute features for signals";
    }
}