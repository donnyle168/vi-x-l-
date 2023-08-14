#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_TEMPLATE_ID "TMPL61CrgLpZp"
#define BLYNK_TEMPLATE_NAME "CẢNH BÁO KHÍ GAS"
#define BLYNK_AUTH_TOKEN "rOqSv3qQtM8sUIwa9IvrDuuZGDCq4HVt"

char auth[] = "rOqSv3qQtM8sUIwa9IvrDuuZGDCq4HVt"; // Token Blynk của bạn

const char* ssid = "tp-link";
const char* password = "anhvu6868";

const int gasPin = A0; // Chân kết nối cảm biến MQ2 với chân Analog A0
const int buzzerPin = 12; // Chân kết nối loa buzzer với chân Digital 5
const int ledPin = V2; // Đèn LED ảo kết nối với Virtual Pin V2 trong Blynk

int thresholdValue = 500; // Ngưỡng cảnh báo, mặc định là 500
int gasValue = 0;
int alertState = 0; // Trạng thái cảnh báo, 0 là tắt, 1 là bật

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(buzzerPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Blynk.begin(auth, ssid, password);

  timer.setInterval(1000L, checkGas);
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkGas() {
  gasValue = analogRead(gasPin);
  Blynk.virtualWrite(V1, gasValue/4); // Hiển thị giá trị cảm biến MQ2 trên biểu đồ trong Blynk

  if (gasValue/4 > thresholdValue) {
    if (alertState == 0) {
      Blynk.virtualWrite(V2, HIGH); // Bật đèn LED ảo
      playBuzzer(); // Kích hoạt loa buzzer
      alertState = 1; // Đánh dấu đã có cảnh báo
    }
  } else {
    if (alertState == 1) {
      Blynk.virtualWrite(V2, LOW); // Tắt đèn LED ảo
      alertState = 0; // Đánh dấu cảnh báo đã được xử lý
      stopBuzzer(); // Dừng loa buzzer khi giá trị gas giảm xuống mức cài đặt
    }
  }
}

void playBuzzer() {
  digitalWrite(buzzerPin, HIGH); // Kích hoạt loa buzzer
}

void stopBuzzer() {
  digitalWrite(buzzerPin, LOW); // Tắt loa buzzer
}


BLYNK_WRITE(V3) {
  thresholdValue = param.asInt(); // Đọc giá trị ngưỡng cảnh báo từ thanh trượt slider
}
