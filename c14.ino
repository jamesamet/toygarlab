
// ================== Sabitler ve Pin Tanımları ====================
int sagduvarmin = 0;
int sagduvarmax = 800;
int solduvarmin = 0;
int solduvarmax = 800;
int sagduvarideal = 150; // SAĞ duvar takip mesafesi ayarlanır. 
int solduvarideal = 150;  // SOL duvar takip mesafesi ayarlanır.
// ================================================================ kullanıcı robot ayarları
int SPD = 205; // ROBOTUN HIZI
int On_Duvar_Mesafe=450; // ön  engel algılama değeri ; çarpma varsa değeri  azar azar arttırın ,  EN FAZLA 15CM OLMASINA DİKKAT EDİN  
float sag_u_kp=1.1; // artarsa u dönüş dar alır azalırsa geniş alır
float sol_u_kp=1.1; // artarsa u dönüş dar alır azalırsa geniş alır

int FanHiz1=160; // VAKUM HIZI ARTTIRILINCA  TUTUNMA ARTAR HIZ AZALIR
int FanHiz2=130; // VAKUM HIZI ARTTIRILINCA  TUTUNMA ARTAR HIZ AZALIR

int FanHiz3=150; // VAKUM HIZI ARTTIRILINCA  TUTUNMA ARTAR HIZ AZALIR
// ================================================================

// ================================================================sure ile takip yonu değiştirme şuan pasif  
unsigned long sonDegisimZamani = 0;
bool birinciDegisimYapildi = false;
bool ikinciDegisimYapildi = false;

const unsigned long birinciGecisSuresi = 5000;   // 5 sn sonra kontrol
const unsigned long ikinciGecisSuresi = 15000;   // toplam 15 sn sonra kontrol
int solsensor,sagsensor,onsensor;
long integral;
float i =0.00060;
int limit;

// ================================================================

// Motor pinleri
const int IN_A2 = 5;   // Sağ motor ileri/geri kontrol
const int IN_A1 = 6;
const int IN_B1 = 9;  // Sol motor ileri/geri kontrol
const int IN_B2 = 10;

#define FAN_PIN 3  // D3 pinine bağlı

// Sensör pinleri
const int SAG_PIN = A0;   
const int ONS_PIN = A1;   
const int SOL_PIN = A3;   
const int QTR_PIN = A2;   
const int start_pin=4; //kumanda ile start pın no

// Sabitler
const int NUM_READINGS = 14;// SENSORLER KAÇ KEZ OKUNSUN                      
int MAX_SPEED = 254;           
const int MIN_SPEED = -254;          
const unsigned long CONTROL_INTERVAL_US = 1000; // PID kontrol aralığı (mikrosaniye)

// Sabit min/max değerleri
const int FIXED_MIN = 30;// KALİBRE EDİLECEK MİN SENSÖR DEĞERİ
const int FIXED_MAX = 850;//KALİBRE EDİLECEK MAX SENSÖR DEĞERİ

// Takip yönü: 1 = sağ duvar, 0 = sol duvar
int takip = 1;

// Global değişkenler (PID)
int error = 0;
int lastError = 0;
unsigned long lastControlTime = 0;

// PID değerleri çalışma anında belirlenecek
float kp = 0;
int ki = 0;
float kd = 0;
int SPD2=0;
int oncekiFanHizi = 0;  // Son yazılan fan hızı
// ================================================================
void setup() {

ADCSRA = (ADCSRA & 0xF8) | 0x06;

// ============ TIMER AYARLARI (ATmega32U4 - Pro Micro) ============

                    // TOP = 255 (8-bit)

  pinMode(2, INPUT_PULLUP);//jumper
  pinMode(start_pin, INPUT); //kumanda ile başlama durumu varsa
 
  pinMode(ONS_PIN, INPUT);
  pinMode(SAG_PIN, INPUT);
  pinMode(SOL_PIN, INPUT);
  pinMode(QTR_PIN, INPUT);

  pinMode(IN_A2, OUTPUT);  
  pinMode(IN_A1, OUTPUT);  
  pinMode(IN_B1, OUTPUT);  
  pinMode(IN_B2, OUTPUT); 
  pinMode(FAN_PIN, OUTPUT); // Fan pinini çıkış olarak ayarla
  SPD2=SPD;
  // 1. Pin modları
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  // 2. Timer ayarları (pinMode'dan SONRA)
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11);

  TCCR3A = _BV(WGM30);
  TCCR3B = _BV(CS31);

  TCCR4A = 0;
  TCCR4B = 0x07;
  TCCR4C = _BV(PWM4D);
  TCCR4D = 0;
  TC4H   = 0x00;
  OCR4C  = 255;
    digitalWrite(IN_A1, LOW);
    digitalWrite(IN_A2, LOW);
     digitalWrite(IN_B1, LOW);
    digitalWrite(IN_B2, LOW);
  
  analogWrite(FAN_PIN, 100);
  oncekiFanHizi = 100;  // Başlangıç fan hızını kaydet
  
  
  //Serial.begin(115200);
 limit= readCalibratedSensor(ONS_PIN)-10;
   delay(1200);
//***********************************************************************************************************************
// ================================================================  ROBOTUN BAŞLATMA SEÇENEKLERİ YALNIZCA  1 TANESİ AKTİF OLMALI

// if(digitalRead(2)==1){ takip=1;} else{takip=0;}  // 1:sağ ******** JUMPERE GÖRE TAKİP :  JUMPER VARKEN SAĞ YOKKEN SOL
 
if(analogRead(SAG_PIN)<analogRead(SOL_PIN)){takip=1;}else{takip=0;}// HANGİ DUVARA YAKINSA TAKİBİ O TARAFA ALIR. 1:SAĞ TAKİP----  0:SOL TAKİP

// ================================================================   
//***********************************************************************************************************************


// ================================================================   kumanda BAŞLATMA MODÜLÜ VARSA  AKTİF ET VE **ZEMİN RENGİ** PASİF ET ALTTAKİ KODDA
  //while (digitalRead(start_pin) == LOW) { setMotors(0,0); }  // START bekleniyor (start_pin LOW)

// ================================================================
//***********************************************************************************************************************

sonDegisimZamani = millis();  // Başlangıç zamanı
}//setup sonu

void loop() {

//while(1){ setMotors(150,150);delay(2000); setMotors(0,0);delay(2000); setMotors(-150,-150);delay(2000); setMotors(0,0);delay(2000);}
  //***********************************************************************************************************************
  // ================================================================  degerler için test yaparken setuptan  Serial.begin(115200);  aktif et 
//sensortest();
//motortest();
  //***********************************************************************************************************************

  
  // ================================================================  zamana gore yon değiştirme işlemleri şuan pasif 
     //zamantakipyon();
  // ================================================================  
  
   // ================================================================  // STOP basıldı mı kontrol et   kumanda ile başlatma varsa aktif et
  //if (digitalRead(start_pin) == LOW) { setMotors(0,0); delay(200000000000); }
   // ================================================================
   //***********************************************************************************************************************




// ================================================================ZEMİNDE BEYAZ VARSA DURMASI İÇİN (ZEMİN RENGİ ONEMLİ DEĞİLSE BU BLOGU PASİF YAPIN) 
    if ((analogRead(QTR_PIN) < 600)){ setMotors(50, 50); delay(250); setMotors(0, 0); delay(500); analogWrite(FAN_PIN, 0); setMotors(0, 0); delay(100000000);      }
// ================================================================






  
  int engel = 0;

  // Engel kontrolü  onde duvar gorurse ne yapsın 
  if (takip == 1) {
    engel = onKontrolSag();
  } else {
    engel = onKontrolSol();
  }



  // ============ ERROR HESAPLAMA (sadece engel yoksa) ============
  if (!engel) {
    // Engel yoksa normal takip - error hesapla
    if (takip == 1) {
      error = errorSag();
    } else {
      error = errorSol();
    }
    sensoroku();
  }

  // ============ HIZ VE FAN KONTROLÜ (Her zaman çalışır) ============
  // Başlangıç: Normal hız ve fan
  SPD = SPD2;
  int fanHizi = FanHiz2;
  int hizDegisimi = 0;  // Toplam hız değişimi

  // 1. KONTROL: Önde duvar varsa yavaşla
  if (onsensor < 800) {
    hizDegisimi -= 1;  // Hızdan 20 çıkar
    fanHizi = FanHiz1;  // Fan gücünü artır
  }
  /*  if (onsensor < 100) {
   
    fanHizi = FanHiz3;  // Fan gücünü artır
  }*/

  // 2. KONTROL: Takip edilen yanda duvar yoksa yavaşla
  if (takip == 1) {
    // Sağ duvar takibindeyken
    if (sagsensor > 800) {
      MAX_SPEED=220;
      hizDegisimi -= 1;  // Hızdan 25 daha çıkar
      fanHizi = FanHiz1 ;  // Fan daha da güçlü
    }
    else{ MAX_SPEED=254;}
  } else {
    // Sol duvar takibindeyken
    if (solsensor > 800) {
      MAX_SPEED=220;
      hizDegisimi -= 1;  // Hızdan 25 daha çıkar
      fanHizi = FanHiz1 ;  // Fan daha da güçlü
    }
    else{ MAX_SPEED=254;}
  }

  // 3. KONTROL: Ön mesafe limitten büyükse sürekli hızlan
  if (limit < onsensor) {
    hizDegisimi += 3;  // Hıza 3 ekle
  }

  // Toplam hız değişimini uygula
  SPD = SPD2 + hizDegisimi;

  // Güvenlik: PWM maksimum değeri 254
  if (SPD > 254) SPD = 254;

  // Fan ayarını uygula (sadece değiştiğinde yaz - kasma önleme)
  if (fanHizi != oncekiFanHizi) {
    analogWrite(FAN_PIN, fanHizi);
    oncekiFanHizi = fanHizi;
  }

  // ============ PID KONTROLÜ (Her zaman çalışır) ============
  unsigned long simdikiZaman = micros();

  // Her 200 mikrosaniyede bir PID hesapla (yüksek hız için gerekli)
  if (simdikiZaman - lastControlTime >= 200) {
    double P, D;  // Proportional ve Derivative değerleri

    // Integral hesaplama (hata birikimi)
    integral += error;

    // Eğer hata düzeliyorsa integral'i sıfırla (windup önleme)
    if (abs(error) < abs(lastError)) {
      integral = 0;
    }

    // PID hesaplamaları
    P = error * (double)kp;  // Orantısal kısım
    D = (error - lastError) * (double)kd;  // Türevsel kısım
    ki = i * integral;  // İntegral kısım
    lastError = error;  // Son hatayı kaydet

    // Toplam düzeltme değeri
    float duzeltmeDegeri = P + ki + D;

    // Motor hızlarını hesapla
    int solMotorHiz = SPD - duzeltmeDegeri;
    int sagMotorHiz = SPD + duzeltmeDegeri;

    // Hızları güvenli sınırlar içinde tut (manuel constrain - daha hızlı)
    if (solMotorHiz > MAX_SPEED) solMotorHiz = MAX_SPEED;
    else if (solMotorHiz < 60) solMotorHiz = 60;

    if (sagMotorHiz > MAX_SPEED) sagMotorHiz = MAX_SPEED;
    else if (sagMotorHiz < 60) sagMotorHiz = 60;

    // Motorları çalıştır
    setMotors(solMotorHiz, sagMotorHiz);
    lastControlTime = simdikiZaman;
  }
}
