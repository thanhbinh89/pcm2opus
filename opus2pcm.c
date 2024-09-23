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
  int decoded_frame;
  short pcm_buff[SAMPLE_SIZE * CHANNELS];
  OpusDecoder *decoder;
  unsigned char cbits[MAX_PACKET_SIZE];

  // fs: sampling rate, one of 8000, 12000, 16000, 24000, 48000
  // channels: number of channels
  // application: encoding mode, there are three types:
  // OPUS_APPLICATION_VOIP: Processes voice signals, suitable for voip business scenarios
  // OPUS_APPLICATION_AUDIO: This mode is suitable for non-voice content such as music
  // OPUS_APPLICATION_RESTRICTED_LOWDELAY: low latency mode
  // error: Encoded return value

  decoder = opus_decoder_create(SAMPLE_RATE, CHANNELS, &err);
  if (err < 0)
  {
    fprintf(stderr, "failed to create an decoder: %s\n", opus_strerror(err));
    return -1;
  }
  printf("opus_decoder_create ok\n");

  decoded_frame = opus_decode(decoder, cbits, sizeof(pcm_buff), pcm_buff, sizeof(cbits), 0);
    if (decoded_frame < 0) {
      fprintf(stderr, "decoder failed: %s\n", opus_strerror(decoded_frame));
      return -1;
    }
    printf("opus_decode decoded_frame return: %d\n", decoded_frame);

  opus_decoder_destroy(decoder);
  return 0;
}
