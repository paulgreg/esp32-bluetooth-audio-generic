
#include "parameters.h"

#include <SPI.h>
#include <VS1053.h>
#include "player.h"

#include "BluetoothA2DPSink.h"
#include <cbuf.h>
#include "bluetoothsink.h"

BluetoothA2DPSink a2dp_sink;

#define VOLUME  80 // Default volume

void setup() {
  Serial.begin(115200);
  Serial.println("Bluetooth Audio Receiver");
  delay(500);

  SPI.begin();

  player.begin();
  if (player.getChipVersion() == 4) { // Only perform an update if we really are using a VS1053, not. eg. VS1003
      player.loadDefaultVs1053Patches(); 
  }
  player.switchToMp3Mode();
  player.setVolume(VOLUME);

  circBuffer.flush();

  a2dp_sink.set_stream_reader(read_data_stream, false);
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  a2dp_sink.start(BLUETOOTH_NAME);
  delay(100);
  circBuffer.write((char *)bt_wav_header, 44);
  delay(100);

}

void loop() {
  handle_stream();
}