#include "sound.h"
#include "configuration.h"

cSound::cSound(const char *dev ) {
	device			= dev;
	tx_src_state = 0;
	tx_src_data = 0;
	rx_src_state = 0;
	rx_src_data = 0;
	snd_buffer = 0;
	src_buffer = 0;
	txppm = progdefaults.TX_corr;
	rxppm = progdefaults.RX_corr;
	
	try {
		Open(O_RDONLY);
		getVersion();
		getCapabilities();
		getFormats();
		Close();
	}
	catch (SndException e) {
		std::cout << e.szError - 1 
			 << " <" << device.c_str()
			 << ">" << std::endl;
    }

	try {
		int err;
		snd_buffer	= new float[SND_BUF_LEN];
		src_buffer	= new float[SND_BUF_LEN];
		cbuff 		= new unsigned char [2 * SND_BUF_LEN];
		if (!snd_buffer || !src_buffer || !cbuff)
			throw("Cannot create src buffers");
		for (int i = 0; i < SND_BUF_LEN; i++)
			snd_buffer[i] = src_buffer[i] = 0.0;
		for (int i = 0; i < 2 * SND_BUF_LEN; i++)
			cbuff[i] = 0;

		tx_src_data = new SRC_DATA;
		rx_src_data = new SRC_DATA;
		if (!tx_src_data || !rx_src_data)
			throw("Cannot create source data structures");
		rx_src_state = src_new(SRC_SINC_FASTEST, 1, &err);
		if (rx_src_state == 0)
			throw(src_strerror(err));
		tx_src_data->src_ratio = 1.0;
		tx_src_state = src_new(SRC_SINC_FASTEST, 1, &err);
		if (rx_src_state == 0)
			throw(src_strerror(err));
		rx_src_data->src_ratio = 1.0/(1.0 + rxppm/1e6);
		src_set_ratio ( rx_src_state, 1.0/(1.0 + rxppm/1e6));
		
//		tx_src_data->src_ratio = 1.0/(1.0 + txppm/1e6);
		tx_src_data->src_ratio = 1.0 + txppm/1e6; // for gmfsk compatibility
//		src_set_ratio ( tx_src_state, 1.0/(1.0 + txppm/1e6));
		src_set_ratio ( tx_src_state, 1.0 + txppm/1e6);
	}
	catch (SndException){
		exit(1);
	};

}

cSound::~cSound()
{
	Close();
	if (cbuff) delete [] cbuff;
	if (snd_buffer) delete [] snd_buffer;
	if (src_buffer) delete [] src_buffer;
	if (tx_src_data) delete tx_src_data;
	if (rx_src_data) delete rx_src_data;
	if (rx_src_state) src_delete (rx_src_state);
	if (tx_src_state) src_delete (tx_src_state);
}

void cSound::setfragsize()
{
	int sndparam;
// Try to get ~100ms worth of samples per fragment
	sndparam = (int)log2(sample_frequency * 0.1);
// double since we are using 16 bit samples
	sndparam += 1;
// Unlimited amount of buffers for RX, four for TX
	if (mode == O_RDONLY)
		sndparam |= 0x7FFF0000;
	else
		sndparam |= 0x00040000;

	if (ioctl(device_fd, SNDCTL_DSP_SETFRAGMENT, &sndparam) < 0)
		throw errno;
}

int cSound::Open(int md, int freq)
{
	mode = md;
	try {
		device_fd = open(device.c_str(), mode, 0);
		if (device_fd == -1)
			throw SndException(errno);
		Format(AFMT_S16_LE);	// default: 16 bit little endian
		Channels(1);			//          1 channel
		Frequency(freq);
		setfragsize();
	} 
	catch (...) {
		throw;
	}
	return device_fd;
}

void cSound::Close()
{
	if (device_fd == -1)
		return;
	close(device_fd);
	device_fd = -1;
}

void cSound::getVersion()
{
	version = 0;
#ifndef __FreeBSD__
 	if (ioctl(device_fd, OSS_GETVERSION, &version) == -1) {
 		version = -1;
 		throw SndException("OSS Version");
 	}
#endif
}

void cSound::getCapabilities()
{
	capability_mask = 0;
	if (ioctl(device_fd, SNDCTL_DSP_GETCAPS, &capability_mask) == -1) {
		capability_mask = 0;
		throw SndException("OSS capabilities");
	}
}

void cSound::getFormats()
{
	format_mask = 0;
	if (ioctl(device_fd, SNDCTL_DSP_GETFMTS, &format_mask) == -1) {
		format_mask = 0;
		throw SndException("OSS formats");
	}
}

void cSound::Format(int format)
{
	play_format = format;
	if (ioctl(device_fd, SNDCTL_DSP_SETFMT, &play_format) == -1) {
		device_fd = -1;
		formatok = false;
		throw SndException("Unsupported snd card format");
    }
	formatok = true;
}

void cSound::Channels(int nuchannels)
{
	channels = nuchannels;
	if (ioctl(device_fd, SNDCTL_DSP_CHANNELS, &channels) == -1) {
		device_fd = -1;
		throw "Snd card channel request failed";
	}
}

void cSound::Frequency(int frequency)
{	
	sample_frequency = frequency;
	if (ioctl(device_fd, SNDCTL_DSP_SPEED, &sample_frequency) == -1) {
		device_fd = -1;
		throw SndException("Cannot set frequency");
    }
}

int cSound::BufferSize( int seconds )
{
	int bytes_per_channel = 0;
	switch (play_format) {
        case AFMT_MU_LAW:
        case AFMT_A_LAW:
        case AFMT_IMA_ADPCM:
            bytes_per_channel = 0; /* format not supported by this program */
			break;
        case AFMT_S16_BE:
        case AFMT_U16_LE:
        case AFMT_U16_BE:
        case AFMT_MPEG:
        case AFMT_S16_LE:
			bytes_per_channel = 2;
			break;
		case AFMT_U8:
        case AFMT_S8:
            bytes_per_channel = 1;
            break;
	}
  return seconds * sample_frequency * bytes_per_channel * channels;
}

bool cSound::wait_till_finished()
{
	if (ioctl(device_fd, SNDCTL_DSP_POST, 1) == -1 )
		return false;
	if (ioctl(device_fd, SNDCTL_DSP_SYNC, 0) == -1)
		return false; /* format (or ioctl()) not supported by device */
	return true; /* all sound has been played */
}

bool cSound::reset_device()
{
	if (ioctl(device_fd, SNDCTL_DSP_RESET, 0) == -1) {
		device_fd = -1;
		return false; /* format (or ioctl()) not supported by device */
    }
	return 1; /* sounddevice has been reset */
}

int cSound::Write(unsigned char *buffer, int buffersize)
{
	if (device_fd == -1)
		return -1;
	return write (device_fd, buffer, buffersize);
}

int cSound::Read(unsigned char *buffer, int buffersize)
{
	if (device_fd == -1)
		return -1;
	
	return read (device_fd, buffer, buffersize);
}

int cSound::Read(double *buffer, int buffersize)
{
	short int *ibuff = (short int *)cbuff;
	int numread;
	if (device_fd == -1)
		return -1;

	numread = Read(cbuff, buffersize * 2);
	for (int i = 0; i < buffersize; i++)
		src_buffer[i] = 
		buffer[i] = ibuff[i] / maxsc;

	if (rxppm != progdefaults.RX_corr) {
		rxppm = progdefaults.RX_corr;
		rx_src_data->src_ratio = 1.0/(1.0 + rxppm/1e6);
		src_set_ratio ( rx_src_state, 1.0/(1.0 + rxppm/1e6));
	}
	if (rxppm == 0)
		return buffersize;
	
// process using samplerate library
	rx_src_data->data_in = src_buffer;
	rx_src_data->input_frames = buffersize;
	rx_src_data->data_out = snd_buffer;
	rx_src_data->output_frames = SND_BUF_LEN;
	rx_src_data->end_of_input = 0;

	if ((numread = src_process(rx_src_state, rx_src_data)) != 0)
		return -1;

	numread = rx_src_data->output_frames_gen;

	for (int i = 0; i < numread; i++)
		buffer[i] = snd_buffer[i];
	
	return numread;
}

int cSound::write_samples(double *buf, int count)
{
	int retval;
	short int *wbuff;
	unsigned char *p;

	if (device_fd == -1 || count <= 0)
		return -1;

	if (txppm != progdefaults.TX_corr) {
		txppm = progdefaults.TX_corr;
//		tx_src_data->src_ratio = 1.0/(1.0 + txppm/1e6);
//		src_set_ratio ( tx_src_state, 1.0/(1.0 + txppm/1e6));
		tx_src_data->src_ratio = 1.0 + txppm/1e6;
		src_set_ratio ( tx_src_state, 1.0 + txppm/1e6);
	}
	
	if (txppm == 0) {
		wbuff = new short int[count];
		p = (unsigned char *)wbuff;
		for (int i = 0; i < count; i++) {
			wbuff[i] = (short int)(buf[i] * maxsc);
		}
		count *= sizeof(short int);
		retval = Write(p, count);
		delete [] wbuff;
	}
	else {
		float *inbuf;
		inbuf = new float[count];
		int bufsize;
		for (int i = 0; i < count; i++)
			inbuf[i] = buf[i];
		tx_src_data->data_in = inbuf;
		tx_src_data->input_frames = count;
		tx_src_data->data_out = src_buffer;
		tx_src_data->output_frames = SND_BUF_LEN;
		tx_src_data->end_of_input = 0;
		
		if (src_process(tx_src_state, tx_src_data) != 0) {
			delete [] inbuf;
			return -1;
		}
		delete [] inbuf;
		bufsize = tx_src_data->output_frames_gen;
		wbuff = new short int[bufsize];
		p = (unsigned char *)wbuff;
		
		for (int i = 0; i < bufsize; i++)
			wbuff[i] = (short int)(src_buffer[i] * maxsc);
		
		retval = Write(p, bufsize * 2);
		delete [] wbuff;
		if (retval != bufsize * 2)
			return -1;
		retval = count;
	}

	return retval;
}

