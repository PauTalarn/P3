/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Estimator 

Usage:
    get_pitch [options] <input-wav> <output-txt>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -m FLOAT, --th_rmaxnorm=FLOAT  umbral de la autocorrelación a largo plazo [default: 0.4]
    -p FLOAT, --llindarPos=FLOAT  umbral positivo central clipping [default: 0.01]
    -n FLOAT, --llindarNeg=FLOAT  umbral negativo central clipping [default: -0.01]
    -u FLOAT, --th_r1norm=FLOAT  umbral unvoiced [default: 0.9]
    -w FLOAT, --th_pot=FLOAT  umbral potencia [default: -80.5]
    -h, --help  Show this screen
    --version   Show the version of the project

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the estimation:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.

  /**
   \DONE :the user 
  */
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();
  float llindarPos=stof(args["--llindarPos"].asString());
  float llindarNeg=stof(args["--llindarNeg"].asString());

  float th_rmaxnorm=stof(args["--th_rmaxnorm"].asString());
  //float th_rmaxnorm=0.7;
  
  float th_r1norm=stof(args["--th_r1norm"].asString());
  //float th_r1norm=0.7;

  float th_pot=stof(args["--th_pot"].asString());
  //float th_pot=-43.5;

  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, th_rmaxnorm, th_r1norm, th_pot,  PitchAnalyzer::HAMMING, 50, 500);

  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  

  /**
   \DONE central-clipping implemented
  */

 for(unsigned int k=0; k<x.size();k++){
    if(x[k]>0){
      x[k]=x[k]-llindarPos;
      if(x[k]<0){
        x[k]=0;
      }
    } else {
      x[k]=x[k]-llindarNeg;
      if(x[k]>0){
        x[k]=0;
      }
    }
  }

  // Iterate for each frame and save values in f0 vector
  vector<float>::iterator iX;
  vector<float> f0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    float f = analyzer(iX, iX + n_len);
    f0.push_back(f);
  }

  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.

vector<float> vect_Med;     // se declara un vector vacío de tipo float llamado fMediana
vector<float> vect_f0;          // se declara un vector vacío de tipo float llamado  vect_f0
vect_f0.push_back(f0[0]);       // se agrega el primer elemento del vector f0 al vector  vect_f0
for (unsigned int l=1; l<f0.size()-1; l++){   // se itera desde el segundo elemento de f0 hasta el penúltimo elemento de f0
    for(int r=-1; r<2; r++){    // se itera sobre los vecinos del elemento actual de f0
      vect_Med.push_back(f0[l+r]);   // se agrega el elemento actual de f0 y sus vecinos al vector fMediana
    }
    sort(vect_Med.begin(),vect_Med.end());   // se ordena el vector fMediana en orden ascendente
    vect_f0.push_back(vect_Med[1]);    // se agrega el elemento central del vector fMediana al vector  vect_f0 como la mediana de los tres elementos
    vect_Med.clear();    // se borran los elementos de fMediana para la siguiente iteración
  }
vect_f0.push_back(f0[f0.size()-1]);  // se agrega el último elemento de f0 al vector  vect_f0




  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = vect_f0.begin(); iX != vect_f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}



//casa