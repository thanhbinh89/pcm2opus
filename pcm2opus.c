#include <stdio.h>
#include <stdlib.h>
#include <opus/opus.h>

#define CHANNELS 1
#define SAMPLE_SIZE 960 // 8k, 120sps
#define SAMPLE_RATE 8000
#define MAX_PACKET_SIZE (3 * 1276)

int main(int argc, char *argv[])
{

  int err;
  int encoded_bytes;
  short pcm_buff[SAMPLE_SIZE * CHANNELS];

  OpusEncoder *encoder;
  unsigned char cbits[MAX_PACKET_SIZE];

  // fs: sampling rate, one of 8000, 12000, 16000, 24000, 48000
  // channels: number of channels
  // application: encoding mode, there are three types:
  // OPUS_APPLICATION_VOIP: Processes voice signals, suitable for voip business scenarios
  // OPUS_APPLICATION_AUDIO: This mode is suitable for non-voice content such as music
  // OPUS_APPLICATION_RESTRICTED_LOWDELAY: low latency mode
  // error: Encoded return value

  encoder = opus_encoder_create(SAMPLE_RATE, CHANNELS, OPUS_APPLICATION_AUDIO, &err);
  if (err < 0)
  {
    fprintf(stderr, "failed to create an encoder: %s\n", opus_strerror(err));
    return -1;
  }
  printf("opus_encoder_create ok\n");

  opus_encoder_ctl(encoder, OPUS_SET_VBR(0)); // 0:CBR, 1:VBR
  opus_encoder_ctl(encoder, OPUS_SET_VBR_CONSTRAINT(1));
  opus_encoder_ctl(encoder, OPUS_SET_BITRATE(64000));
  opus_encoder_ctl(encoder, OPUS_SET_COMPLEXITY(8)); // 8    0~10
  opus_encoder_ctl(encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
  opus_encoder_ctl(encoder, OPUS_SET_LSB_DEPTH(16)); // 每个采样16个bit，2个byte
  opus_encoder_ctl(encoder, OPUS_SET_DTX(0));
  err = opus_encoder_ctl(encoder, OPUS_SET_INBAND_FEC(0));
  if (err < 0)
  {
    fprintf(stderr, "failed to set bitrate: %s\n", opus_strerror(err));
    return -1;
  }
  printf("opus_set_bitrate ok\n");

  // sample rate 8000, 20ms -> 8000/(1000ms/20ms) = 160 frames
  // sample rate 8000, 120ms -> 8000/(1000ms/120ms) = 960 frames
  encoded_bytes = opus_encode(encoder, (opus_int16 *)pcm_buff, SAMPLE_SIZE, cbits, sizeof(cbits));
  if (encoded_bytes < 0)
  {
    fprintf(stderr, "encode failed: %s\n", opus_strerror(encoded_bytes));
    return -1;
  }
  printf("opus_encode return %d bytes\n", encoded_bytes);

  opus_encoder_destroy(encoder);
  return 0;
}
