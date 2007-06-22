//===========================================================================
// Real Discrete Fourier Transform
//     dimension   :one
//     data length :power of 2, must be larger than 4
//     decimation  :frequency
//     radix       :4, 2
//     data        :inplace
// classes:
//	 Cfft: real discrete fourier transform class
// functions:
//	 Cfft::rdft : compute the forward real discrete fourier transform
//   Cfft::cdft : compute the forward complex discrete fourier transform
//   Cfft::fft  : compute the forward real dft on a set of integer values
//	
//	 This class is derived from the work of Takuya Ooura, who has kindly put his
//	 fft algorithims in the public domain.  Thank you Takuya Ooura!
//===========================================================================

#ifndef FFT_H
#define FFT_H

#include <math.h>
#include <stdio.h>
#include "complex.h"
#include "misc.h"

enum fftPrefilter {FFT_NONE, FFT_HAMMING, FFT_HANNING, FFT_BLACKMAN, FFT_TRIANGULAR};


class Cfft {
private:
	double xi;
	double *w;
	int  *ip;
	double *fftwin;
	fftPrefilter wintype;
	int  fftlen;
	int  fftsiz;
    void makewt();
    void makect();
    void bitrv2(int n, int *ip, double *a);
	void bitrv2conj(int n, int *ip, double *a);
    void cftfsub(int n, double *a);
	void cftbsub(int n, double *a);
	void cftmdl(int n, int l, double *a);
	void cft1st(int n, double *a);
	void rftfsub(int n, double *a);
	void rftbsub(int n, double *a);
	
// Rectangular - no pre filtering of data array
	void RectWindow(double *array, int n) {
		for (int i = 0; i < n; i++)
			array[i] = 1.0;
	}

// Hamming - used by gmfsk
	void HammingWindow(double *array, int n) {
		double pwr = 0.0;
		for (int i = 0; i < n; i++) {
			array[i] = hamming((double)i/(double)n);
			pwr += array[i] * array[i];
		}
		pwr = sqrt((double)n/pwr);
		for (int i = 0; i < n; i++)
			array[i] *= pwr;
	}

// Hanning - used by winpsk
	void HanningWindow(double *array, int n) {
		double pwr = 0.0;
		for (int i = 0; i < n; i++) {
			array[i] = hanning((double)i/(double)n);
			pwr += array[i] * array[i];
		}
		pwr = sqrt((double)n/pwr);
		for (int i = 0; i < n; i++)
			array[i] *= pwr;
	}

// Best lob suppression - least in band ripple
	void BlackmanWindow(double *array, int n) {
		double pwr = 0.0;
		for (int i = 0; i < n; i++) {
			array[i] = blackman((double)i/(double)n);
			pwr += array[i] * array[i];
		}
		pwr = sqrt((double)n/pwr);
		for (int i = 0; i < n; i++)
			array[i] *= pwr;
	}

// Simple about effective as Hamming or Hanning
	void TriangularWindow(double *array, int n) {
		double pwr = 0.0;
		for (int i = 0; i < n; i++) array[i] = 1.0;
		for (int i = 0; i < n / 4; i++) {
				array[i] = 4.0 * (double)i / (double)n;
				array[n-i] = array[i];
		}
		for (int i = 0; i < n; i++)	pwr += array[i] * array[i];
		pwr = sqrt((double)n/pwr);
		for (int i = 0; i < n; i++)
			array[i] *= pwr;
	}
	
public:
	Cfft(int n);
	~Cfft();
	void resize(int n);
	void cdft(double *a);
	void cdft(complex *a) { cdft( (double *) a); }
	void icdft(double *a);
	void icdft(complex *a) { icdft( (double *) a); }
	void sifft(short int *siData, double *out);
	void sifft(short int *siData, complex *a) { sifft(siData, (double *) a); }
	void rdft(double *a);
	void rdft(complex *a) { rdft( (double *) a); }
	void irdft(double *a);
	void irdft(complex *a) { irdft( (double *) a); }
	
	void setWindow(fftPrefilter pf) {
		wintype = pf;
		if (wintype == FFT_TRIANGULAR)
			TriangularWindow(fftwin, fftlen*2);
		else if (wintype == FFT_HAMMING)
			HammingWindow(fftwin, fftlen*2);
		else if (wintype == FFT_HANNING)
			HanningWindow(fftwin, fftlen*2);
		else if (wintype == FFT_BLACKMAN)
			BlackmanWindow(fftwin, fftlen*2);
		else
			RectWindow(fftwin, fftlen*2);
	}
};

#endif
