/// @file

#include <iostream>
#include <math.h>
#include "pitch_analyzer.h"

using namespace std;

/// Name space of UPC
namespace upc
{
  void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const
  {

    for (unsigned int l = 0; l < r.size(); ++l)
    {
      /// \TODO Compute the autocorrelation r[l]
      /**
      \DONE autocorrelation computed
      Calculates the autocorrelation function of a signal x and stores the result in a vector r.
      The autocorrelation function is a measure of the similarity between a signal and a time-delayed version of itself.
      Calculates the cross-product of the signal with itself at different time delays (or lags) and normalizes it by the length of the signal.
      Resulting autocorrelation vector r contains values that represent the degree of similarity between the signal and its delayed version at different time lags.
      Includes a check to ensure that the first element of the autocorrelation vector is not zero, to avoid divide-by-zero errors when performing subsequent calculations involving the autocorrelation function.
      */
      r[l] = 0;
      for (unsigned int n = l; n < x.size(); n++)
      {
        r[l] += x[n] * x[n - l];
      }
      r[l] /= x.size();
    }

    if (r[0] == 0.0F)
      r[0] = 1e-12;
  }

  void PitchAnalyzer::set_window(Window win_type)
  {
    if (frameLen == 0)
      return;

    window.resize(frameLen);

    switch (win_type)
    {
    case HAMMING:
      for (unsigned int n = 0; n < frameLen; n++)
      {
        window[n] = 0.53836 - (0.46164 * (cos((2 * 3.1416 * n) / (frameLen - 1))));
      }

      break;
    case RECT:
    default:
      window.assign(frameLen, 1);
    }
  }

  void PitchAnalyzer::set_f0_range(float min_F0, float max_F0)
  {
    npitch_min = (unsigned int)samplingFreq / max_F0;
    if (npitch_min < 2)
      npitch_min = 2; // samplingFreq/2

    npitch_max = 1 + (unsigned int)samplingFreq / min_F0;

    // frameLen should include at least 2*T0
    if (npitch_max > frameLen / 2)
      npitch_max = frameLen / 2;
  }

  bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm) const
  {
    /// \TODO Implement a rule to decide whether the sound is voiced or not.
    /// * You can use the standard features (pot, r1norm, rmaxnorm),
    ///   or compute and use other ones.

    /**
    \DONE rule implemented
    Returns a boolean value. It checks if two variables are both greater than the threshold values
    th_rmaxnorm and th_r1norm.
    If they are, the function returns "false". If not, the function returns "true".
    */
    //&& pot > th_pot
    if (rmaxnorm > th_rmaxnorm && r1norm > th_r1norm && pot > th_pot)
    {
      return false;
    }

    return true;
  }

  float PitchAnalyzer::compute_pitch(vector<float> &x) const
  {
    if (x.size() != frameLen)
      return -1.0F;

    // Window input frame
    for (unsigned int i = 0; i < x.size(); ++i)
      x[i] *= window[i];

    vector<float> r(npitch_max);

    // Compute correlation
    autocorrelation(x, r);

    vector<float>::const_iterator iR = r.begin(), iRMax = iR;

    /// \TODO
    /// Find the lag of the maximum value of the autocorrelation away from the origin.<br>
    /// Choices to set the minimum value of the lag are:
    ///    - The first negative value of the autocorrelation.
    ///    - The lag corresponding to the maximum value of the pitch.
    ///	   .
    /// In either case, the lag should not exceed that of the minimum value of the pitch.

    /**
      \DONE rule implemented
      The code calculates the lag of the maximum value of the autocorrelation function of a signal.
      It begins by initializing two constant iterators, iR and iRMax, to the beginning of the vector r.
      The for loop iterates through the elements of r from the index npitch_min to npitch_max - 1.
      For each iteration, it compares the current element with the value pointed by iRMax.
      If the current element is greater than iRMax, then iRMax is updated to point to the current element.
      At the end of the loop, the variable lag is calculated as the difference between iRMax and the beginning of r.
      Finally, the logarithm base 10 of the first element of the r vector is computed and stored in the variable pot.
      This value can be used to identify whether the segment of speech is voiced or unvoiced.
      */
    for (iR = iRMax = r.begin() + npitch_min; iR < r.begin() + npitch_max; iR++)
    {
      if (*iR > *iRMax)
      {
        iRMax = iR;
      }
    }
    unsigned int lag = iRMax - r.begin();

    float pot = 10 * log10(r[0]);

    // You can print these (and other) features, look at them using wavesurfer
    // Based on that, implement a rule for unvoiced
    // change to #if 1 and compile

#if 0
    if (r[0] > 0.0F)
    cout << pot << '\t' << r[1]/r[0] << '\t' << r[lag]/r[0] << endl;
#endif

    static int cont = 0;

    if (unvoiced(pot, r[1] / r[0], r[lag] / r[0]))
      return 0;
    else
    {

      // We save the waveform and its autocorrelation of the first SOUND frame.
      // to do de graphics

      if (cont == 4)
      {
        FILE *file_voiced_x = fopen("voiced_x.txt", "w+");
        FILE *file_voiced_r = fopen("voiced_r.txt", "w+");

        for (unsigned int i = 0; i < x.size(); i++)
        {
          fprintf(file_voiced_x, "%f \n", x[i]);
          fprintf(file_voiced_r, "%f \n", r[i]);
        }

        fclose(file_voiced_x);
        fclose(file_voiced_r);
      }
      cont++;
      return (float)samplingFreq / (float)lag;
    }
  }
}
