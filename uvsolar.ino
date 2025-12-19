#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <SparkFun_AS7331.h>
 
// ---- I2C pins for XIAO ESP32-S3 ----
#define I2C_SDA 5   // D4
#define I2C_SCL 6   // D5
 
// ---- Devices ----
Adafruit_INA219 ina219(0x40);   // Your scan shows 0x40
SfeAS7331ArdI2C uv;             // AS7331 UV sensor
 
// Try AS7331 at these addresses (your scan says 0x51)
const uint8_t UV_ADDRS[] = {0x51, 0x39, 0x30};
 
bool beginAS7331()
{
  // Try default first
  if (uv.begin()) return true;
 
  // Then try explicit addresses: address first, then Wire
  for (uint8_t i = 0; i < sizeof(UV_ADDRS); ++i) {
    if (uv.begin(UV_ADDRS[i], Wire)) return true;
  }
  return false;
}
 
// Average INA219 over n samples to smooth sunlight flicker
void readINA219Averaged(uint8_t n, float &v_load_V, float &i_mA, float &p_mW)
{
  float vbus = 0, vshunt_mV = 0, imA = 0, pmW = 0;
  for (uint8_t k = 0; k < n; k++) {
    float vb = ina219.getBusVoltage_V();     // V at VIN-
    float vs = ina219.getShuntVoltage_mV();  // mV across shunt
    float i  = ina219.getCurrent_mA();       // mA (+ when VIN+->VIN-)
    float vload = vb + (vs / 1000.0f);       // estimate panel terminal voltage
    float p = vload * i;                     // mW
 
    vbus += vb; vshunt_mV += vs; imA += i; pmW += p;
    delay(10);
  }
  v_load_V = (vbus / n) + (vshunt_mV / n) / 1000.0f;
  i_mA     = imA / n;
  p_mW     = pmW / n;
}
 
void setup()
{
  Serial.begin(115200);
  delay(200);
 
  Wire.begin(I2C_SDA, I2C_SCL);
  delay(50);
 
  // INA219 (0x40)
  if (!ina219.begin()) {
    Serial.println("ERROR: INA219 (0x40) not found. Check wiring.");
    while (1) delay(10);
  }
  // Higher precision for small panel currents
  ina219.setCalibration_16V_400mA();
 
  // AS7331 (0x51 per your scan)
  if (!beginAS7331()) {
    Serial.println("ERROR: AS7331 not found (tried 0x51/0x39/0x30).");
    while (1) delay(10);
  }
  if (!uv.prepareMeasurement(MEAS_MODE_CMD)) {
    Serial.println("ERROR: AS7331 prepareMeasurement failed.");
    while (1) delay(10);
  }
 
  Serial.println("# Ready: INA219@0x40 + AS7331 (one-shot)");
  Serial.println("time_s,V_load_V,I_mA,P_mW,UVA,UVB,UVC"); // CSV header
}
 
void loop()
{
  static uint32_t t0 = millis();
  float t_sec = (millis() - t0) / 1000.0f;
 
  // Start one-shot UV conversion
  if (ksfTkErrOk != uv.setStartState(true)) {
    Serial.println("WARN: AS7331 start failed");
  }
 
  // Conversion time for AS7331; sample INA219 during this window
  uint16_t cvt_ms = 2 + uv.getConversionTimeMillis();
  uint8_t n_samples = max<uint16_t>(1, cvt_ms / 20); // ~20 ms/sample
 
  float Vload, ImA, PmW;
  readINA219Averaged(n_samples, Vload, ImA, PmW);
 
  // Ensure conversion time has passed
  delay(cvt_ms);
 
  // Read UV values
  if (ksfTkErrOk != uv.readAllUV()) {
    Serial.println("WARN: AS7331 readAllUV failed");
  }
 
  float uva = uv.getUVA();
  float uvb = uv.getUVB();
  float uvc = uv.getUVC();
 
  // One CSV line
  Serial.print(t_sec, 2);  Serial.print(",");
  Serial.print(Vload, 3);  Serial.print(",");
  Serial.print(ImA, 1);    Serial.print(",");
  Serial.print(PmW, 1);    Serial.print(",");
  Serial.print(uva, 3);    Serial.print(",");
  Serial.print(uvb, 3);    Serial.print(",");
  Serial.println(uvc, 3);
 
  // ~2 Hz overall
  delay(200);
}